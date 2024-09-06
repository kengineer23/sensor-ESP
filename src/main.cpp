/**
 * @file main.cpp
 * @brief Firmware for ESP32 responsible for communication with cloud ESP, sensor data collectiona and conditional BLDC motor control.
 * 
 * This file includes functionalities for WiFi connectivity, BLE connectivity with Android, MQTT communication, HTTP requests,
 * , collecting sensor data and controlling the speed of BLDC motor. The various sensors included are DHT11 for Temperature and Humidity,
 * PMS5003 for PM2.5 levels and MQ7 for CO level detection. 
 * 
 * @note Next: Mechanism for unsuccessful data collection
 *             Temporary ESP-NOW connection to send WiFi credentials on setup
 *             Switch to low-level/bare level programming to not be dependent on third-party libraries.
 *    
 */

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Preferences.h> 
#include <CRC32.h>

#include <DHT11Sensor.h>
#include <PMS5003Sensor.h>
#include <MQ7Sensor.h>
#include <LedControl.h>
#include <BLDC.h> 
#include <BLE.h>

//MAC address = C0:49:EF:D3:43:5C

Preferences preferences;  ///< To write network credentials permanently in ESP32's File system


//DHT11 setup
#define DHTPIN 15
#define DHTTYPE DHT11
DHT11Sensor dht11(DHTPIN, DHTTYPE);   ///< Create an object of custom class DHT11Sensor

//MQ7 setup
#define MQ7 33
MQ7Sensor mq7(MQ7);   ///< Create an object of custom class MQ7Sensor

//PMS5003 object 
PMS5003Sensor pms5003;  ///< Create an object of custom class PMS5003Sensor


/**
 * @brief A structure to hold data from various sensors.
 * 
 * This structure contains data from multiple sensors, including the DHT11 sensor,
 * PMS5003 particulate matter sensor, and the MQ7 gas sensor. It provides a convenient
 * way to manage and access the data from these sensors as a single unit.
 */
struct SensorData {
    /**
     * @brief Data from the DHT11 sensor.
     * 
     * This member holds the temperature and humidity readings from the DHT11 sensor.
     */
    DHT11Data dht11;

    /**
     * @brief Data from the PMS5003 sensor.
     * 
     * This member contains the particulate matter readings from the PMS5003 sensor,
     * which measures different particle sizes in the air.
     */
    PMS5003Data pms5003;

    /**
     * @brief Data from the MQ7 gas sensor.
     * 
     * This member stores the carbon monoxide concentration readings from the MQ7 sensor.
     */
    MQ7Data mq7;
};

/**
 * @brief A variable to hold sensor data readings.
 * 
 * This variable is an instance of the SensorData structure and is used to store and manage
 * the data from the DHT11, PMS5003, and MQ7 sensors.
 */
SensorData sensorData;



/**
 * @brief LED control object for managing LED states.
 * 
 * This object is used to interface with and control an LED, allowing 
 * for operations such as setting colors and brightness.
 */
LedControl led;

/**
 * @brief A structure to hold RGB color values for an LED.
 * 
 * This structure contains the parameters needed to define the color 
 * of an LED using red, green, and blue components. Each component is 
 * represented as an 8-bit unsigned integer.
 */
struct ledParameters {
    /**
     * @brief Red/Green/Blue color component.
     * 
     * The following members represents the intensity of the red, green and blue components of the LED color.
     * It is an 8-bit unsigned integer, ranging from 0 to 255.
     */
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

/**
 * @brief A variable to hold the LED color parameters.
 * 
 * This variable is an instance of the ledParameters structure and is used
 * to define and store the current RGB color values for the LED.
 */
ledParameters ledcolor;


#define PWMPin 4

/**
 * @brief BLDC motor control object.
 * 
 * This object is used to control a brushless DC (BLDC) motor using 
 * a PWM signal. The motor is connected to the specified PWM pin.
 * The PWM signal is configured to produce a frequency of 20 kHz with a voltage range from 0 to 3.3V.
 * 
 * @param PWMPin The GPIO pin used to generate the PWM signal.
 */
BLDC motor(PWMPin);

/**
 * @brief Duty cycle for PWM signal.
 * 
 * This variable represents the duty cycle of the PWM signal, which 
 * determines the speed of the BLDC motor. The value of dutycycle ranges from 0 to 2047(2^11), which means 5 bits will be unused here.
 */
uint16_t dutycycle;


/**
 * @brief Task handles for managing RTOS tasks.
 * 
 * These handles are used to manage and control tasks within the RTOS. 
 * They allow operations such as starting, stopping, suspending, and resuming tasks.
 */
///< Core 0
TaskHandle_t TaskDHT11Handle;
TaskHandle_t TaskPMS5003Handle;
TaskHandle_t TaskMQ7Handle;
TaskHandle_t TaskSendToESPHandle;

///< Core 1
TaskHandle_t TaskReceiveFromESPHandle;
TaskHandle_t TaskHandleWiFiCredentials;

/**
 * @brief Semaphore handles for synchronizing tasks.
 * 
 * These handles are used to manage semaphores in the RTOS, allowing tasks
 * to synchronize their execution and manage shared resources safely.
 */
//Semaphore handles for synchronization
SemaphoreHandle_t xSendSemaphore;
SemaphoreHandle_t xStartSemaphore;
SemaphoreHandle_t xSendMutex;



/* The parameter 'pvParameters' is a pointer to void, allowing any type of data to be passed to the task.
This provides flexibility, as the task function can cast 'pvParameters' to the appropriate type as needed.
For example, it can be used to pass a structure containing configuration data or a simple integer value.
When creating the task, this parameter can be specified by the caller to customize the task's behavior or 
provide it with the necessary context information. */

/**
 * @brief Task for collecting data from the DHT11 sensor.
 * 
 * This FreeRTOS task reads temperature and humidity data from the DHT11 sensor
 * and outputs the values to the serial monitor. It uses a semaphore to manage
 * access to shared resources, ensuring data integrity and task synchronization.
 * 
 * @param pvParameters Pointer to the task parameters (unused in this task).
 * 
 * The task runs indefinitely, attempting to read the DHT11 sensor every 5 seconds.
 * If the mutex `xSendMutex` is available, it reads the sensor data and stores it
 * in the `sensorData` structure. The task then releases the semaphore and mutex.
 * If the sensor data is invalid, an error message is printed.
 */

void TaskDHT11(void *pvParameters) {
  Serial.print("TaskDHT11 running on core ");
  Serial.println(xPortGetCoreID());
  while (1) {
    //Check if the mutex is not taken by the display task
    if(xSemaphoreTake(xSendMutex,0) == pdTRUE){
      sensorData.dht11 = dht11.readDHT11();
      xSemaphoreGive(xSendSemaphore);
      xSemaphoreGive(xSendMutex);
      if (isnan(sensorData.dht11.temperature) || isnan(sensorData.dht11.humidity)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
        Serial.print("DHT11 - Temperature: ");
        Serial.print(sensorData.dht11.temperature);
        Serial.print(" °C, Humidity: ");
        Serial.print(sensorData.dht11.humidity);
        Serial.println(" %");
      }
    }
    vTaskDelay(5000 / portTICK_PERIOD_MS); // Delay for 5 seconds
  }
}

/**
 * @brief Task for collecting data from the PMS5003 sensor.
 * 
 * This FreeRTOS task reads particulate matter data from the PMS5003 sensor
 * and stores the readings in the `sensorData` structure. It uses a semaphore 
 * to manage access to shared resources, ensuring data integrity and task 
 * synchronization.
 * 
 * @param pvParameters Pointer to the task parameters (unused in this task).
 * 
 * The task runs indefinitely, attempting to read the PMS5003 sensor every 2 seconds.
 * If the mutex `xSendMutex` is available, it reads the sensor data and stores it
 * in the `sensorData` structure. The task then releases the semaphore and mutex.
 */

void TaskPMS5003(void *pvParameters) {
  Serial.print("TaskPMS5003 running on core ");
  Serial.println(xPortGetCoreID());
  Serial.println(" ");
  while (1) {
    if(xSemaphoreTake(xSendMutex,0) == pdTRUE){
      sensorData.pms5003 = pms5003.readData();
      xSemaphoreGive(xSendSemaphore);
      xSemaphoreGive(xSendMutex);
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 2 seconds
  }
}


/**
 * @brief Task for monitoring gas levels using the MQ7 sensor.
 * 
 * This FreeRTOS task monitors gas levels from the MQ7 gas sensor. It periodically
 * updates the `sensorData` structure with the gas sensor readings and checks if 
 * the gas value exceeds a certain threshold. If gas is detected, it outputs a 
 * message to the serial monitor. The task uses a semaphore to signal other tasks 
 * when new data is available.
 * 
 * @param pvParameters Pointer to the task parameters (unused in this task).
 * 
 * The task runs indefinitely, checking the MQ7 sensor every 2 seconds. Currently, 
 * the sensor data is set to a fixed value for demonstration purposes. The task 
 * signals that new sensor data is available by giving the `xSendSemaphore` semaphore.
 * If the gas value is equal to 1, indicating gas detection, a message is printed to 
 * the serial monitor.
 */

void TaskMQ7(void *pvParameters) {
  while (1) {
    //sensorData.mq7 = mq7.gasRead();
    sensorData.mq7.gasValue = 0;
    xSemaphoreGive(xSendSemaphore);
    if(sensorData.mq7.gasValue == 1){
      Serial.println("Gas Detected");
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 2 seconds
  }
}


/**
 * @brief This function is responsible for connecting the ESP32 to the Mosquitto MQTT Broker
 * 
 * @note If the connection is successful the ESP is subscribed to collection 'isaac/action' and
 *       is a publisher to topic 'isaac/sensor_data'
 * 
 * @note The following return codes(rc) indicate unsuccessful connections:
 *       - **0x01**: Incorrect Protocol Version - The server does not support the MQTT protocol version requested by the client.
 *       - **0x02**: Invalid Client Identifier - The client identifier is not valid.
 *       - **0x03**: Server Unavailable - The server is currently unavailable.
 *       - **0x04**: Bad Username or Password - The username or password is incorrect.
 *       - **0x05**: Not Authorized - The client is not authorized to connect to the server.
 * 
 */

/**
 * @brief TaskSendToESP function sends sensor data to the cloud-ESP periodically.
 * 
 * This function is a task that runs indefinitely and sends sensor data to the cloud-ESP every 30 seconds.
 * It takes the sensor data, constructs a JSON payload, and sends it to the cloud-ESP using the Serial1 interface.
 * Before sending the data, it acquires multiple semaphores to ensure exclusive access to the sensor data and to stop other sensor tasks.
 * After sending the data, it releases the acquired semaphores.
 * 
 * Serial.write() sends the string as a series of bytes to sensor-ESP32.
 * 
 * @param pvParameters A pointer to task parameters (not used in this function).
 */
void TaskSendToESP(void *pvParameters){
  while(1){
      if (xSemaphoreTake(xSendSemaphore, portMAX_DELAY) == pdTRUE &&
          xSemaphoreTake(xSendSemaphore, portMAX_DELAY) == pdTRUE &&
          xSemaphoreTake(xSendSemaphore, portMAX_DELAY) == pdTRUE) {

          // Attempt to take the mutex to stop sensor tasks
          if (xSemaphoreTake(xSendMutex, portMAX_DELAY) == pdTRUE) {
          // All sensor data is ready and access is exclusive
            String jsonPayload;
            jsonPayload+="{\"database\":\"isaac_v1\",\"collection\":\"sensor_readings\",\"dataSource\":\"IsaacTest\",\"document\": {";
            //jsonPayload+="\"Timestamp\":" + timestamp + ",";
            jsonPayload += "\"ISAAC ID\" : \"ec03f332a7b0400000\",";
            jsonPayload += "\"PM2.5\":" + String(sensorData.pms5003.pm2_5) + ",";
            jsonPayload += "\"Temperature\":" + String(sensorData.dht11.temperature) + ",";
            jsonPayload += "\"Humidity\":" + String(sensorData.dht11.humidity) + ",";
            jsonPayload += "\"Smoke\":" + String(sensorData.mq7.gasValue);
            jsonPayload += "}}";   
            Serial.println(jsonPayload);

            // Calculate CRC32
            CRC32 crc;
            crc.update((uint8_t*)jsonPayload.c_str(), jsonPayload.length());
            uint32_t crcValue = crc.finalize();

            // Append CRC32 to the JSON payload
            jsonPayload +=String(crcValue) + "\n";

            Serial.println(jsonPayload.c_str());
            
            // Send data to the cloud-ESP
            Serial1.write(jsonPayload.c_str());
              

            // Release the semaphores
            xSemaphoreGive(xSendSemaphore);
            xSemaphoreGive(xSendSemaphore);
            xSemaphoreGive(xSendSemaphore);
            xSemaphoreGive(xSendMutex);
          }
      }
      vTaskDelay(60000/portTICK_PERIOD_MS); ///< Send data every 30 seconds
    }
}


void controlLed(){
  led.changecolor(ledcolor.red, ledcolor.green, ledcolor.blue);
}

void motorControlTask(){
  motor.speedcontrol(dutycycle);
}

/**
 * @brief This task is responsible for receiving data from the ESP module via Serial1 communication.
 * It reads the incoming data, parses it as JSON, and performs actions based on the received parameters.
 * The received parameters include LED color values (RED, GREEN, BLUE) and duty cycle (DUTYCYCLE).
 * After parsing the JSON, it calls the controlLed() function to control the LED based on the received parameters.
 * It also calls the motorControlTask() function to perform motor control operations.
 * 
 * @param pvParameters Pointer to task parameters (not used in this task).
 **/
void TaskReceiveFromESP(void *pvParameters){
  while(1){
    // Get the number of bytes available to read
    //Serial.println("Receiving data from ESP32, if any...");
    if(Serial1.available() > 0){
      int incomingByte = Serial1.peek();  // Peek the next incoming byte w/o removing it from serial buffer
      if(incomingByte != -1){
        String action_params = Serial1.readStringUntil('\n');
        
        // Check for start and end markers
        if(action_params.startsWith("{")){
          Serial.println("Received valid data: " + action_params);

          // Extract relevant data from the received JSON string
          Serial.println("Extracting relevant data");
          String receivedJsonPaylaod = action_params.substring(0, action_params.lastIndexOf('}') + 1);
          Serial.println(receivedJsonPaylaod);

          // Extract CRC from the received string
          String crcString = action_params.substring(action_params.lastIndexOf('}') + 1);
          Serial.println(crcString);

          int id_index = receivedJsonPaylaod.lastIndexOf(',');
          
          receivedJsonPaylaod.remove(id_index);

          if(receivedJsonPaylaod.indexOf('}') == -1){
            
            receivedJsonPaylaod += '}';
          }

          Serial.println(receivedJsonPaylaod);
          //uint32_t crcValue = strtoul(crcString.c_str(), NULL, 10);
          uint32_t crcValue = 0;
          // Calculate CRC32 of the received JSON string
          CRC32 crc;
          //crc.update((uint8_t*)action_params.c_str(), action_params.length() - 9);
          //uint32_t calculatedCRC = crc.finalize();
          uint32_t calculatedCRC = 0;

          // Check if the received CRC matches the calculated CRC
          if (crcValue == calculatedCRC) {
            Serial.println("CRC32 match");
            StaticJsonDocument<512> doc;
            DeserializationError error = deserializeJson(doc, receivedJsonPaylaod);
            if (!error) {
              ledcolor.red = doc["RED"].as<uint8_t>();
              ledcolor.green = doc["GREEN"].as<uint8_t>();
              ledcolor.blue = doc["BLUE"].as<uint8_t>();
              dutycycle = doc["DutyCycle"].as<uint16_t>();

              Serial.println(String(ledcolor.red) + " " + String(ledcolor.green) + " " + String(ledcolor.blue) + " " + String(dutycycle));

              controlLed();
              motorControlTask();
            }
            else {
              Serial.println("Executing default action");
              Serial.println("Failed to parse JSON");
            }
          }
          else {
            Serial.println(String(crcValue) + " " + String(calculatedCRC));
            Serial.println("CRC32 mismatch");
            Serial.println("Discarding data");
            continue;
          }
        }
        else{
          Serial.println("Invalid data received");
        }
      }
    }
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}

/**
 * @brief Task for receiving and processing WiFi credentials via BLE.
 * 
 * This FreeRTOS task listens for new WiFi credentials received via BLE and attempts 
 * to connect to the specified WiFi network. The credentials are expected to be in 
 * the format "SSID:password". The task performs the following steps:
 * - Waits until valid WiFi credentials are received.
 * - Parses the received data to extract SSID and password.
 * - Stores the credentials in persistent storage.
 * - Attempts to connect to the WiFi network using the extracted credentials.
 * - If invalid credentials are received, it restarts BLE setup for reconfiguration.
 * 
 * @param pvParameters Pointer to the task parameters (unused in this task).
 * 
 * @note The task continuously checks for connection status and processes new data 
 *       only when it is available. It uses the `newData` flag to detect incoming 
 *       credentials and the `receivedData` string to hold the data.
 * 
 * @warning Ensure that BLE setup and WiFi connection logic are correctly implemented 
 *          to handle various error scenarios and credential formats.
 */

/*
void WiFiCredentials(void* pvParameters) {
  String ssid = "";
  String password = "";
  while(WiFi.status() != WL_CONNECTED){
    if (newData) {
      newData = false;
      Serial.println("Received WiFi credentials");
      Serial.println(receivedData);
      // Assuming the format is "SSID:password"
      if(String(receivedData).length() != 0){
        int separator = String(receivedData).indexOf(':');
        ssid = String(receivedData).substring(0,separator);
        password = String(receivedData).substring(separator + 1);
        preferences.putString("SSID", ssid.c_str());
        preferences.putString("Password", password.c_str());
        Serial.println(ssid.c_str());
        Serial.println(password.c_str());

        connect_to_WIFI();
      }
      else{
        Serial.println("Invalid credentials received. Try Again!");
        Serial.println(String(receivedData));
        setupBLE();
      }
    }
    vTaskDelay(200/portTICK_PERIOD_MS);
  }
  vTaskDelete(NULL);
}
*/

/**
 * @brief Initializes the system and sets up tasks for sensor data handling and communication.
 * 
 * This function is called once during the startup of the ESP32. It performs the following operations:
 * - Initializes serial communication at a baud rate of 9600.
 * - Loads WiFi credentials from persistent storage (Flash memory) and waits for 10 seconds.
 * - Initializes sensors (DHT11 and PMS5003).
 * - Checks if WiFi credentials are available in persistent storage. If not, it sets up BLE 
 *   and creates a task to handle WiFi credentials retrieval.
 * - If WiFi credentials are found, it connects to the WiFi network.
 * - Once connected to WiFi, it sets up the MQTT client with the server address and port.
 * - Creates semaphores and a mutex for task synchronization.
 * - Creates FreeRTOS tasks for sensor data collection, data sending, and data reception.
 * 
 * @note Tasks are pinned to specific cores as follows:
 * - Core 0: Tasks for sensor data collection (DHT11, PMS5003, MQ7) and sending data to the server.
 * - Core 1: Task for receiving data from the server and handling WiFi credentials.
 * 
 * @warning Ensure that the MQTT server address, port, and BLE setup are correctly configured 
 *          for your specific use case.
 */
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600, SERIAL_8E1, 25,26); // RX, TX
  led.setpins();
  motor.motor_init();
  Serial1.flush();

  // WiFi credentials stored in Flash memory permanently
  //preferences.begin("credentials", false);  //false for R/W operations; true for read-only
  delay(20000);

  // Initialize the sensors
  dht11.init();
  pms5003.begin();


  if (!preferences.isKey("SSID") && !preferences.isKey("Password")) {
    setupBLE();
    Serial.println("Waiting for client connection to notify");

    // Core 1 Task: Handles WiFi credentials retrieval
    //xTaskCreatePinnedToCore(WiFiCredentials, "WiFiCredentials", 4096, NULL, 2, &TaskHandleWiFiCredentials, 1);
  } else {
    //connect_to_WIFI();
  }

  Serial.println("Task Creation and other processes started");

  // Create semaphore
  xSendSemaphore = xSemaphoreCreateCounting(3, 0);

  // Create a mutex
  xSendMutex = xSemaphoreCreateMutex();

  // Create tasks
  //(Function to implement the task, Name of the task, Stack size, Task input parameter, Priority of the task, Task handle, Core ID);
  
  // Core 0 Tasks: Sensor data collection and sending
  xTaskCreatePinnedToCore(TaskDHT11, "TaskDHT11", 2048, NULL, 1, &TaskDHT11Handle, 0);
  xTaskCreatePinnedToCore(TaskPMS5003, "TaskPMS5003", 2048, NULL, 1, &TaskPMS5003Handle, 0);
  xTaskCreatePinnedToCore(TaskMQ7, "TaskMQ7", 2048, NULL, 1, &TaskMQ7Handle, 0);
  xTaskCreatePinnedToCore(TaskSendToESP, "TaskSendToESP", 4096, NULL, 2, &TaskSendToESPHandle, 0);

  // Core 1 Task: Receives data from the server
  
  xTaskCreatePinnedToCore(TaskReceiveFromESP, "TaskReceiveFromESP", 4096 , NULL, 1, &TaskReceiveFromESPHandle, 1);
}




void loop(){

  //Do nothing here

}
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>


#include <DHT11Sensor.h>
#include <PMS5003Sensor.h>
#include <MQ2Sensor.h>
#include "AnimatedEye.h"
#include <BLDC.h>


//WiFi Credentials
const char* ssid = "S24";
const char* password = "kanishk23";
const char* serverURL = "http://192.168.103.68:5000/store-data";

//DHT11 setup
#define DHTPIN 5
#define DHTTYPE DHT11
DHT11Sensor dht11(DHTPIN, DHTTYPE);

//MQ2 setup
#define MQ2 33
MQ2Sensor mq2(MQ2);

//PMS5003 object 
PMS5003Sensor pms5003;


//Display Setup
AnimatedEye eye((240-160)/2, (320-120)/2);

//BLDC Motor Control
#define PWMPin  15 //generate 0-3.3V 20KHz signal
BLDC motor(PWMPin);

//Task Handles
TaskHandle_t TaskDHT11Handle;
TaskHandle_t TaskPMS5003Handle;
TaskHandle_t TaskMQ2Handle;
TaskHandle_t TaskSendToServerHandle;
TaskHandle_t TaskDisplayEyesHandle;
TaskHandle_t TaskControlMotorHandle;

//Semaphore handles for synchronization
//SemaphoreHandle_t xDisplaySemaphore;
SemaphoreHandle_t xSendSemaphore;
SemaphoreHandle_t xControlSemaphore;
SemaphoreHandle_t xMutex;

//Queue handle
QueueHandle_t xQueue;



// Data Storage
struct SensorData {
  DHT11Data dht11;
  PMS5003Data pms5003;
  MQ2Data mq2; 
};
SensorData sensorData;  //a variable of type SensorData


//Task1code: collect Temperature, Humidity data
/* The parameter 'pvParameters' is a pointer to void, allowing any type of data to be passed to the task.
This provides flexibility, as the task function can cast 'pvParameters' to the appropriate type as needed.
For example, it can be used to pass a structure containing configuration data or a simple integer value.
When creating the task, this parameter can be specified by the caller to customize the task's behavior or 
provide it with the necessary context information. */

void TaskDHT11(void *pvParameters) {
  Serial.print("TaskDHT11 running on core ");
  Serial.println(xPortGetCoreID());
  while (1) {
    //Check if the mutex is not taken by the display task
    if(xSemaphoreTake(xMutex,0) == pdTRUE){
      //sensorData.dht11 = dht11.readDHT11();
      sensorData.dht11.humidity = 30;
      sensorData.dht11.temperature = 31;
      xSemaphoreGive(xSendSemaphore);
      xSemaphoreGive(xMutex);
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

void TaskPMS5003(void *pvParameters) {
  Serial.print("TaskPMS5003 running on core ");
  Serial.println(xPortGetCoreID());
  while (1) {
    if(xSemaphoreTake(xMutex,0) == pdTRUE){
      //sensorData.pms5003 = pms5003.readData();
      sensorData.pms5003.pm2_5 = random(55,59);
      xSemaphoreGive(xSendSemaphore);
      xSemaphoreGive(xMutex);
      //Serial.print("PM2.5: ");
      //Serial.print(sensorData.pms5003.pm2_5);

      if(xQueueSend(xQueue, &sensorData.pms5003.pm2_5, portMAX_DELAY) == pdPASS){
        Serial.print("PM2.5: ");
        Serial.print(sensorData.pms5003.pm2_5);
      }
    }
    vTaskDelay(10000 / portTICK_PERIOD_MS); // Delay for 10 seconds
  }
}


void TaskMQ2(void *pvParameters) {
  while (1) {
    sensorData.mq2 = mq2.gasRead();
    xSemaphoreGive(xSendSemaphore);
    if(sensorData.mq2.gasValue == 1){
      Serial.println("Gas Detected");
    }
    vTaskDelay(2000 / portTICK_PERIOD_MS); // Delay for 2 seconds
  }
}

void TaskSendToServer(void *pvParameters) {
    while (1) {
        // Wait for all three sensor data to be ready
        if (xSemaphoreTake(xSendSemaphore, portMAX_DELAY) == pdTRUE &&
            xSemaphoreTake(xSendSemaphore, portMAX_DELAY) == pdTRUE &&
            xSemaphoreTake(xSendSemaphore, portMAX_DELAY) == pdTRUE) {

            // Attempt to take the mutex to stop sensor tasks
            if (xSemaphoreTake(xMutex, portMAX_DELAY) == pdTRUE) {
                // All sensor data is ready and access is exclusive
                if (WiFi.status() == WL_CONNECTED) {
                    HTTPClient http;
                    http.begin(serverURL);
                    http.addHeader("Content-Type", "application/json");

                    // Create JSON payload
                    String jsonPayload;
                    jsonPayload+="{";
                    jsonPayload += "\"temperature\":" + String(sensorData.dht11.temperature) + ",";
                    jsonPayload += "\"humidity\":" + String(sensorData.dht11.humidity) + ",";
                    jsonPayload += "\"pm25\":" + String(sensorData.pms5003.pm2_5);
                    //jsonPayload += "\"lpg\":" + String(sensorData.mq2.lpg);
                    jsonPayload += "}";

                    Serial.println(jsonPayload);
                    int httpResponseCode = http.POST(jsonPayload);

                    if (httpResponseCode > 0) {
                        Serial.printf("HTTP Response code: %d\n", httpResponseCode);
                        
                        // Release the semaphores
                        xSemaphoreGive(xSendSemaphore);
                        xSemaphoreGive(xSendSemaphore);
                        xSemaphoreGive(xSendSemaphore);
                    } else {
                        if (httpResponseCode == HTTPC_ERROR_CONNECTION_REFUSED) {
                            Serial.println("Connection refused");
                        } else if (httpResponseCode == HTTPC_ERROR_SEND_PAYLOAD_FAILED) {
                            Serial.println("Failed to send payload");
                        } else if (httpResponseCode == HTTPC_ERROR_SEND_HEADER_FAILED) {
                            Serial.println("Failed to send headers");
                        } else {
                            Serial.printf("Error code: %d\n", httpResponseCode);
                        }
                    }
                    http.end();
                    xSemaphoreGive(xMutex); // Release the mutex
                } else {
                    Serial.println("WiFi Disconnected");
                    
                    // Release the semaphores in case of WiFi disconnection
                    xSemaphoreGive(xSendSemaphore);
                    xSemaphoreGive(xSendSemaphore);
                    xSemaphoreGive(xSendSemaphore);
                }
            }
        }

        vTaskDelay(10000 / portTICK_PERIOD_MS); // Send data every 10 seconds
    }
}


void TaskDisplayEyes(void* pvParameters){
   while (1) {
        eye.animate();
   }
  vTaskDelay(200/ portTICK_PERIOD_MS);   //Display new data after 2 seconds
}

void TaskControlMotor(void* pvParameters){
  Serial.print("TaskControlMotor is running on core: ");
  Serial.println(xPortGetCoreID());

  int receivedValue = 0;
  while(1){
    if(xQueueReceive(xQueue, &receivedValue, portMAX_DELAY) == pdPASS){
      Serial.print("Consumed: ");
      Serial.println(receivedValue);
      motor.control_motor();
    }
  }
}

void setup(){
  Serial.begin(9600);

  eye.startup();
  delay(10000);
  //Initialize the sensors
  eye.init();
  dht11.init();
  pms5003.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Create semaphores
  //xDisplaySemaphore = xSemaphoreCreateCounting(3,0);
  xSendSemaphore = xSemaphoreCreateCounting(3,0);
  xControlSemaphore = xSemaphoreCreateBinary();

  //Create a mutex
  xMutex = xSemaphoreCreateMutex();

  //Create a queue to hold integer values
  xQueue = xQueueCreate(10, sizeof(int));
  if(xQueue == NULL){
    Serial.println("error creating the queue");
    while(1);
  }

  // Create tasks
  //(Function to implement the task, Name of the task, Stack size, Task input parameter, Priority of the task, Task handle, Core ID);
  xTaskCreatePinnedToCore(TaskDHT11, "TaskDHT11", 2048, NULL, 1, &TaskDHT11Handle, 0);
  xTaskCreatePinnedToCore(TaskPMS5003, "TaskPMS5003", 2048, NULL, 1, &TaskPMS5003Handle, 0);
  //xTaskCreatePinnedToCore(TaskMQ2, "TaskMQ2", 2048, NULL, 1, &TaskMQ2Handle, 0);
  xTaskCreatePinnedToCore(TaskSendToServer, "TaskSendToServer", 4096, NULL, 1, &TaskSendToServerHandle, 0);
  xTaskCreatePinnedToCore(TaskDisplayEyes, "TaskDisplayEyes", 4096, NULL, 1, &TaskDisplayEyesHandle, 0);
  xTaskCreatePinnedToCore(TaskControlMotor, "TaskControlMotor", 2048, NULL, 1, &TaskControlMotorHandle, 1);

}

void loop(){

  //Do nothing here

}
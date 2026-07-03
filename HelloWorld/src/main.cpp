#include <Arduino.h>


#define LCD_TASK_STACK_SIZE 1056

typedef struct {
    char  *data;
    size_t length;
} ReceivedData;
volatile ReceivedData SharedData;

SemaphoreHandle_t mysemaphore = NULL;

void taskSend(void * parameter)
{
    int i=0;
  while(true)
  {
   // Serial.print("Sending: ");
  //  Serial.println(i);
  //  i++;
    xSemaphoreTake(mysemaphore, portMAX_DELAY);
    if(SharedData.length > 0)
    {
       // Serial.print("Sending: ");
        for(int j=0;j<SharedData.length;j++)
        {
            Serial.print((char)SharedData.data[j]);
        }
        vPortFree(SharedData.data);
  }
  }
    
}



void  taskReceive(void *)
{
    while (true)
    {
     if(Serial.available() > 0) {
        String receivedData = Serial.readStringUntil('\n');
       // Serial.print("Received: ");
       // Serial.println(receivedData);
       // Serial.print("sizeof(receivedData): ");
       // Serial.println(receivedData.length());

        char * receivedDataArray =(char * ) pvPortMalloc(receivedData.length());
        for(int i=0;i<receivedData.length();i++)
        {
            receivedDataArray[i] = receivedData[i];
        }
        SharedData.data = receivedDataArray;
        SharedData.length = receivedData.length();
        xSemaphoreGive(mysemaphore);

      }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
}

void setup() {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
     Serial.println("Setup end ");


      BaseType_t lcdTaskCreated = xTaskCreate(
      taskReceive,
      "taskReceive",
      LCD_TASK_STACK_SIZE,
      nullptr,
      1,
      nullptr);
      BaseType_t lcdTaskCreated2 = xTaskCreate(
      taskSend,
      "taskSend",
      LCD_TASK_STACK_SIZE,
      nullptr,
      1,
      nullptr);

      mysemaphore = xSemaphoreCreateBinary();


}

void loop()
{

     vTaskDelay(pdMS_TO_TICKS(1000));
}

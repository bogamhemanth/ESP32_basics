#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <rotary_encoder.h>

// set the LCD number of columns and rows
constexpr uint8_t LCD_ADDRESS = 0x27;
constexpr uint8_t LCD_COLUMNS = 16;
constexpr uint8_t LCD_ROWS = 2;
constexpr uint32_t LED_TASK_STACK_SIZE = 2048;
constexpr uint32_t LCD_TASK_STACK_SIZE = 4096;
TimerHandle_t timer10msHandle;

// set LCD address, number of columns and rows
// if you don't know your display address, run an I2C scanner sketch
LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void ledBlinkTask(void *parameter);
void LCDTask(void *parameter);
void timmer10MsTask(TimerHandle_t xTimer);
void scanI2cBus();

void setuptemp() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  Wire.begin();
  //scanI2cBus();

  // initialize LCD
  lcd.init();
  // turn on LCD backlight                      
  lcd.backlight();
  #if 0
  BaseType_t ledTaskCreated = xTaskCreate(
      ledBlinkTask,
      "LED Blink Task",
      LED_TASK_STACK_SIZE,
      nullptr,
      1,
      nullptr);

  if (ledTaskCreated != pdPASS) {
    Serial.println("Failed to create LED task");
  }
#endif

  BaseType_t lcdTaskCreated = xTaskCreate(
      LCDTask,
      "LCD Task",
      LCD_TASK_STACK_SIZE,
      nullptr,
      1,
      nullptr);

  if (lcdTaskCreated != pdPASS) {
    Serial.println("Failed to create LCD task");
  }

  timer10msHandle = xTimerCreate(
    "10ms Timer",
    pdMS_TO_TICKS(10),// testing with 500ms to see the output
    pdTRUE,
    nullptr,
    timmer10MsTask);


    if (timer10msHandle != nullptr) {
        xTimerStart(timer10msHandle, 0);
    } else {
        Serial.println("Failed to create timer");
    }
    init10msHardwareTimer();
}

void loop_1() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}

void scanI2cBus() {
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");
  nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
      nDevices++;
    }
    else if (error==4) {
      Serial.print("Unknow error at address 0x");
      if (address<16) {
        Serial.print("0");
      }
      Serial.println(address,HEX);
    }    
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  }
  else {
    Serial.println("done\n");
  }
}

void ledBlinkTask(void *parameter) {
  (void)parameter;

  while (true) {
    #if 1
    digitalWrite(LED_BUILTIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(1000));

    digitalWrite(LED_BUILTIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(1000));

    Serial.println("LED blinked");
    #endif
    vTaskDelay(pdMS_TO_TICKS(10000));
  }
}

void LCDTask(void *parameter) {
  (void)parameter;

  while (true) {
    // set cursor to first column, first row
    lcd.setCursor(0, 0);
    lcd.print("Hello, World!");
    vTaskDelay(pdMS_TO_TICKS(1000));

    lcd.setCursor(0, 1);
    lcd.print("Hello, World!");
    vTaskDelay(pdMS_TO_TICKS(1000));

    lcd.clear(); 
    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}
void timmer10MsTask(TimerHandle_t xTimer) {
  uint32_t ulCount;
  ulCount = (uint32_t)pvTimerGetTimerID(xTimer);
  ulCount++;
  if(ulCount > 100) {
     // Do something every 10ms
    Serial.print("timer10msTask called");
    Serial.print("Count: ");
    Serial.println(ulCount);
    ulCount = 0;
  }
   vTimerSetTimerID(xTimer, (void *)ulCount);
  
}

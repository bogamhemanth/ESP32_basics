#include"rotary_encoder.h"

#define LED 21
hw_timer_t *My_timer = NULL;

bool init10msHardwareTimer() {
    // Initialize the hardware timer for 10ms intervals here
    My_timer = timerBegin(0, 80, true);
    timerAttachInterrupt(My_timer, &timer10msCallback, true);
    timerAlarmWrite(My_timer, 1000000, true);
    timerAlarmEnable(My_timer);
  // For example, configure the timer registers and set up interrupts if needed
  return true; // Return true if initialization is successful
}

// 10ms timer callback function
void timer10msCallback() {
  // This function will be called every 10ms
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle the built-in LED

}





bool rotaryencoderinit() {
  // Initialize the rotary encoder pins and state here
  // For example, set pin modes and attach interrupts if needed
  return true; // Return true if initialization is successful
}
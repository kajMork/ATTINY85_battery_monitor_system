#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define READ_PIN A1
#define LED_PIN 0
//https://docs.platformio.org/en/latest/platforms/atmelavr.html#upload-using-programmer
// WDT Interrupt Need some code for coming out of sleep.
// but is does not need to do anything! (just exist).
ISR(WDT_vect) {
}

// Enters the arduino into sleep mode.
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  sleep_mode(); // Start sleep mode

  // Hang here until WDT timeout

  sleep_disable();
  power_all_enable();
}

// Setup the Watch Dog Timer (WDT)
void setupWDT() {

   MCUSR &= ~(1<<WDRF); // Clear the WDRF (Reset Flag).

   // Setting WDCE allows updates for 4 clock cycles end is needed to
   // change WDE or the watchdog pre-scalers.
   WDTCR |= (1<<WDCE) | (1<<WDE);

   //0.5s Good for cat light 
   WDTCR  = (0<<WDP3) | (1<<WDP2) | (0<<WDP0) | (1<<WDP0);
 // 16ms Good for POV light.
     //32ms, 64ms no good flicker using 1ms on time
//   WDTCR  = (0<<WDP3) | (0<<WDP2) | (0<<WDP1) | (0<<WDP0);

   WDTCR |= _BV(WDIE); // Enable the WDT interrupt.
}

void setup() {
  //pinMode(1, INPUT_PULLUP);
  //pinMode(3, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT); // 4
  //pinMode(5, INPUT_PULLUP);
  //pinMode(6, INPUT_PULLUP);

  //ADCSRA &= ~(1<<ADEN); // Disable ADC
  setupWDT();
}


void _loop() {
  // Re-enter sleep mode.
  enterSleep();
}

void loop() {

   // Toggle the LED on
   int sensorValue = analogRead(READ_PIN);
   if (sensorValue > 1000){
     digitalWrite(LED_PIN, 1);
  
     delay(500);
  
     // Toggle the LED off
     digitalWrite(LED_PIN, 0);
   }

   enterSleep();
}
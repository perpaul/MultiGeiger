// code related to the geiger-mueller tube hw interface
// - high voltage generation
// - GM pulse counting

#include <Arduino.h>

#define PIN_TEST_OUTPUT 13
#define PIN_HV_FET_OUTPUT 23
#define PIN_HV_CAP_FULL_INPUT 22  // !! has to be capable of "interrupt on change"
#define PIN_GMC_COUNT_INPUT 2     // !! has to be capable of "interrupt on change"

// Dead Time of the Geiger Counter. [usec]
// Has to be longer than the complete pulse generated on the Pin PIN_GMC_COUNT_INPUT.
#define GMC_DEAD_TIME 190

volatile bool isr_GMC_cap_full;
volatile bool isr_gotGMCpulse;
volatile unsigned int isr_GMC_counts;
volatile unsigned int isr_GMC_counts_2send;
volatile unsigned long isr_count_timestamp;
volatile unsigned long isr_count_timestamp_2send;
volatile unsigned long isr_count_time_between;

unsigned long time2hvpulse;  // initialized via setup_tube -> gen_charge_pulses

// MUX (mutexes used for mutual exclusive access to isr variables)
portMUX_TYPE mux_cap_full = portMUX_INITIALIZER_UNLOCKED;
portMUX_TYPE mux_GMC_count = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR isr_GMC_capacitor_full() {
  portENTER_CRITICAL_ISR(&mux_cap_full);
  isr_GMC_cap_full = 1;
  portEXIT_CRITICAL_ISR(&mux_cap_full);
}

void IRAM_ATTR isr_GMC_count() {
  portENTER_CRITICAL_ISR(&mux_GMC_count);
  static unsigned long isr_count_timestamp_us;
  static unsigned long isr_count_timestamp_us_prev;
  static unsigned long isr_count_timestamp_us_prev_used;
  digitalWrite(PIN_TEST_OUTPUT, HIGH);
  isr_count_timestamp_us_prev = isr_count_timestamp_us;
  isr_count_timestamp_us = micros();
  if ((isr_count_timestamp_us - isr_count_timestamp_us_prev) > GMC_DEAD_TIME) {
    // the rest is only executed if GMC_DEAD_TIME is exceeded.
    // Reason: pulses occurring short after another pulse are false pulses generated by the rising edge on the PIN_GMC_COUNT_INPUT.
    // This happens because we don't have an Schmitt-Trigger on this controller pin.
    isr_GMC_counts++;                                             // count the pulse
    isr_count_timestamp = isr_count_timestamp_2send = millis();   // notice (system) time of the pulse
    isr_GMC_counts_2send++;
    isr_gotGMCpulse = 1;

    isr_count_time_between = isr_count_timestamp_us - isr_count_timestamp_us_prev_used;  // save for statistics debuging
    isr_count_timestamp_us_prev_used = isr_count_timestamp_us;
  }
  digitalWrite(PIN_TEST_OUTPUT, LOW);
  portEXIT_CRITICAL_ISR(&mux_GMC_count);
}


// How many HV capacitor charge pulses to generate before giving up.
// (MAX_CHARGE_PULSES * pulse_duration) should be less than the interval in
// the main loop (currently 1000ms) where it unconditionally recharges.
#define MAX_CHARGE_PULSES 333

// How many HV capacitor charge pulses to generate for first charge,
// for the call in setup(). Not timing critical and capacitor is empty.
#define MAX_CHARGE_PULSES_INITIAL 3333

int gen_charge_pulses(bool setup) {
  int max_charge_pulses = setup ? MAX_CHARGE_PULSES_INITIAL : MAX_CHARGE_PULSES;
  int charge_pulses = 0;
  isr_GMC_cap_full = 0;
  do {
    digitalWrite(PIN_HV_FET_OUTPUT, HIGH);              // turn the HV FET on
    delayMicroseconds(1500);                            // 5000 usec gives 1,3 times more charge, 500 usec gives 1/20 th of charge
    digitalWrite(PIN_HV_FET_OUTPUT, LOW);               // turn the HV FET off
    delayMicroseconds(1000);
    charge_pulses++;
  } while ((charge_pulses < max_charge_pulses) && !isr_GMC_cap_full); // either a timeout or a capacitor full interrupt stops this loop
  time2hvpulse = millis();                              // we just pulsed, so restart timer
  if ((charge_pulses == max_charge_pulses) && !isr_GMC_cap_full)
    return -1; // TODO log(CRITICAL, "HV charging failed!");               // pulsed a lot, but still the capacitor is not at desired voltage
  return charge_pulses;
}

void setup_tube() {
  pinMode(PIN_TEST_OUTPUT, OUTPUT);
  pinMode(PIN_HV_FET_OUTPUT, OUTPUT);
  pinMode(PIN_HV_CAP_FULL_INPUT, INPUT);
  pinMode(PIN_GMC_COUNT_INPUT, INPUT);

  digitalWrite(PIN_TEST_OUTPUT, LOW);
  digitalWrite(PIN_HV_FET_OUTPUT, LOW);

  unsigned long now_ms = millis();
  unsigned long now_us = micros();

  // note: we do not need to get the portMUX here as we did not yet enable interrupts.
  isr_count_timestamp = now_ms;
  isr_count_time_between = now_us;
  isr_count_timestamp_2send = now_us;
  isr_GMC_cap_full = 0;
  isr_GMC_counts = 0;
  isr_GMC_counts_2send = 0;
  isr_gotGMCpulse = 0;

  attachInterrupt(digitalPinToInterrupt(PIN_HV_CAP_FULL_INPUT), isr_GMC_capacitor_full, RISING);  // capacitor full
  attachInterrupt(digitalPinToInterrupt(PIN_GMC_COUNT_INPUT), isr_GMC_count, FALLING);            // GMC pulse detected

  gen_charge_pulses(true);
}

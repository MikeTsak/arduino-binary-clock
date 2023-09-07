// Define pins
const int hourPins[] = {2, 3, 4, 5};      // pins for hours in binary
const int minutePins[] = {6, 7, 8, 9, 10, 11};  // pins for minutes in binary
const int secondPins[] = {A0, A1, A2, A3, A4, A5};  // pins for seconds in binary

const int onboardLED = 13;  // onboard LED pin

int hours = 0;
int minutes = 0;
int seconds = 0;

void setup() {
  // Initialize pins as outputs
  for (int pin : hourPins) {
    pinMode(pin, OUTPUT);
  }
  for (int pin : minutePins) {
    pinMode(pin, OUTPUT);
  }
  for (int pin : secondPins) {
    pinMode(pin, OUTPUT);
  }

  pinMode(onboardLED, OUTPUT); // set onboard LED as output

  // Start an interrupt for counting seconds
  cli();  // stop interrupts
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1  = 0;

  OCR1A = 15624;  // compare match register, 1 Hz
  TCCR1B |= (1 << WGM12);  // CTC mode
  TCCR1B |= (1 << CS10) | (1 << CS12);  // 1024 prescaler
  TIMSK1 |= (1 << OCIE1A);  // enable timer compare interrupt
  sei();  // allow interrupts
}

void loop() {
  // Main loop is empty, as everything is handled by the interrupt
}

void displayBinary(int number, const int pins[], int numPins) {
  for (int i = 0; i < numPins; i++) {
    digitalWrite(pins[i], (number >> i) & 1);
  }
}

ISR(TIMER1_COMPA_vect) {
  seconds++;
  digitalWrite(onboardLED, !digitalRead(onboardLED)); // Toggle onboard LED state

  if (seconds >= 60) {
    seconds = 0;
    minutes++;
    if (minutes >= 60) {
      minutes = 0;
      hours++;
      if (hours >= 12) {
        hours = 0;
      }
    }
  }

  displayBinary(hours, hourPins, sizeof(hourPins) / sizeof(int));
  displayBinary(minutes, minutePins, sizeof(minutePins) / sizeof(int));
  displayBinary(seconds, secondPins, sizeof(secondPins) / sizeof(int));
}

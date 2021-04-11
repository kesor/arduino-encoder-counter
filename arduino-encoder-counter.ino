#include <Arduino.h>
#include <avr/interrupt.h>

#define PIN_ENC_1 4
#define PIN_ENC_2 5
#define PIN_ENC_B 6

/* ===================================================== */
volatile uint8_t dport_pins = B01110000;
volatile uint8_t enc_state = 0; // 8 bits used for state machine

volatile uint16_t num = 2000;
volatile uint8_t funcSelect = B01;

// Pre-defines states of the 4-state Finite State Machine
// checked as a sequence when encoder returns to 0b11 "normal"
#define ENC_CW  0b01001011
#define ENC_CCW 0b10000111

// Pin Change Interrupt handler for PORTD
ISR(PCINT2_vect) {
  cli(); // disallow interrupts

  uint8_t last_pins = dport_pins;

  // Store PIND in a volatile global variable to inspect next time
  dport_pins = PIND;

  // Has any encoder pin/button changed since last time
  bool enc_1_pc = (bitRead(dport_pins, PIN_ENC_1) != bitRead(last_pins, PIN_ENC_1));
  bool enc_2_pc = (bitRead(dport_pins, PIN_ENC_2) != bitRead(last_pins, PIN_ENC_2));
  bool enc_b_pc = (bitRead(dport_pins, PIN_ENC_B) != bitRead(last_pins, PIN_ENC_B));

  if (enc_b_pc && bitRead(dport_pins, PIN_ENC_B)) {
    funcSelect ^= B11;
  }

  if (enc_1_pc || enc_2_pc) {
    enc_state = enc_state << 2; // shift LIFO queue by 2 bits
    enc_state &= 0b11111100;
    enc_state |= (bitRead(dport_pins, PIN_ENC_1) << 1) + bitRead(dport_pins, PIN_ENC_2);
    if (enc_state == ENC_CW)
      num++;
    else if (enc_state == ENC_CCW)
      num--;
  }

  sei(); // allow interrupts
}
/* ===================================================== */

void printPortD(void) {
  char binary[1 + 8 * sizeof(uint8_t)];
  char stringify[1 + 8 * sizeof(uint8_t)];
  sprintf(stringify, "00000000"); // add left zero-padding to stingified binary
  // add the binary number string after the zero-pad, not the first-character
  sprintf(stringify + (8 - strlen(binary)), "%s", utoa(dport_pins, binary, 2));
  Serial.println(stringify);
}

void setup(void) {
  cli(); // disable interrupts

  // set internal pin pull-up resistors to high
  pinMode(PIN_ENC_1, INPUT_PULLUP);
  pinMode(PIN_ENC_2, INPUT_PULLUP);
  pinMode(PIN_ENC_B, INPUT_PULLUP);
  digitalWrite(PIN_ENC_1, HIGH);
  digitalWrite(PIN_ENC_2, HIGH);
  digitalWrite(PIN_ENC_B, HIGH);

  // Disable External Interrupt
  EIMSK  =  0;

  // Enable Pin Change Interrupt on Port D
  PCICR  |= 0b00000100; // PCICR – Pin Change Interrupt Control Register
  PCIFR  |= 0b00000100; // PCIFR – Pin Change Interrupt Flag Register
  // PCINT23 PCINT22 PCINT21 PCINT20 PCINT19 PCINT18 PCINT17 PCINT16
  //     PD7     PD6     PD5     PD4     PD3     PD2     PD1     PD0
  //       7       6       5       4   EI1 3   EI0 2    TX 1    RX 0
  PCMSK2 |= 0b01110000; // PCMSK2 – Pin Change Mask Register 2

  sei(); // enable interrupts

  // activate internal pullups for i2c.
  pinMode(SDA, INPUT_PULLUP);
  pinMode(SCL, INPUT_PULLUP);
  digitalWrite(SDA, HIGH);
  digitalWrite(SCL, HIGH);

  Serial.begin(115200);
  delay(20);
  Serial.println("Encoder Decoder");
}

void loop(void) {
  static uint16_t last_num = num;
  static uint8_t last_func = funcSelect;

  Serial.print("Number: ");
  Serial.println(num, DEC);
  Serial.print("Function: ");


  switch (funcSelect) {
  case B01:
    Serial.println("F_A");
    break;
  case B10:
    Serial.println("F_B");
    break;
  default:
    Serial.println("UNKNOWN!");
    break;
  }

  Serial.print("Latest state of digital pins: ");
  printPortD();

  Serial.println("---");

  Serial.flush();

  for (; num == last_num && last_func == funcSelect ;) {
    // wait for something to change ...
    delayMicroseconds(1);
  }

  last_num = num;
  last_func = funcSelect;
}

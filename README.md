# Example: Arduino Encoder Counter

Example code that demonstrates how to decode a [Quadrature Rotary Encoder](
https://en.wikipedia.org/wiki/Rotary_encoder) using Arduino interrupts.

Includes several unique features:

## Using ATmega Pin-Change interrupt instead of the external interrupt

Using the Pin Change Interrupt allows connecting encoders and buttons to any
of the Arduino pins, not just the two External Interrupt enabled pins. While
maintaining the ability of immediate reaction to any encoder rotation changes.

In the example code the Pin Change Interrupt used catches any changes on the
ATmega PORTD pins. Should you need to use different pins, the `PCINT*_vect` and
the interrupt setup need to be changed with the appropriate port settings.

## 8-bit state machine for discrete increment/decrement of an integer number

The four states of the Quadarture Rotary Encoder are stored in a LIFO queue
that acts as the history of a state machine. The queue itself is an 8-bit
integer that can be compared with the CW or the CWW sequence of events to match
a clocked change by the encoder.

## Two-State push button with variable

It is common for rotaty encoders to also incorporate a push-button; The code
includes an example of how this button can be used to switch states using an
2-bit function selector.

## Bit plot of the PORTD pins

The code includes an example on how the 8-bits can be printed in binary
notation with left zero padding. This can be used during debug to show the
state of the pins to the serial console or any connected display device.

## Serial output of the inc/dec number and function selector state

Example of how the `loop()` function of the Arduino can be used to wait for
changes and be "normally sleeping" until something is changed with the encoder
number and/or state selected by the push button.

In situations where the Arduino `loop()` is being used to change state of
external components such as LEDs and/or OLED displays, having to only send
changes when these have happened reduces wire noise and power usage.



### Authors

* Evgeny Zislis [kesor](https://github.com/kesor)

### License

* This project is licensed under the BSD License - see the [LICENSE](LICENSE)
  file for details.

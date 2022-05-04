#include "mrf24j.h"
#include <SPI.h>
#include <SoftPWM.h>
#include <SoftPWM_timer.h>
/*
 * uiGC0
 * Goofy Controller 0
 * common cathode LEDs +
 * Mrf24j40
 *
 * Benjamin Jeffery
 * University of Idaho
 * 10/09/2015
 *
 * Edited By Hayden Carroll
 * University of Idaho
 * 5/2/2022
 *
 */

int redPin = 3;   // red pin location
int greenPin = 4; // green pin location
int bluePin = 5;  // blue pin location
int pin_reset = 6;
int pin_cs = 8;
int dip8_pin = 10;
int dip7_pin = 9;
int pin_interrupt = 2;
unsigned int dipSwitch = 0; // value of the dipswitch for the board
int startId; // value of where in the packet to read from (currently should
             // equal dipswitch, unless we add header info, we would offset it)

Mrf24j mrf(pin_reset, pin_cs, pin_interrupt);

void setup() {
    DDRC = 0x00;

    pinMode(redPin, OUTPUT);   // sets the pin as output
    pinMode(greenPin, OUTPUT); // sets the pin as output
    pinMode(bluePin, OUTPUT);  // sets the pin as output

    //    SoftPWMBegin();
    //    SoftPWMSet(redPin, 0);
    //  SoftPWMSet(greenPin, 0);
    //  SoftPWMSet(bluePin, 0);
    //    SoftPWMSetFadeTime(redPin, 100, 1000);
    //  SoftPWMSetFadeTime(greenPin, 10, 10);
    //  SoftPWMSetFadeTime(bluePin, 10, 10);

    // The following pins are connected to the rotary or DIP switch that sets
    // the channel. They are all inputs, but we write to them to enable the
    // internal pullup resistor.

    Serial.begin(38400);

    setColor(0, 0, 0);

    mrf.reset();
    mrf.init();
    mrf.set_pan(2015);
    mrf.set_channel(0x0C);
    mrf.address16_write(0x4202);
    mrf.set_promiscuous(true);
    mrf.set_bufferPHY(true);

    attachInterrupt(0, interrupt_routine, CHANGE);
    interrupts();
    pinMode(A0, INPUT_PULLUP);
    pinMode(A1, INPUT_PULLUP);
    pinMode(A2, INPUT_PULLUP);
    pinMode(A3, INPUT_PULLUP);
    pinMode(A4, INPUT_PULLUP);
    pinMode(A5, INPUT_PULLUP);
    pinMode(dip7_pin, INPUT_PULLUP);
    pinMode(dip8_pin, INPUT_PULLUP);

    dipSwitch = (digitalRead(dip8_pin))
                << 7; // get 8th dip switch bit, shift it to 8th pos
    dipSwitch =
        dipSwitch | (digitalRead(dip7_pin)
                     << 6); // get 7th dip switch bit, shift it to 7th pos
    dipSwitch = (dipSwitch | (PINC & 0x3F)); // get 1-6th dip switch bit
    dipSwitch = ~dipSwitch & 0xFF;

    startId = dipSwitch;
    Serial.println("\n The dipswitch value is: ");
    Serial.println(dipSwitch);
}

void interrupt_routine() { mrf.interrupt_handler(); }

void loop() { mrf.check_flags(&handle_rx, &handle_tx); }

void handle_rx() {
    rx_info_t *info = mrf.get_rxinfo();

    uint8_t encodedColor = info->rx_data[startId];
    uint8_t red = 0;
    uint8_t blue = 0;
    uint8_t green = 0;

    decodeColor(encodedColor, &red, &green, &blue);
    Serial.print(red);
    Serial.print(" ");
    Serial.print(green);
    Serial.print(" ");
    Serial.print(blue);
    Serial.print("\n");

    setColor(red, green, blue);

    mrf.rx_flush();
}

void handle_tx() {}

void setColor(int red, int green, int blue) {
    //    SoftPWMSet(redPin, red);
    //  SoftPWMSet(greenPin, green);
    //  SoftPWMSet(bluePin, blue);
    analogWrite(redPin, red); // reds intensity is way too high
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
}

// takes in the encoded color, decodes it,
// and changes the red, green, and blue values accordingly
void decodeColor(uint8_t encodedColor, uint8_t *red, uint8_t *green,
                 uint8_t *blue) {
    *blue = encodedColor & 0b00000011;
    encodedColor = encodedColor >> 2;
    *green = encodedColor & 0b00000111;
    encodedColor = encodedColor >> 3;
    *red = encodedColor & 0b00000111;

    *red = (*red / 7.f) * 255;
    *green = (*green / 7.f) * 255;
    *blue = (*blue / 3.f) * 255;
}

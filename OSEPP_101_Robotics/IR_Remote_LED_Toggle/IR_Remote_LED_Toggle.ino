/*
* Tutorial 2c: IR Remote LED toggle
*
* Toggles the LED on the Uno when you press
* button 5 on the IR Remote.
*
* Requires an assembled robot.
*
* created 03 Oct 2013
* by Blaise Jarrett
*
* This example code is in the public domain.
*
*/

// include Ken Shirriff's IR Remote Library
// You must install this if you havn't already
// https://github.com/shirriff/Arduino-IRremote
#include <IRremote.h>
// include our remote control codes
#include "codes.h"

// the pin the IR Receiver is connected to
const int irPin = 12;
// the pin the Uno's LED is connected to
const int ledPin = 13;

// create an IR Receiver object from the library
IRrecv irrecv(irPin);
// we'll need a decode_results variable to store
// the IR readings
decode_results results;

void setup()
{
    // We're going to print the results over serial
    // Open the serial port
    Serial.begin(9600);

    // Set up our LED
    pinMode(ledPin, OUTPUT);

    // enable the receiver
    irrecv.enableIRIn();
}

void loop()
{
    // check to see if the receiver has received
    // a command
    if (irrecv.decode(&results))
    {
        // send the command code to our function for
        // processing
        processCommand(results.value);

        // tell the library we're ready to listen for
        // another command
        irrecv.resume();
    }
}

void processCommand(unsigned long code)
{
    // use the switch case to chose what to do
    // with the command
    // http://arduino.cc/en/Reference/SwitchCase
    switch (code)
    {
        // Button 5 was pressed
    case IR_5:
        Serial.println("Toggle LED");
        // toggle the LED by reading its existing state
        // and setting it to the oposite
        digitalWrite(ledPin, !digitalRead(ledPin));
        break;
    case IR_REPEAT:
        Serial.println("Repeat");
        break;
    default:
        Serial.println("Other");
        break;
    }
}

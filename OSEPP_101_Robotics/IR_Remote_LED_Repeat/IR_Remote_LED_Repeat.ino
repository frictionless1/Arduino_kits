/*
* Tutorial 2d: IR Remote LED Repeat
*
* Shows you how to use the IR Repeat signal
* to keep an LED on while you hold down button 5.
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

// We're going to need to remember the last command
// and the time the last command was received while
// waiting for a repeat to occur
unsigned long last_time = 0;
unsigned long last_cmd = 0;

// the time a repeat must be sent by to keep the LED on (in ms)
const int repeatTimeout = 150;

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
        // yes
        // is it the Repeat command?
        if (results.value != IR_REPEAT)
        {
            // if it isn't remember the command and process it
            last_cmd = results.value;
            last_time = millis();

            processCommand(results.value);
        }
        else
        {
            // if it is reset the timeout
            last_time = millis();

            // print over serial only if its a repeat for the command to turn on the LED
            if (last_cmd == IR_5)
            {
                Serial.print("Received Repeat: Keeping LED on for another ");
                Serial.print(repeatTimeout);
                Serial.println("ms");
            }
        }

        // tell the library we're ready to listen for
        // another command
        irrecv.resume();
    }
    else
    {
        // no command has been received
        // check to see if we're waiting for a repeat and our
        // if our time has expired
        if (last_cmd != 0 && last_time + repeatTimeout < millis())
        {
            // if the command that expired was the LED command
            if (last_cmd == IR_5)
            {
                Serial.print("Repeate command hasn't been received in ");
                Serial.print(repeatTimeout);
                Serial.println("ms. Turning off LED");

                // turn off the LED
                digitalWrite(ledPin, LOW);
            }

            // the time expired, don't wait anymore
            last_cmd = 0;
        }
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
        Serial.println("Received 5: Turning on LED");
        digitalWrite(ledPin, HIGH);
        break;
    default:
        Serial.println("Other");
        break;
    }
}

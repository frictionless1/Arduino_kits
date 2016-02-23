/*
 Name:		DoorAlarm.ino
 Created:	2/9/2016 2:04:38 PM
 Author:	Sam
*/

// include our list of note pitches
#include "pitches.h"
#include <LiquidCrystal.h>

#define PIN_01      1
#define BUTTON_PIN  2
#define ECHO_PIN    3
#define TRIG_PIN    4
#define LCDD7_PIN   5
#define LCDD6_PIN   6
#define LCDD5_PIN   7
#define LCDD4_PIN   8
#define LCDE_PIN    9
#define LCDRS_PIN   10
#define SPEAKER_PIN 11
#define PIN_12      12
#define LED_PIN     13

float distance = 0;
float tolerance = 3;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(LCDRS_PIN, LCDE_PIN,
    LCDD4_PIN, LCDD5_PIN, LCDD6_PIN, LCDD7_PIN);

// the setup function runs once when you press reset or power the board
void setup() {
    // set up serial
    Serial.begin(9600);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT);

    // set the pinmode on our ultrasonic echo, and tric pins
    pinMode(ECHO_PIN, INPUT);
    pinMode(TRIG_PIN, OUTPUT);

    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);

    /* set initial disance to door */
    distance = GetRange();
}

// the loop function runs over and over again until power down or reset
void loop() {

    float range = 0;

    range = GetRange();


    if (ButtonPressed()) {
        PlayAlarm();
        distance = GetRange();
    }
    else if ((distance - range) > tolerance) {
        PlayAlarm();
    }

    DoBlink();

    UpdateDisplay(range);

    delay(500);
}

    void
DoBlink()
{
    static bool LedState = LOW;

    LedState = (LedState == LOW) ? HIGH : LOW;  // Flip LED state

    Serial.println(LedState);

    digitalWrite(LED_PIN, LedState);            // Set LED to current state
}

    bool
ButtonPressed()
{
    int state = digitalRead(BUTTON_PIN);

    Serial.print("Button = ");
    Serial.println(state);

    return (state == LOW) ? false : true;
}

    float
GetRange()
{
    float distanceCentimeters;
    int pulseLenMicroseconds;

    // bit-bang a small square wave
    // on the trig pin to start the range finder
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(20);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(TRIG_PIN, LOW);

    // measure the pulse length from the echo pin
    pulseLenMicroseconds = pulseIn(ECHO_PIN, HIGH);

    // calculate the distance using the speed of sound
    distanceCentimeters = pulseLenMicroseconds / 29.387 / 2;

    // print it out over serial
    Serial.print("Distance (cm): ");
    Serial.println(distanceCentimeters);

    return distanceCentimeters;
}

    void
UpdateDisplay(float range)
{
    // set the cursor to column 0, line 15
    // (note: line 1 is the second row, since counting begins with 0)
    // print the distance on the first row
    lcd.setCursor(0, 0);
    lcd.print("Dist:(");
    lcd.print(int(distance));
    lcd.print(")");
    lcd.print(range);
    //Serial.print("LCD Distance: ");
    //Serial.println(int(distance));

    // print the number of seconds since reset
    lcd.setCursor(0, 1);
    lcd.print("Seconds: ");
    lcd.print(millis() / 1000);
    //Serial.print("LCD Secs: ");
    //Serial.println(millis() / 1000);

}

// the notes in our melody and their duration in fractions of a second
// e.g. quarter note = 4, eighth note = 8, etc.
const int alarm[][2] =
{
    { NOTE_D7, 4 },
    { NOTE_A6, 4 },
    { NOTE_D7, 4 },
    { NOTE_A6, 4 },
    { NOTE_BLANK, 4 }
};

//const int melody[][2] =
//{
//    { NOTE_C4, 4 },
//    { NOTE_G3, 8 },
//    { NOTE_G3, 8 },
//    { NOTE_A3, 4 },
//    { NOTE_G3, 4 },
//    { NOTE_BLANK, 4 },
//    { NOTE_B3, 4 },
//    { NOTE_C4, 4 }
//};

void PlayAlarm()
{
    // figure out the number of notes in our melody
    int numberOfNotes = sizeof(alarm) / sizeof(alarm[0]);

    // iterate over the notes of the melody
    for (int thisNote = 0; thisNote < numberOfNotes; thisNote++)
    {
        // grab our note and note duration from our array
        int thisNoteTone = alarm[thisNote][0];
        int thisNoteDuration = alarm[thisNote][1];

        // to calculate the note duration in ms
        int noteDurationMS = 1000 / thisNoteDuration;

        // play the note
        tone(SPEAKER_PIN, thisNoteTone, noteDurationMS);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        delay(noteDurationMS * 1.30);
    }
}


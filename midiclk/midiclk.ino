/*
  MIDI note player

  This sketch shows how to use the serial transmit pin (pin 1) to send MIDI note data.
  If this circuit is connected to a MIDI synth, it will play the notes
  F#-0 (0x1E) to F#-5 (0x5A) in sequence.

  The circuit:
  - digital in 1 connected to MIDI jack pin 5
  - MIDI jack pin 2 connected to ground
  - MIDI jack pin 4 connected to +5V through 220 ohm resistor
  - Attach a MIDI cable to the jack, then to a MIDI synth, and play music.

  created 13 Jun 2006
  modified 13 Aug 2012
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Midi
*/

#define MIDI_SYSRT_CLK 0xF8

#define PULSE_PIN 2



void setup() {
  // Set MIDI baud rate:
  Serial.begin(31250);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PULSE_PIN, INPUT);

}

void loop() {

  do_without_delay();
  getPulse();
  
}

int pulseLastState = 0;

unsigned long pulsePeriodMicros = 0;
unsigned long pulseEdgeLastMicros = 0;


unsigned long midiPeriodMicros = 2000000/24;
unsigned long midiEdgeLastMicros = 0;

const int midiDivider = 24;           // interval at which to blink (milliseconds)

//int midiLeftThisPulse = midiDivider;


void getPulse(){

  int pulseState = digitalRead(PULSE_PIN);
  //pulseState = LOW; //debug only
  digitalWrite(LED_BUILTIN, pulseState);   // turn the LED on (HIGH is the voltage level)
  if (pulseState == HIGH and pulseLastState == LOW) {
    unsigned long currentMicros = micros();
    pulsePeriodMicros = currentMicros - pulseEdgeLastMicros;
    pulseEdgeLastMicros = currentMicros;    
    midiPeriodMicros = pulsePeriodMicros / midiDivider;
    do_now();
  }  

  pulseLastState = pulseState;
  
}






void do_now (){
  
  unsigned long currentMicros = micros();
  //if (currentMicros - midiEdgeLastMicros >= midiPeriodMicros) {
    // save the last time you blinked the LED
    midiEdgeLastMicros = currentMicros;
    //do something here
    Serial.write(MIDI_SYSRT_CLK);
    //Serial.println("currentMicros");
    //Serial.println("MIDI OUT NOW !!!!!!!!!!!!!!!");

    //Serial.println(currentMicros);
  //}

}


void do_without_delay (){
  
  unsigned long currentMicros = micros();
  if (currentMicros - midiEdgeLastMicros >= midiPeriodMicros) {
    // save the last time you blinked the LED
    midiEdgeLastMicros = currentMicros;
    //do something here
    Serial.write(MIDI_SYSRT_CLK);
    //Serial.println("currentMicros");
    //Serial.println("MIDI OUT NOW !!!!!!!!!!!!!!!");

    //Serial.println(currentMicros);
  }

}

/*
Written by Clark using various sources of inspiration.  Use at your own risk.

Pulse in, detect rising edge
Midi clock out with 24 times faster period per midi spec 
  
  
  Circuit:
  
    Pulse interface (TS 3.5mm)
        Arduino pin 2      <- Pulse TIP
        Arduino GND         <- Pulse Sleeve
        
    MIDI interface (TRS 3.5mm)
        Arduino Tx pin 1-> 220ohm resistor -> MIDI TIP
        Arduino 5v        -> 220ohm resistor -> MIDI Ring
        Arduino GND         -> MIDI Sleeve
    
    I then used a TRS to MIDI cable to get the midi to my synth!
    
  
*/

#define MIDI_SYSRT_CLK 0xF8

#define PULSE_PIN 2



void setup() {
    // Set MIDI baud rate:
    Serial.begin(31250);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PULSE_PIN, INPUT);

}

void loop() {

    do_without_delay();
    getPulse();
    
}

int pulseLastState = LOW;

unsigned long pulsePeriodMicros = 0;
unsigned long pulseEdgeLastMicros = 0;
//const unsigned long pulseDebounceMicros = 10;

unsigned long midiPeriodMicros = 2000000/24;
unsigned long midiEdgeLastMicros = 0;

const int midiDivider = 24;           // interval at which to blink (milliseconds)

int midiLeftThisPulse = midiDivider;


void getPulse(){

    int pulseState = digitalRead(PULSE_PIN);
    //pulseState = LOW; //debug only
    digitalWrite(LED_BUILTIN, pulseState);     // turn the LED on (HIGH is the voltage level)
    if (pulseState == HIGH and pulseLastState == LOW) {
        unsigned long currentMicros = micros();
        pulsePeriodMicros = currentMicros - pulseEdgeLastMicros;
        pulseEdgeLastMicros = currentMicros;        
        midiPeriodMicros = pulsePeriodMicros / midiDivider;
        do_remianing();
        do_now(currentMicros);
    }    

    pulseLastState = pulseState;
    
}


//hopefully we never get here
void do_remianing(){
    for (; midiLeftThisPulse>0; midiLeftThisPulse --){
         Serial.write(MIDI_SYSRT_CLK);
    } 
}



void do_now(unsigned long currentMicros){
    Serial.write(MIDI_SYSRT_CLK);
    
    // save the last time you blinked the LED
    midiEdgeLastMicros = currentMicros;
  
    midiLeftThisPulse = midiDivider-1;
  

}

//meant to be polled contantly, 
//send clk bytes in a steady stream based on midi period untill remiaining bytes = 0
void do_without_delay (){
    
    unsigned long currentMicros = micros();
    if (midiLeftThisPulse >0 && currentMicros - midiEdgeLastMicros >= midiPeriodMicros) {
        Serial.write(MIDI_SYSRT_CLK);    
    
        midiEdgeLastMicros = currentMicros;
    
        midiLeftThisPulse = midiLeftThisPulse -1;
    }
}

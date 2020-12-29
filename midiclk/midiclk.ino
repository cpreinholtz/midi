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

//#define TESTM

void setup() {
    // Set MIDI baud rate:

#ifdef TESTM
        Serial.begin(115200);
#else
      Serial.begin(31250);

#endif


    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PULSE_PIN, INPUT);

}

void loop() {

    do_without_delay();
    getPulse();
    
}

bool freeRun = true;

int pulseLastState = LOW;

unsigned long pulsePeriodMicros = 1000000;  //default to 120 bpm ( half second)
unsigned long pulseEdgeLastMicros = 0;
//const unsigned long pulseDebounceMicros = 10;

unsigned long midiPeriodMicros = pulsePeriodMicros/24; //default to 120 bpm ( half second)
unsigned long midiEdgeLastMicros = 0;

//const int midiDivider = 24;           // interval at which to blink (milliseconds)
const int midiDivider = 24/4;           // interval at which to blink (milliseconds)

//int midiLeftThisPulse = midiDivider-1;// this will send 23 clks, then the 24th will occour @ every pulse edge
int midiSentThisPulse = 0;// this will send 23 clks, then the 24th will occour @ every pulse edge


void getPulse(){

    int pulseState = digitalRead(PULSE_PIN);
    //pulseState = LOW; //debug only
    if (not freeRun) digitalWrite(LED_BUILTIN, pulseState);     // turn the LED on (HIGH is the voltage level)
    if (pulseState == HIGH and pulseLastState == LOW) {
        //Serial.println("!!!!!!!!!!!!!!!!!pulsein!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        unsigned long currentMicros = micros();
        pulsePeriodMicros = currentMicros - pulseEdgeLastMicros;
        pulseEdgeLastMicros = currentMicros;        
        midiPeriodMicros = pulsePeriodMicros / midiDivider;
        do_remianing();  //ensure we always end on an even 24 (0 left to send )
        do_now(currentMicros);
        freeRun = false;

        
    }    

    pulseLastState = pulseState;
    
}


//
void do_remianing(){
    while (1){
        
        //if 0 or 24 break immediatly, else make sure we sent an even 24 then break
        if (midiSentThisPulse >= midiDivider || midiSentThisPulse <=0){
             break;
        }
         Serial.write(MIDI_SYSRT_CLK);
         midiSentThisPulse = midiSentThisPulse+1;
    }
    midiSentThisPulse =0;
}




//meant to be called as the last midi clk of the 24clk cycle, resets counter 
void do_now(unsigned long currentMicros){
    Serial.write(MIDI_SYSRT_CLK);

    // save the last time you blinked the LED
    midiEdgeLastMicros = currentMicros;
    
    midiSentThisPulse = midiSentThisPulse+1;

#ifdef TESTM
       Serial.println("sending midi pulse");
       Serial.println(midiSentThisPulse);
#endif



}



//meant to be polled contantly, 
//send clk bytes in a steady stream based on midi period untill remiaining bytes = 0
void do_without_delay (){
    
    unsigned long currentMicros = micros();
    unsigned long elapsedMicros = currentMicros - midiEdgeLastMicros;
    
    //free run, current counter is irrelivant
    if (freeRun ==true && elapsedMicros >= midiPeriodMicros) {
        //Serial.write(MIDI_SYSRT_CLK);    
        //midiEdgeLastMicros = currentMicros;    
        //midiSentThisPulse = midiSentThisPulse +1;
        do_now(currentMicros);
            
        if(midiSentThisPulse >= midiDivider){
            digitalWrite(LED_BUILTIN, HIGH);
            midiSentThisPulse = 0;
            //Serial.println(midiSentThisPulse);
        } else {
          digitalWrite(LED_BUILTIN, LOW);
        }
        
        
    //not free running
    // ends up at 24, then hangs till pulse
    //} else if ( midiSentThisPulse < midiDivider && elapsedMicros >= midiPeriodMicros) {
    } else if (freeRun ==false && midiSentThisPulse < midiDivider && elapsedMicros >= midiPeriodMicros) {
        //Serial.write(MIDI_SYSRT_CLK);
        //midiEdgeLastMicros = currentMicros;
        //midiSentThisPulse = midiSentThisPulse +1;
        do_now(currentMicros);
        

    // transition to free running
    //this will end up skipping a quarter note and then restart counter and start free running based on last known period
    } else if (freeRun ==false && midiSentThisPulse >=midiDivider && elapsedMicros >= (midiPeriodMicros * (midiDivider +1))) {  
      midiSentThisPulse =0;
      do_now(currentMicros);
      freeRun=true;
    }
    
}

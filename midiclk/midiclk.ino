/*
Written by Clark using various sources of inspiration.  Use at your own risk.

Pulse in, detect rising edge
Midi clock out with 24 times faster period per midi spec 
  
  
  Circuit:

    Button Interface
      Arduino 5v          -> button 1 
      arduino pin 3       <- button 2
      Arduino GND         -> button 3

    Pot Interface
      Arduino 5v          -> pot 1 
      arduino pin A5      <- pot 2
      Arduino GND         -> pot 3
      
    
    Pulse interface (TS 3.5mm)
        Arduino pin 2      <- Pulse TIP
        Arduino GND         <- Pulse Sleeve
        
    MIDI interface (TRS 3.5mm)
        Arduino Tx pin 1-> 220ohm resistor -> MIDI TIP
        Arduino 5v        -> 220ohm resistor -> MIDI Ring
        Arduino GND         -> MIDI Sleeve
    
    I then used a TRS to MIDI cable to get the midi to my synth!
    


  Behavior:
    if the button is 0, then the midi bpm is controlled with the pot

    if the button is 1, then the midi bpm is controlled by the pulse bpm
      if the pulse is lost (no change) for an extra quarternote, the midi out clk will continue at the last known bpm

  
*/

#define MIDI_SYSRT_CLK 0xF8

#define PULSE_PIN 2

#define BUTTON_PIN 3

#define POT_PIN A5

#define TESTM

void setup() {
    // Set MIDI baud rate:

#ifdef TESTM
        Serial.begin(115200);
#else
      Serial.begin(31250);

#endif


    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PULSE_PIN, INPUT_PULLUP);
    pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  
    getButton();
    getPot();
    
    do_without_delay();

    getPulse();

    //delay(100);
    
}





//lost pulsein
bool freeRunPulseLost = true;

//button = 0
bool freeRunButton = true;












/////////////////////////////////////////
//MIDI CLK
////////////////////////////////////////

//number of midi clks per pulse  std is (24 midi clks / quarternote)
const int midiDivider = 24/4;            // was 24 but that was too fast when used with the dpham

//this does not change, used if button is low
unsigned long midiPeriodMicrosDefault = 1000000/24;//default to 60 bpm  (24 midi clks / quarternote)

//this can change with pulse if button is high
unsigned long midiPeriodMicros = midiPeriodMicrosDefault; 

//keep track of midi time
unsigned long midiEdgeLastMicros = 0;

//keep track of haw many midi clks we have sent in this pulse cycle
int midiSentThisPulse = 0;// this will send 23 clks, then the 24th will occour @ every pulse edge









///////////////////////////////////////
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



///////////////////////////////////////
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


///////////////////////////////////////
//meant to be polled contantly, 
//send clk bytes in a steady stream based on midi period untill remiaining bytes = 0
void do_without_delay (){
    
    unsigned long currentMicros = micros();
    unsigned long elapsedMicros = currentMicros - midiEdgeLastMicros;
    
    //free run, current counter is irrelivant
    if ((freeRunPulseLost ==true || freeRunButton ==true)  && elapsedMicros >= midiPeriodMicros) {
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
    } else if (freeRunPulseLost ==false && freeRunButton ==false && midiSentThisPulse < midiDivider && elapsedMicros >= midiPeriodMicros) {
        //Serial.write(MIDI_SYSRT_CLK);
        //midiEdgeLastMicros = currentMicros;
        //midiSentThisPulse = midiSentThisPulse +1;
        do_now(currentMicros);
        

    // transition to free running
    //this will end up skipping a quarter note and then restart counter and start free running based on last known period
    } else if (freeRunPulseLost ==false && freeRunButton ==false && midiSentThisPulse >=midiDivider && elapsedMicros >= (midiPeriodMicros * (midiDivider +1))) {  
      midiSentThisPulse =0;
      do_now(currentMicros);
      freeRunPulseLost=true;
    }
    
}


/////////////////////////////////////////
//PULSE CLK
////////////////////////////////////////

int pulseLastState = LOW;
unsigned long pulsePeriodMicros = 1000000;  //default to 120 bpm ( half second)
unsigned long pulseEdgeLastMicros = 0;
//const unsigned long pulseDebounceMicros = 10;

void getPulse(){

    if (freeRunButton == false){
        int pulseState = digitalRead(PULSE_PIN);
#ifdef TESTM
       Serial.println("pulseState");
       Serial.println(pulseState);
#endif
        
        //pulseState = LOW; //debug only
        if (not freeRunPulseLost) digitalWrite(LED_BUILTIN, pulseState);     // turn the LED on (HIGH is the voltage level)
        if (pulseState == HIGH and pulseLastState == LOW) {
            //Serial.println("!!!!!!!!!!!!!!!!!pulsein!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
            unsigned long currentMicros = micros();
            pulsePeriodMicros = currentMicros - pulseEdgeLastMicros;
            pulseEdgeLastMicros = currentMicros;        
            midiPeriodMicros = pulsePeriodMicros / midiDivider;
            do_remianing();  //ensure we always end on an even 24 (0 left to send )
            do_now(currentMicros);
            freeRunPulseLost = false;
    
            
        }    
    
        pulseLastState = pulseState;

    }
    
}




/////////////////////////////////////////
//BUTTON
////////////////////////////////////////
int buttonValue = 0;



void getButton(){
  buttonValue = digitalRead(BUTTON_PIN);
  if (buttonValue ==LOW){
    freeRunButton = true;
  } else {
    freeRunButton = false;    
  }
  
#ifdef TESTM
       //Serial.println("buttonValue");
       //Serial.println(buttonValue);
#endif
  
}

/////////////////////////////////////////
//POT
////////////////////////////////////////
int potValue = 0;
int potDigitize = 0;

void getPot(){


  if (freeRunButton){

    potValue = analogRead(POT_PIN);
    potDigitize = map(potValue, 0, 1023, 0, 18);
    
    //60 to 180 bpm in 5 bpm steps
    midiPeriodMicros = map(potDigitize, 18, 0, 1000000/24 , 1000000/(24*3));

#ifdef TESTM
/*
       Serial.println("potValue");
       Serial.println(potValue);
       
       Serial.println("potDigitize");
       Serial.println(potDigitize);


       Serial.println("midiPeriodMicros");
       Serial.println(midiPeriodMicros);
       */
#endif
    
  } 


  
}

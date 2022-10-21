#ifndef TIMER_H
#define TIMER_H



#ifdef ARDUINO_TARGET
#else
#include <mock_arduino.h>
#endif

float bpmToFreq(float bpm){
    float bps = bpm / 60.0;
    return bps;
}


unsigned long bpmToMicros(float bpm) {
    unsigned long micros = unsigned long( 1000000 * bpmToFreq(bpm));
    return micros;
}


//This class is used to perform actions at regular intervals
// usage: 
//      set timer period with setPeriodMicros, or in constructor
//      pollAndReset in main loop
//Limitations:
//      must be polled almost constantly, if it gets behind the next loops will shift in time
class Timer{
private:
    unsigned long mPeriodMicros;
    unsigned long mStartedMicros;

public:

    Timer(unsigned long periodMicros){        
        setPeriodMicros(periodMicros);
        startTick();        
    }

    Timer() {
        setPeriodMicros(bpmToMicros(120.0));
        startTick();
    }



    void setPeriodMicros(unsigned long periodMicros){
        mPeriodMicros =  periodMicros;        
    }
    unsigned long getPeriodMicros() {
        return mPeriodMicros;
    }

    
    void startTick(){
        mStartedMicros =  micros();        
    }
    unsigned long getCurrentMicros() {
        return (micros() - mStartedMicros);
    }
    


    bool epochDone(){
        unsigned long currentMicros = micros();
        if (currentMicros - mStartedMicros >mPeriodMicros ){
            if (currentMicros - mStartedMicros >mPeriodMicros*2 ){
                Serial.println("too slow");
                Serial.println(currentMicros - mStartedMicros);
            }
            return true;
        } else {
            return false;
        }
    }

    bool pollAndReset(){
        bool isDone = epochDone();
        if (isDone){
            startTick();
        }
        return isDone;
    }

    void setPeriodToCurrentMicrosAndReset() {
        mPeriodMicros = micros() - mStartedMicros;        
        startTick();        
    }


};





#endif

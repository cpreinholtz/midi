#ifndef TIMER_H
#define TIMER_H



#ifdef ARDUINO_TARGET
#else
#include <mock_arduino.h>
#endif

float bpmToBps(float bpm){
    float bps = bpm / 60.0;
    return bps;
}


unsigned long bpmToMicros(float bpm) {
    //micros/beat =  1e6/ bps
    unsigned long micros = unsigned long( 1000000 / bpmToBps(bpm));
    return micros;
}

float microsToBps(unsigned long micros) {
    //Bps  =   1/ ( sec/micro * micros/beat)
    // = 1/ ( 1/1e6 * micros/beat)
    // = 1/ (micros/1e6) 
    // =  1e6/micros
    float bps = 1000000.0 / ((float)micros);
    return bps;
}

float microsToBpm (unsigned long micros) {
    float bpm = 60.0 * microsToBps(micros);
    return bpm;
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

    unsigned long setPeriodToCurrentMicrosAndReset() {
        mPeriodMicros = micros() - mStartedMicros;        
        startTick();  
        return mPeriodMicros;
    }


};





#endif

#pragma once

#include <timer.h>

class MidiClk {
private:

    //**************************************************************
    // members
    //**************************************************************
    Timer mInTimer;
    Timer mOutTimer;
    unsigned long mLastTickMicros;
    //**************************************************************
    // misc
    //**************************************************************


public:
    static const int kBpmDivider = 24;
    //**************************************************************
    // Consturctors
    //**************************************************************
    MidiClk() {//default constructor
        mInTimer.startTick();
        mOutTimer.startTick();
        float defaultBPM = 120.0;
        mLastTickMicros = bpmToMicros(defaultBPM) / kBpmDivider;
        mOutTimer.setPeriodMicros(mLastTickMicros);
    }

    //**************************************************************
    // actions
    //**************************************************************
    //recieved a midi clk pulse
    void tickIn() {
        mLastTickMicros = mInTimer.setPeriodToCurrentMicrosAndReset();
        mOutTimer.setPeriodMicros(mLastTickMicros);
        //mInTimer.startTick();
    }


    bool service() {
        if (mOutTimer.pollAndReset()) {
            //tell calling program its time to send a midi clk pulse
            //Serial.write(MIDI_SYSRT_CLK);
            return true;
        }
    }

    //**************************************************************
    // Setters
    //**************************************************************
    void set() {
        //todo add a way to manually set frequency (or period)
    }

    //**************************************************************
    // getters / non modifiers
    //**************************************************************
    int getBpm() const {
        return microsToBpm(mLastTickMicros);
    }
    int getMicros() const {
        return mLastTickMicros;
    }
    int getQuarterNoteBpm() const {
        return microsToBpm(mLastTickMicros) * kBpmDivider;
    }


};
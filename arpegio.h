#pragma once
//#include <chord_maker.h>
#include<timer.h>
#include<notes.h>
#include<guarded_numbers.h>
//arpegio features
//Members
//  BPM, Gate %, Mode, Div, Notes, 
// 
//Mode:
//  Up, Down, up down, random, 
// 
//


#ifdef ARDUINO_TARGET
#else
#include <cstdint>
typedef uint8_t byte;
#endif


class Arpegio {
private:
    //**************************************************************
    // members 
    //**************************************************************
    Notes  mNotes;
    Timer mNoteTimer;
    Timer mGateTimer;

    GuardedInt mGatePercent;
    GuardedFloat mBpm;
    GuardedInt mDiv;
    bool mGateStatus;
    bool mRunning;
    int mCurrentNote;

public:

    //**************************************************************
    // Consturctors
    //**************************************************************
    Arpegio() {     
        mBpm.setGuards(250.0, 5.0);
        mDiv.setGuards(64, 1);
        mGatePercent.setGuards(99, 1);

        mDiv = 1; //prevent div0
        setBpm(120.0);      
        setDiv(1);      
        setGatePercent(50);

        mNotes.clear();
        mCurrentNote = 0;
        mRunning = false;
        mGateStatus = false;
    }

    //**************************************************************
    // actions
    //**************************************************************
    void noteOn(byte note, byte vel) {
        mNotes.noteOn(note, vel);
    }

    void noteOff(byte note) {
        mNotes.noteOff(note);
        checkCurrentNote();
    }   

    void start() {
        mNoteTimer.startTick();
        mGateTimer.startTick();
        mGateStatus = false;//set false to prevent first not entirley TODO fix this
        mCurrentNote = 0;
        mRunning = true;
    }

    void stop() {
        mRunning = false;
    }


    bool service() {
        if (mRunning == false) {
            return false;
        }
        else if (mNotes.getActiveNotes() <= 0) { 
            return false; 
        }
        else if (mGateStatus == true) {
            if (mGateTimer.pollAndReset()) {
                mGateStatus = false;
                return true;
            }
        }
        else if (mGateStatus == false) {
            if (mNoteTimer.pollAndReset()) {
                updateCurrentNote();
                mGateStatus = true;
                mGateTimer.startTick();
                return true;
            }
        }
        return false;
    }




    //**************************************************************
    // setters
    //**************************************************************
    void setBpm(float bpm) {
        mBpm = bpm;
        updateNoteTimer();
    }
    void setGatePercent(int percent) {
        mGatePercent = percent;
        updateGateTimer();
    }

    void setDiv(int div) {
        mDiv = div;
        updateNoteTimer();
    }


    //**************************************************************
    // getters
    //**************************************************************
    Note get () const {
        return mNotes.get(mCurrentNote);
    }

    float getBpm () const{
        return mBpm.get();
    }

    bool getGateStatus () const{
        return mGateStatus;
    }




private:
    //**************************************************************
    // setters
    //**************************************************************
    void updateNoteTimer() {
        unsigned long micros = bpmToMicros(mBpm.get()) / mDiv.get();
        mNoteTimer.setPeriodMicros(micros);
        updateGateTimer();
    }

    void updateGateTimer() {
        unsigned long micros = mNoteTimer.getPeriodMicros() * (unsigned long) (mGatePercent.get()) / 100;
        mGateTimer.setPeriodMicros(micros);
    }

    void updateCurrentNote() {
        //for now just go in order, then loop back to 0
        mCurrentNote = mCurrentNote + 1;
        checkCurrentNote();
    }
    void checkCurrentNote() {
        if (mCurrentNote >= mNotes.getActiveNotes()) mCurrentNote = 0;
    }
};




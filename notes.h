#ifndef NOTES_H
#define NOTES_H

//#pragma once

#include <inttypes.h>

//typedef uint8_t byte;

// -----------------------------------------------------------------------------



class Note
{

    
public:

    //**************************************************************
    // members  note  / velocity
    //**************************************************************
    byte mNote;    
    byte mVel;

    
    //**************************************************************
    // setters
    //**************************************************************
    void set(byte inNote, byte inVel){
        mNote = inNote;
        mVel = inVel;
    }      
    

    //overload "=" operator using the set function defined above
    void operator=(const Note& setTo){
        set(setTo.mNote, setTo.mVel);        
    }


    void clear(){
        set (0, 0);
    }      
    
    //**************************************************************
    // Consturctors
    //**************************************************************
    Note() {
        clear();
    }

};









//as noteOn commands come in, place them at idx 0 (least significat index is latest notes played.  shift the other note to more siginificant indexes


//as noteOff commands come, loop through till you find the matching note, clear it, then resort the list


class Notes
{
    
private:

    //**************************************************************
    // members
    //**************************************************************
    static const int kListMax = 16;   
    Note mNotes[kListMax];
    int mActiveNotes;   

    const Note kDefaultEmptyNote; 

    //**************************************************************
    // misc
    //**************************************************************
    void squishNotes(int start =0){
        int found = 0;
        int i;
        //tempNotes Note[mActiveNotes];  <- really this is all you should need
        Note tempNotes[kListMax]; //just to be safe, lets make it a full size array
        
        //search through the current list for any notes with non-zero velocities 
        for (i = 0 ; i<kListMax; i++){
            if (mNotes[i].mVel > 0){         
                tempNotes[found] = mNotes[i];
                found = found+1;                
            }
        }        
        //at this point found should be = to m Active Notes, if not you have issues in the noton and noteoff handling
        //recreate original list using search results, then clear the tail
        for (i = 0 ; i<mActiveNotes; i++){
            mNotes[i] = tempNotes[i];
        }
        for (i = found ; i<kListMax; i++){
            mNotes[i].clear();
        }
    }


public:


    //**************************************************************
    // actions
    //**************************************************************
    void noteOn(byte note , byte vel){

        if (vel <=0){
            noteOff(note);
        }        
        else {
            if (mActiveNotes < kListMax){
                //shift all notes 
                int i;
                for (i=mActiveNotes; i >0 ; i--){
                    mNotes[i] = mNotes[i-1];
                }
                //place the most recent note in position 0 
                mNotes[0].set(note, vel);
                //this is VERY important to keep track of for the squish algorithm to work right
                mActiveNotes = mActiveNotes+1;
            }            
        }
    }

    

    void noteOff(byte note){
        int i;
        for (i = 0 ; i<kListMax; i++){
            if (mNotes[i].mNote == note and mNotes[i].mVel > 0 ){
                mNotes[i].clear();
                mActiveNotes = mActiveNotes-1;
            }
        }
        squishNotes();       
    }
    

    //**************************************************************
    // setters / modifiers
    //**************************************************************
    void clear(){
        int i;
        for (i = 0 ; i<kListMax; i++){
            mNotes[i].clear();
        }
        mActiveNotes = 0;
    }

    //**************************************************************
    // getters / non modifiers
    //**************************************************************
    //by default returns the 0th item in array I.E. the last note that we recieved a noteOn command for.
    //0th in array= last note
    //1st in array = second to last note    
    Note get(int i =0) const{
        if (i < 0 or i >= mActiveNotes){
            return kDefaultEmptyNote;
        } else {
            return mNotes[i];
        }
    }

    //this function allows for FIFO type operation, 
    //0 is the FIRST note we recieved, 1 is the Next etc
    Note getFifo(int i = 0) const {
        if (i < 0 or i >= mActiveNotes) {
            return kDefaultEmptyNote;
        }
        else {
            return mNotes[mActiveNotes -1 - i];
        }
    }

    int getActiveNotes() const{
        return mActiveNotes;
    }

    bool isActive(byte note) const{
        for (int i = 0; i < mActiveNotes; i++) {
            if (mNotes[i].mNote == note) return true;
        }
        return false;
    }
    

    void dbg(){
        /*
        Serial.println("noteslist ordered from last (left) to first (right)");
        int i;
        for (i = 0 ; i<kListMax; i++){
            if (mNotes[i].mVel <= 0 ) {break;}
            Serial.print(mNotes[i].mNote);
            Serial.print("\t");
        }
        Serial.println("");
        */
    }

    //**************************************************************
    // Consturctors
    //**************************************************************
    Notes(){
        clear();
    }
    
};

#endif

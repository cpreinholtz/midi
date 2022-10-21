
#ifndef CHORD_MAKER_H
#define CHORD_MAKER_H

#ifdef ARDUINO_TARGET
#else
#include <cstdint>
    typedef uint8_t byte ;
#endif

class ChordMaker{
    public:

        static const int notesPerOctave = 12;
        static const int notesPerScale = 7;
        static const byte  minorHalfSteps[notesPerScale];




        ChordMaker(){
            setKey(0);
            setRoot(0);
            setTriad();
        }


        //*******************************************************
        //SETTERS
        //*******************************************************
        void setKey(byte key){
            byte setTo = key%notesPerOctave;//ensure value is from 0 to 11
            if (mKey !=setTo){
                mKey = setTo;
                calculateOffset();
                setTriad();
            }
        }

        void setRoot(byte root){
            if (mRoot !=root){
                mRoot = root;
                calculateOffset();
                setTriad();
            }
        }



        //*******************************************************
        //GETTERS
        //*******************************************************
        byte getRoot(){
            return mRoot;
        }
        byte getKey(){
            return mKey;
        }
        int getOffset() {
            return mOffset;
        }
        bool getValid(){
            return mValid;
        }

        byte getTriadFirst(){
            //setTriad(); //handled in key and root set
            return mTriadFirst;
        }
        byte getTriadMiddle(){
            //setTriad();
            return mTriadMiddle;
        }

        byte getTriadLast(){
            //setTriad();
            return mTriadLast;
        }

    private:
        byte mKey;//key is the base key that we are playing in, for now always a minor key.  only 0 to 11 allowed        
        byte mRoot;//root is the root note of the triad, in midi byte form
        bool mValid;//check and see if the root is even part of the scale
        byte mTriadFirst;
        byte mTriadMiddle;
        byte mTriadLast;
        int mOffset;

        //offset in tems of notes IN THE SCALE, used to figure out where we are in the half step array
        void calculateOffset() {

            byte rootMod = mRoot % notesPerOctave;
            int offset = 0;
            byte note = mKey;
            while (note > rootMod) rootMod = rootMod + notesPerOctave;
            while (note < rootMod) {
                note = note + minorHalfSteps[offset];
                offset++;
            }
            mOffset = offset;

            //we must hit it EXACTLY to be valid
            if (note == rootMod){ 
                mValid = true;
            }
            else {
                mValid = false;
            }
            
        }



        void setTriad(){
            //skip if already set
            if (mValid ==false) {
                clearTriad();
            } else {
                //get the position in the scale
                //int offset = calculateOffset();  

                //triads are formed with the first third and fith note in a scale couting from the root note
                //find the triad by adding the steps for the next to notes in the scale
                int i;
                mTriadMiddle=mRoot;
                const int switchAt = 2;
                for (i = 0; i < switchAt; i++) {
                    mTriadMiddle = mTriadMiddle+ minorHalfSteps[(i+ mOffset)%notesPerScale];
                }
                mTriadLast = mTriadMiddle;
                for (i = switchAt; i < 4; i++) {
                    mTriadLast = mTriadLast+ minorHalfSteps[(i+ mOffset)%notesPerScale];
                }
            }
            
        }


        void clearTriad(){
            mTriadFirst = mKey;
            mTriadMiddle = mKey;
            mTriadLast = mKey;
        }

        
};

const byte ChordMaker::minorHalfSteps[notesPerScale] = { 2,1,2,2,1,2,2 };

//cminor is 0 3 7
//d         2 5 8
//dS        3 7 10
//

//

//expected diffs
//middle {3,3,4,3,3,4,4}
//last = middle + {4,3,3,4,4,3,3} = 
//last ={7,6,7,7,7,7,7}

// {7, 6 

#endif

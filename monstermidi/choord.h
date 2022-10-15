
#ifndef CHOORD_H
#define CHOORD_H
#ifdef ARDUINO_TARGET
#else
    typedef uint8_t byte ;
#endif

class ChordMaker{
    public:

        const int notesPerOctave = 12;
        const int notesPerScale = 7;
        const byte [notesPerScale] minorHalfSteps = [2,1,2,2,1,2,2];

        byte mKey;//key is the base key that we are playing in, for now always a minor key.  only 0 to 11 allowed        
        byte mRoot;//root is the root note of the triad, in midi byte form
        bool mValid;//check and see if the root is even part of the scale
        byte mTriadFirst;
        byte mTriadMiddle;
        byte mTriadLast;


        ChordMaker(){
            setKey(0);
            setRoot(0);
            setTriad();
        }

        void setKey(byte key){
            byte setTo = key%notesPerOctave;//ensure value is from 0 to 11
            if (mKey !=setTo){
                mKey = setTo;
                checkRoot();
                setTriad();
            }
        }

        void setRoot(byte root){
            if (mRoot !=root){
                mRoot = root;
                checkRoot();
                setTriad();
            }
        }


        void checkRoot(){
            byte rootDown = mRoot%notesPerOctave;
            mValid = false;
            int i;
            byte note = key;
            for (i=0; i<notesPerScale; i++){
                rootDown = (rootDown+minorHalfSteps[i])%notesPerOctave;
                if (rootDown == note){ 
                    mValid = true;
                    return;
            }
        }


        byte getRoot(){
            return mRoot;
        }
        byte getKey(){
            return mKey;
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
        void setTriad(){
            //skip if already set
            if (mValid ==false) {
                clearTriad();
            } else {
                //get the position in the scale
                int diff = (mRoot - mKey) %notesPerOctave;//key should always be from 0 to 11 (notes per octave)
                if (diff < 0 ) {diff = diff +notesPerOctave;} //prevent negatives

                //triads are formed with the first third and fith note in a scale couting from the root note
                //find the triad by adding the steps for the next to notes in the scale
                int i;
                mTriadMiddle=mRoot;
                for (i=0;i<3 i++){
                    mTriadMiddle = minorHalfSteps[(i+diff)%notesPerScale];
                }
                mTriad[2] = mTriad[1];
                for (i=3;i<5 i++){
                    mTriadLast = minorHalfSteps[(i+diff)%notesPerScale];
                }
            }
            
        }


        void clearTriad(){
            mTriadMiddle = mKey;
            mTriadLast = mKey;
        }

        
};



#endif

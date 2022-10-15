#include <chord_maker.h>

ChordMaker chordMaker;

void setup() {
    
}

void loop() {   
    
    for (byte key =0; key < 24; key++){
        chordMaker.setKey(key);
        for (byte root =0; root < 24; root++){
            chordMaker.setRoot(root);        
            Serial.print("the key is: ");
            Serial.println(chordMaker.getKey());        
            Serial.print("the triad root note is: ");
            Serial.println(chordMaker.getRoot());
            if (chordMaker.getValid()) {
                Serial.println("the triad root note is valid (in the key)");
                Serial.print("the triad middle note is: ");
                Serial.println(chordMaker.getTriadMiddle());        
                Serial.print("the triad last note is: ");
                Serial.println(chordMaker.getTriadLast());
            } else {
                Serial.println("the triad root note is invalid (not in the key)");
            }
            Serial.println();
            delay (500);
        }
    }    
}

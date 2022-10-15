#include <chord_maker.h>


void setup() {
    // put your setup code here, to run once:

}

void loop() {
    // put your main code here, to run repeatedly:
    
    ChordMaker chordMaker;
    byte key=0;//Cminor scale
    byte root = 0; //set C as the triad root
    
    chordMaker.setKey(key);
    chordMaker.setRoot(root);

    Serial.print("the key is: ");
    Serial.println(chordMaker.getKey());

    Serial.print("the triad root note is: ");
    Serial.println(chordMaker.getRoot());
    
    Serial.print("the triad middle note is: ");
    Serial.println(chordMaker.getMiddle());
    
    Serial.print("the triad last note is: ");
    Serial.println(chordMaker.getLast());
    
}
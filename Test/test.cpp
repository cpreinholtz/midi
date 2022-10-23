#include <cstdint>
typedef uint8_t byte;
#include <iostream>
#include <chord_maker.h>
#include <mock_arduino.h>
#include <arpegio.h>

using namespace std;


//const byte ChordMaker::minorHalfSteps[notesPerScale] = { 2,1,2,2,1,2,2 };
//example:::::
//cminor is 0 3 7
//d         2 5 8
//dSharp    3 7 10
//expected diffs in a chord 
//middle {3,3,4,3,3,4,4}
//last = middle + {4,3,3,4,4,3,3} = 
//last ={7,6,7,7,7,7,7}

const byte middleDiff[7] = { 3,3,4,3,3,4,4 };
const byte lastDiff[7] = { 7,6,7,7,7,7,7 };

int tt(byte root, byte key, ChordMaker chordMaker, int expectedOffset){
    byte triadMiddle, triadLast;
    chordMaker.setKey(key);
    chordMaker.setRoot(root);

    byte expectedMiddle = root + middleDiff[expectedOffset];
    byte expectedLast = root + lastDiff[expectedOffset];

    triadMiddle = chordMaker.getTriadMiddle();
    triadLast = chordMaker.getTriadLast();
    if (root != chordMaker.getRoot()) return 1;
    if (chordMaker.getValid() != true) return 2;
    if (chordMaker.getOffset() != expectedOffset) return 3;
    if (expectedMiddle != triadMiddle) return 4;
    if (expectedLast != triadLast) return 5;
    if (key != chordMaker.getKey()) return 6;

    cout << "triad is: " << unsigned(root) << " " << unsigned(triadMiddle) << " " << unsigned(triadLast)
        << " \tdiff is: " << 
        unsigned(triadMiddle - root) << " " << unsigned(triadLast - root) << endl;
    return 0;
}




int triad_test() {
    int  e=0;
    byte root =0;
    byte key = 0;
    byte used[7] = { 0,2,3,5,7,8,10 };
    byte unused[5] = { 1,4,6,9,11 };
    
    ChordMaker chordMaker;


    for(key = 0; key < 12; key++){
        
        //check triads
        root=key;
        e = e + tt(root,key,chordMaker,0 );
        cout<<"key is: "<<unsigned(key)<<endl;    
        for (int i =0; i<chordMaker.notesPerScale *2; i++){        
            root = root+chordMaker.minorHalfSteps[i% chordMaker.notesPerScale];
            e = e + tt(root,key, chordMaker, (i+1) % chordMaker.notesPerScale);
        }

        //check for proper silencing.
        for (int i=0; i<5; i++){
            chordMaker.setKey(key);
            chordMaker.setRoot((unused[i]+key)%12);
            if (chordMaker.getValid()) e++;
            chordMaker.setRoot((unused[i]+key)+2*12);
            if (chordMaker.getValid()) e++;
        }
        cout << "key was: " << unsigned(key) << " errors:  " << e << endl << endl;

    }

    

    //if (key!=chordMaker.getKey()) e++;
    cout << "errors:  " << e << endl;
    return e;
}

#include <midi_clk.h>
int conversionTest() {
    MidiClk clk;
    float bpm = 45.0;
    float bps = bpm / 60.0;//min to sec
    float spb = 1 / bps;//change from freq to hz
    float expectedPeriodMicros =  1e6 * (spb);//sec to micros
    unsigned long got = bpmToMicros(bpm);
    cout << expectedPeriodMicros << " <expected VS bpmToMicros program> " << got << endl;
    cout << bpm << " <expected VS microsToBpm program> " << microsToBpm(got) << endl;
    cout << bpm/60.0 << " <expected VS bpmToBps program> " << bpmToBps(bpm) << endl;


    float micros = 5e5;

    cout << 2 << " <expected VS microsToBps program> " << microsToBps(micros) << endl;

    return 0; //TODO make self checking...

}


int arpegioTest() {
    Arpegio arp;
    arp.start();

    arp.noteOn(1,100);
    arp.noteOn(3, 34);
    arp.noteOn(5,43);


    for (int i = 0; i < 10; ) {
        if (arp.service()) {
            if (arp.getGateStatus()==true) {
                cout << "arp on " << unsigned(arp.get().mNote)<< endl;
            } else { 
                cout << " arp off " << unsigned(arp.get().mNote )<< endl; 
            }
            i++;
        }
    }
    return 100;
}


int run_tests() {
    initialize_mock_arduino();

    int e = 0;
    //e=e+triad_test();

    //e = e + arpegioTest();
    e = e + conversionTest();


    cout << "press enter to continue\n";
    while (cin.get() != '\n');
    return e;
}

int main(int argc, char **argv){
    int e = run_tests();
    return e;
}
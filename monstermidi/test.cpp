#include <cstdint>
typedef uint8_t byte;
#include <iostream>
#include <choord.h>

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

int tt(byte root, byte key, ChordMaker chordMaker){
    byte triadMiddle, triadLast;
    chordMaker.setKey(key);
    chordMaker.setRoot(root);
    triadMiddle = chordMaker.getTriadMiddle();
    triadLast = chordMaker.getTriadLast();
    if (root != chordMaker.getRoot()) return 1;
    if (chordMaker.getValid() != true) return 2;
    cout << "triad is: " << unsigned(root) << " " << unsigned(triadMiddle) << " " << unsigned(triadLast)
        << " \tdiff is: " << 
        unsigned(triadMiddle - root) << " " << unsigned(triadLast - root) << endl;
    return 0;
}


void triad_test() {
    int  e=0;
    byte root =0;
    byte key = 0;
    
    ChordMaker chordMaker;
    chordMaker.setKey(key);    
    cout << "key is: " << key << "minor" << endl;

    root=key;
    e=e+ tt(root,key,chordMaker);

    
    for (int i =0; i<chordMaker.notesPerScale; i++){        
        root = root+chordMaker.minorHalfSteps[i];
        tt(root,key, chordMaker);
    }

    for (int i =0; i<chordMaker.notesPerScale; i++){        
        root = root+chordMaker.minorHalfSteps[i];
        tt(root,key, chordMaker);
    }

    if (key!=chordMaker.getKey()) e++;
    cout << "errors:  " << e << endl;
}



void run_tests() {
  triad_test();
  cout << "press enter to continue\n";
  while (cin.get() != '\n');
}

int main(int argc, char **argv){
  run_tests();
}
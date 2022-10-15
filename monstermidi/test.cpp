#include "test.h"
#include "choord.h"

using namespace std;


int tt(byte root, byte key){
    byte triadMiddle, triadLast;
    choordMaker.setKey(key);
    choordMaker.setRoot(root);
    triadMiddle = choordMaker.getTriadMiddle();
    triadLast = choordMaker.getTriadLast();
    if (root != choordMaker.getRoot()) return 1;
    if (choordMaker.getValid != true) return 2;
    cout << "triad is: " << root << " "<< triadMiddle<<" "<<triadLast << endl;
}


void triad_test() {
    int  e=0;
    byte root =0;
    byte key = 0;
    
    choordMaker ChoordMaker();
    choordMaker.setKey(key);    
    cout << "key is: " << key << "minor" << endl;

    root=key;
    e=e+ tt(root,key);

    /*
    for (int i =0; i<choordMaker.notesPerScale; i++){        
        root = root+choordMaker.minorHalfSteps[i];
        tt(root,key);
    }

    for (int i =0; i<choordMaker.notesPerScale; i++){        
        root = root+choordMaker.minorHalfSteps[i];
        tt(root,key);
    }*/

    if (key!=choordMaker.getKey()) e++;
    cout << "errors:  " << e << endl;
}



void run_tests() {
  triad_test();
}

int main(int argc, char **argv){
  run_tests();
}
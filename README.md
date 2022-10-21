# midi middle man

Next up:
clk
metronome(led)
midi arpegio
presets



Features:
Key selection: User can select any minor scale or Major scale.
Chord mode: Create Triads based on the user specified key.[done, see encoder8/restart october 2022]
Chord arpeggio mode: make auto chords with arpegios, running different patterns based on rotary encoder selection, up, updown etc
Chord arp recursion?: make chords of a chords notes
Training wheels mode: Silence or alter notes played out of key. [done, see encoder8/restart october 2022]
Keyboard split: used for either chord mode or output multiplexing [done for chord, see encoder8/restart october 2022]
Midi clocking: Accept pulses, midi in clk, or set manually via tap tempo, knob or looper input, send to all midi outs[see examples/midiclock.ino ]
Looper: up to N notes on any channel. can layer up to M layers. can quantize up to 1/4, 1/8, 1/16, 1/32, or 1/64th (user selectable) of the original loop length. additional layers can be even multiples of original loop length, or even fractional multiples
Metronome: use either the looper or midi clk to generate metronome pulses.
Jammer / Randomizer: make a musical robot for lonely musicians?
CC automation: can be run in a loop or triggered with a switch or something, divide time by X divisions and send only deltas
Presets: all setings can be set and retrieved from ROM, meaning gigging is a breeze



Hardware:
9v input like other pedals?
Usb input/output for programing, usbmidi, and maybey serial prints during debugging, make sure to put a diode or something to prevent double power
midi in 5pin (1x)
midi out 5pin (2x, 3x, 4x?)
LCD display? touch display?

User input hardware:
key set:PB (1x)  press to set new key.
chord mode: pushbutton sw (1x per output), quick press enable disable, long press, change mode from polyphonic to arpegio. 
arpegio: encoder (1x push/turn) select mode or clk division
training wheels: (1x) switch or something. to turn Passthrough, Silence, Correct,
keyboard split: PB (1x) push 
tempo / midi clk: encoder knob (1x push/turn) for tempo, maybe make it a tap tempo footswitch(1x), settings availiable via encoder
looper: pushbutton(1x?) lookup normal looper functionality
Jammer: Expression button? (1x) enhanced craziness
CC autonmation: encoder (1x push/turn) and/or make it a beefy footswitch(1x)
teensy 4.0? esp32<-bluetooth would be pretty cool for APP development?

pbs: 1111 optional:1111

TODO imporovments:
make ChoordMaker lookup table based for executuion speed?
seperate silencer from choordmaker? not necisary really


To run the testbench:
create a blank VS project (C++)
right click on proj in soulution explorer, properties>VC++ directories>external includ directories> ADD this directory to the list
right click header files>add>existing>add chord.h,time.h and any others
right click source files>add>existing>add test.cpp
click local windows debugger

# midi middle man

#pragma once
#define MOCK_ARDUINO_H

unsigned long millis();
unsigned long micros();
void delay(unsigned long ms);
void initialize_mock_arduino();


#include <iostream>

class FakeSerial {
public:
    void begin(unsigned long);
    void end();
    size_t write(const unsigned char*, size_t);
    void println(const char*);
    void println(unsigned long);
};

extern FakeSerial Serial;

extern const unsigned char MIDI_SYSRT_CLK[];




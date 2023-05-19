#ifndef PPM_READER
#define PPM_READER

#include <Arduino.h>
#include <InterruptHandler.h>


class PPMReader : InterruptHandler {

    public:
    
    // range of output from receiver
    unsigned long minChVal = 1000;
    unsigned long maxChVal = 2000;

    // Allowed error taken into account
    unsigned long channelValueMaxError = 10;

    // Minimum signal frame time
    unsigned long blankTime = 2100;


    private:

    // Interrupt Pin
    byte interruptPin = 0;

    // Amount of channels
    byte channelAmount = 0;

    // Arrays for keeping track of channel values
    volatile unsigned long *rawValues = NULL;
    volatile unsigned long *validValues = NULL;

    // A counter variable for next read channel
    volatile byte pulseCounter = 0;

    // Variable for last read pulse
    volatile unsigned long microsAtLastPulse = 0;


    public:

    PPMReader(byte interruptPin, byte channelAmount);
    ~PPMReader();

    /* Returns the latest raw (not necessarily valid) value for the
     * channel (starting from 1). */
    unsigned long rawChannelValue(byte channel);

    /* Returns the latest received value that was considered valid for the channel (starting from 1).
     * Returns defaultValue if the given channel hasn't received any valid values yet. */
    unsigned long latestValidChannelValue(byte channel, unsigned long defaultValue);

    private:

    // Interrupt service routine
    virtual void handleInterrupt(int8_t interruptNum);

};

#endif

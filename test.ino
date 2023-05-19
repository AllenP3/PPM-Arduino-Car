// Upload this code after installing the libraries
// After upload is complete, open Serial Plotter and observe graph for values
#include <PPMReader.h>
// #include <InterruptHandler.h>   <-- You may need this on some versions of Arduino

int interruptPin = 3; // interrupt pin connected to (2 or 3)
int channelAmount = 6; // number of channels
PPMReader ppm(interruptPin, channelAmount);

void setup() {
    Serial.begin(9600);
}

void loop() {
    for (int channel = 1; channel <= channelAmount; ++channel) {
        unsigned long value = ppm.latestValidChannelValue(channel, 0);
        Serial.print(String(value) + " ");
    }
    Serial.println();
 
}

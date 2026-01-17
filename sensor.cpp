#include "sensor.h"
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

float samples[60];
int head = 0;
int count = 0;

void sensor_init() {
    dht.begin();
    for(int i=0; i<60; i++) samples[i] = 0;
}

void sensor_process() {
    float t = dht.readTemperature();
    if (isnan(t)) return;

    samples[head] = t;
    head = (head + 1) % 60;
    if (count < 60) count++;

    float minVal = samples[0], maxVal = samples[0], sumWindow = 0;
    for (int i = 0; i < count; i++) {
        if (samples[i] < minVal) minVal = samples[i];
        if (samples[i] > maxVal) maxVal = samples[i];
    }

    int window = (count < 10) ? count : 10;
    for (int i = 1; i <= window; i++) {
        int idx = (head - i + 60) % 60;
        sumWindow += samples[idx];
    }

    Serial.print("Temp: "); Serial.print(t);
    Serial.print(" | Min: "); Serial.print(minVal);
    Serial.print(" | Max: "); Serial.print(maxVal);
    Serial.print(" | Moving Avg: "); Serial.println(sumWindow / window);
}
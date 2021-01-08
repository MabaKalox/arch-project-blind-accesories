#include <Arduino.h>
#include <iostream>
#include <sstream>
#include "DistanceRollClass.h"

const int POWER_KICK_PIN = 23;
unsigned long last_power_kick_time = 0;
bool was_power_kick_HIGHT = false;

const unsigned int TRIG_PIN = 26;
const unsigned int ECHO_PIN = 25;
unsigned max_distance_to_check = 200;
unsigned long duration = 0;
DistanceRollClass distance_obj;
unsigned long last_distance_check_time = 0;

const int VIBROMOTOR_PIN = 13;
const int VIBROMOTOR_CHANNEL = 1;
const int VIBROMOTOR_FREQ = 18000;
const int VIBROMOTOR_RESOLUTION = 8;
unsigned long last_vibromotor_state_update_time = 0;
bool no_signal = false;
unsigned long last_vibromotor_alert_switch_time = 0;
bool vibromotor_high = true;


std::string ToString(unsigned n) {
    std::ostringstream stm;
    stm << n;
    return stm.str();
}


double normolize(int value, int min, int max) {
    return 1.0 * (value - min) / (max - min);
}


void setup() {
    // power trigger
    pinMode(POWER_KICK_PIN, OUTPUT);
    digitalWrite(POWER_KICK_PIN, HIGH);
    was_power_kick_HIGHT = true;
    last_power_kick_time = millis();
    // echo sensor
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    last_distance_check_time = millis();
    // vibromotor
    pinMode(VIBROMOTOR_PIN, OUTPUT);
    ledcAttachPin(VIBROMOTOR_PIN, VIBROMOTOR_CHANNEL);
    ledcSetup(VIBROMOTOR_CHANNEL, VIBROMOTOR_FREQ, VIBROMOTOR_RESOLUTION);
    ledcWrite(VIBROMOTOR_CHANNEL, 255);
    last_vibromotor_state_update_time = millis();
    last_vibromotor_alert_switch_time = millis();
}

void loop() {
    if (no_signal && millis() - last_vibromotor_alert_switch_time >= 200) {
        std::cout << "Warning: no pulse from sensor" << std::endl;
        ledcWrite(VIBROMOTOR_CHANNEL, vibromotor_high * 255);
        vibromotor_high = !vibromotor_high;
        last_vibromotor_alert_switch_time = millis();
    }

    if (!no_signal && millis() - last_vibromotor_state_update_time >= 100) {
        unsigned distance = distance_obj.get_average();
        std::cout << "distance to nearest object: " + ToString(distance) + " cm." << std::endl;
        if (distance <= max_distance_to_check) {
            double normalized_value = normolize((max_distance_to_check + 2) - distance, 2, max_distance_to_check);
            std::cout << "coefficient: " << normalized_value << std::endl;
            ledcWrite(VIBROMOTOR_CHANNEL, normalized_value * 255);
        } else {
            ledcWrite(VIBROMOTOR_CHANNEL, LOW);
        }
        last_vibromotor_state_update_time = millis();
    }

    if (millis() - last_distance_check_time >= 50) {
        delayMicroseconds(2);
        digitalWrite(TRIG_PIN, HIGH);
        delayMicroseconds(10);
        digitalWrite(TRIG_PIN, LOW);

        duration = pulseIn(ECHO_PIN, HIGH);
        if (duration != 0) {
            if (no_signal) {
//                distance_obj.empty_saved_distance();
            }
            no_signal = false;
            unsigned distance = duration / 29 / 2;
            distance_obj.save_new(distance);
        } else {
            no_signal = true;
        }
        last_distance_check_time = millis();
    }

    if (!was_power_kick_HIGHT && millis() - last_power_kick_time >= 20000) {
        digitalWrite(POWER_KICK_PIN, HIGH);
        last_power_kick_time = millis();
        was_power_kick_HIGHT = true;
    }
    if (was_power_kick_HIGHT && millis() - last_power_kick_time >= 100) {
        digitalWrite(POWER_KICK_PIN, LOW);
        last_power_kick_time = millis();
        was_power_kick_HIGHT = false;
    }
}
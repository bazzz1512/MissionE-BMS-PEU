//
// Created by sebastian on 18-6-24.
//
#include "DriveModes.h"

DriveModes determine_drive_mode(PlugStateModes plug_state) {
    switch (plug_state) {
        case Disconnected:
            return DriveModes::Drive;
        case Ready:
        case Charging:
            return DriveModes::Charge;
        default:
            return DriveModes::Drive;
    }
}

void print_drive_mode(DriveModes drive_mode) {
    Serial.print("Drive mode: ");
    switch (drive_mode) {
        case Drive:
            Serial.println("Drive");
            break;
        case UPS:
            Serial.println("UPS");
            break;
        case Charge:
            Serial.println("Charge");
            break;
    }
}
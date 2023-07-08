//
// Created by sebastian on 7-7-23.
//

#include "CanData.h"
#include <Arduino.h>

sCANReceive::sCANReceive(){
    MaxPack = 0;
    MinPack = 0;
    PackSOC = 0;
    J_PlugState = 0;
    PackDCL = 0;
    PackCCL = 0;
    J_ACCurrent = 0;
}

bool sCANReceive::receive_data(uint32_t id, uint8_t *data) {
    // TODO: Add checksum check
    switch (id) {
        case 0x6B0: {
            // Extract MaxPack (2 bytes)
            MaxPack = (data[0] << 8) | data[1];

            // Extract MinPack (2 bytes)
            MinPack = (data[2] << 8) | data[3];

            // Extract PackSOC (1 byte)
            PackSOC = data[4];

            // Extract RelayState (2 bytes)
            J_PlugState = (data[5] << 8) | data[6];

            // Update successful
            return true;
        }
        case 0x6B1: {
            // Extract PackDCL (4 bytes)
            PackDCL = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];

            // Extract PackCCL (1 byte)
            PackCCL = data[4];

            // Extract J_ACCurrent (1 byte)
            J_ACCurrent = data[5];

            // Update successful
            return true;
        }
        default:
            // Unknown ID, update unsuccessful
            return false;
    }
}

void sCANReceive::print_data() {
    Serial.print("MaxPack: ");
    Serial.println(MaxPack);
    Serial.print("MinPack: ");
    Serial.println(MinPack);
    Serial.print("PackSOC: ");
    Serial.println(PackSOC);
    Serial.print("J_PlugState: ");
    Serial.println(J_PlugState);
    Serial.print("PackDCL: ");
    Serial.println(PackDCL);
    Serial.print("PackCCL: ");
    Serial.println(PackCCL);
    Serial.print("J_ACCurrent: ");
    Serial.println(J_ACCurrent);
}

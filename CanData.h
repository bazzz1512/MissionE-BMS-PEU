//
// Created by sebastian on 7-7-23.
//

#ifndef CANPROTOCOL_SKETCH_CAN_H
#define CANPROTOCOL_SKETCH_CAN_H

#include <stdint.h>

struct sCANReceive {
    uint16_t MaxPack;
    uint16_t MinPack;
    uint8_t PackSOC;
    uint16_t J_PlugState;
    uint16_t PackDCL;
    uint8_t PackCCL;
    uint8_t J_ACCurrent;

    sCANReceive();

    bool receive_data(uint32_t id, uint8_t *data);

    void print_data();
};

enum MessageID {
    Frame1 = 0x6B0,
    Frame2 = 0x6B1
};

typedef enum {
    Error = 0,
    Disconnected = 1,
    Unknown = 2,
    Ready = 3,
    Charging = 4
} PlugStateModes;

#endif //CANPROTOCOL_SKETCH_CAN_H

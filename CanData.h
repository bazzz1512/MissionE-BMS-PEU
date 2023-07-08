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
    uint32_t PackDCL;
    uint8_t PackCCL;
    uint8_t J_ACCurrent;

    sCANReceive();

    bool receive_data(uint32_t id, uint8_t *data);

    void print_data();
};

#endif //CANPROTOCOL_SKETCH_CAN_H

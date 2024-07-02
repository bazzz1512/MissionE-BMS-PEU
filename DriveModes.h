//
// Created by sebastian on 7-7-23.
//

#ifndef CANPROTOCOL_SKETCH_DRIVEMODES_H
#define CANPROTOCOL_SKETCH_DRIVEMODES_H

#include "CanData.h"
#include <Arduino.h>

typedef enum {
    Drive = 0,
    UPS = 1,
    Charge = 2,
} DriveModes;

DriveModes determine_drive_mode(PlugStateModes plug_state);

void print_drive_mode(DriveModes drive_mode);

#endif //CANPROTOCOL_SKETCH_DRIVEMODES_H

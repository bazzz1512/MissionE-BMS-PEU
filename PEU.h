#include <stdint.h>
#include <Arduino.h>

struct PeuData_Send {
    uint8_t VBLimH;
    uint8_t VBLimL;
    uint8_t CMD;
    uint8_t LineLim;
    uint8_t IBatLim;
    uint8_t UPSV;
    uint8_t AuxCMD;
    uint8_t AuxVal;

    PeuData_Send();

    void print_data();

    void set_data(uint8_t *data);

    void set_data(uint8_t VBLimH, uint8_t VBLimL, uint8_t CMD, uint8_t LineLim, uint8_t IBatLim, uint8_t UPSV,
                  uint8_t AuxCMD, uint8_t AuxVal);

    uint8_t xor_checksum();

    uint8_t *get_data();

    bool operator==(const PeuData_Send &other) const;

    bool operator!=(const PeuData_Send &other) const;
};

struct PeuData_Receive {
    // AZ
    uint16_t MotTemp;
    uint16_t PEUTemp;
    uint16_t I_Line;
    uint16_t V_Line;

    // AY
    uint16_t V_APS;
    uint8_t SerErr;
    uint8_t PEUStat;
    uint16_t CMCStat;
    uint8_t AuxStat;
    uint8_t ReqVal;

    // AX
    uint16_t V_Bat;
    uint16_t I_Sys;
    uint16_t I_Hyb;
    uint16_t I_Aux;
    uint16_t PassVal;
    uint16_t TachCnt;

    // Global checksum
    uint8_t checkval;

    PeuData_Receive();

    void print_data();

    void set_az(uint8_t *data);

    void set_az(uint16_t MotTemp, uint16_t PEUTemp, uint16_t I_Line, uint16_t V_Line);

    void set_ay(uint8_t *data);

    void set_ay(uint16_t V_APS, uint8_t SerErr, uint8_t PEUStat, uint16_t CMCStat, uint8_t AuxStat, uint8_t ReqVal);

    void set_ax(uint8_t *data);

    void set_ax(uint16_t V_Bat, uint16_t I_Sys, uint16_t I_Hyb, uint16_t I_Aux, uint16_t PassVal, uint16_t TachCnt);

    uint8_t xor_checksum(uint8_t *data, uint8_t len);

};


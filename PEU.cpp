//
// Created by sebastian on 12-6-23.
//

#include "PEU.h"

PeuData_Send::PeuData_Send() {
    VBLimH = 0;
    VBLimL = 0;
    CMD = 0;
    LineLim = 0;
    IBatLim = 0;
    UPSV = 0;
    AuxCMD = 0;
    AuxVal = 0;
}

void PeuData_Send::print_data() {
    Serial.print("VBLimH: ");
    Serial.println(VBLimH, HEX);
    Serial.print("VBLimL: ");
    Serial.println(VBLimL, HEX);
    Serial.print("CMD: ");
    Serial.println(CMD, HEX);
    Serial.print("LineLim: ");
    Serial.println(LineLim, HEX);
    Serial.print("IBatLim: ");
    Serial.println(IBatLim, HEX);
    Serial.print("UPSV: ");
    Serial.println(UPSV, HEX);
    Serial.print("AuxCMD: ");
    Serial.println(AuxCMD, HEX);
    Serial.print("AuxVal: ");
    Serial.println(AuxVal, HEX);
}

void PeuData_Send::set_data(uint8_t *data) {
    VBLimH = data[0];
    VBLimL = data[1];
    CMD = data[2];
    LineLim = data[3];
    IBatLim = data[4];
    UPSV = data[5];
    AuxCMD = data[6];
    AuxVal = data[7];
}

void PeuData_Send::set_data(uint8_t VBLimH, uint8_t VBLimL, uint8_t CMD, uint8_t LineLim, uint8_t IBatLim, uint8_t UPSV,
                            uint8_t AuxCMD, uint8_t AuxVal) {
    this->VBLimH = VBLimH;
    this->VBLimL = VBLimL;
    this->CMD = CMD;
    this->LineLim = LineLim;
    this->IBatLim = IBatLim;
    this->UPSV = UPSV;
    this->AuxCMD = AuxCMD;
    this->AuxVal = AuxVal;
}

uint8_t PeuData_Send::xor_checksum() {
    return static_cast<uint8_t>('A') ^ static_cast<uint8_t>('z') ^ VBLimH ^ VBLimL ^ CMD ^ LineLim ^ IBatLim ^ UPSV ^
           AuxCMD ^ AuxVal;
}

uint8_t *PeuData_Send::get_data() {
    uint8_t* data = new uint8_t[11] {'A', 'z', VBLimH, VBLimL, CMD, LineLim, IBatLim, UPSV, AuxCMD,
                               AuxVal, xor_checksum()};
    return data;
}

bool PeuData_Send::operator==(const PeuData_Send &other) const {
    return VBLimH == other.VBLimH && VBLimL == other.VBLimL && CMD == other.CMD && LineLim == other.LineLim &&
           IBatLim == other.IBatLim && UPSV == other.UPSV && AuxCMD == other.AuxCMD && AuxVal == other.AuxVal;
}

bool PeuData_Send::operator!=(const PeuData_Send &other) const {
    return !(*this == other);
}

PeuData_Receive::PeuData_Receive() {
    // Constructor to initialize the struct members
    MotTemp = 0;
    PEUTemp = 0;
    I_Line = 0;
    V_Line = 0;
    V_APS = 0;
    SerErr = 0;
    PEUStat = 0;
    CMCStat = 0;
    AuxStat = 0;
    ReqVal = 0;
    V_Bat = 0;
    I_Sys = 0;
    I_Hyb = 0;
    I_Aux = 0;
    PassVal = 0;
    TachCnt = 0;
    checkval = 0;
}

void PeuData_Receive::print_data() {
    // Print the values of the struct members
    Serial.print("MotTemp: ");
    Serial.println(MotTemp);
    Serial.print("PEUTemp: ");
    Serial.println(PEUTemp);
    Serial.print("I_Line: ");
    Serial.println(I_Line);
    Serial.print("V_Line: ");
    Serial.println(V_Line);
    Serial.print("V_APS: ");
    Serial.println(V_APS);
    Serial.print("SerErr: ");
    Serial.println(SerErr);
    Serial.print("PEUStat: ");
    Serial.println(PEUStat);
    Serial.print("CMCStat: ");
    Serial.println(CMCStat);
    Serial.print("AuxStat: ");
    Serial.println(AuxStat);
    Serial.print("ReqVal: ");
    Serial.println(ReqVal);
    Serial.print("V_Bat: ");
    Serial.println(V_Bat);
    Serial.print("I_Sys: ");
    Serial.println(I_Sys);
    Serial.print("I_Hyb: ");
    Serial.println(I_Hyb);
    Serial.print("I_Aux: ");
    Serial.println(I_Aux);
    Serial.print("PassVal: ");
    Serial.println(PassVal);
    Serial.print("TachCnt: ");
    Serial.println(TachCnt);
    Serial.print("checkval: ");
    Serial.println(checkval);
}

void PeuData_Receive::set_az(uint8_t *data) {
    // Set values for AZ members from an array of bytes
    MotTemp = static_cast<uint16_t>((data[0] << 8) | data[1]);
    PEUTemp = static_cast<uint16_t>((data[2] << 8) | data[3]);
    I_Line = static_cast<uint16_t>((data[4] << 8) | data[5]);
    V_Line = static_cast<uint16_t>((data[6] << 8) | data[7]);
}

void PeuData_Receive::set_az(uint16_t MotTemp, uint16_t PEUTemp, uint16_t I_Line, uint16_t V_Line) {
    // Set values for AZ members directly
    this->MotTemp = MotTemp;
    this->PEUTemp = PEUTemp;
    this->I_Line = I_Line;
    this->V_Line = V_Line;
}

void PeuData_Receive::set_ay(uint8_t *data) {
    // Set values for AY members from an array of bytes
    V_APS = static_cast<uint16_t>((data[0] << 8) | data[1]);
    SerErr = data[2];
    PEUStat = data[3];
    CMCStat = static_cast<uint16_t>((data[4] << 8) | data[5]);
    AuxStat = data[6];
    ReqVal = data[7];
}

void PeuData_Receive::set_ay(uint16_t V_APS, uint8_t SerErr, uint8_t PEUStat, uint16_t CMCStat, uint8_t AuxStat,
                             uint8_t ReqVal) {
    // Set values for AY members directly
    this->V_APS = V_APS;
    this->SerErr = SerErr;
    this->PEUStat = PEUStat;
    this->CMCStat = CMCStat;
    this->AuxStat = AuxStat;
    this->ReqVal = ReqVal;
}

void PeuData_Receive::set_ax(uint8_t *data) {
    // Set values for AX members from an array of bytes
    V_Bat = static_cast<uint16_t>((data[0] << 8) | data[1]);
    I_Sys = static_cast<uint16_t>((data[2] << 8) | data[3]);
    I_Hyb = static_cast<uint16_t>((data[4] << 8) | data[5]);
    I_Aux = static_cast<uint16_t>((data[6] << 8) | data[7]);
    PassVal = static_cast<uint16_t>((data[8] << 8) | data[9]);
    TachCnt = static_cast<uint16_t>((data[10] << 8) | data[11]);
}

void PeuData_Receive::set_ax(uint16_t V_Bat, uint16_t I_Sys, uint16_t I_Hyb, uint16_t I_Aux, uint16_t PassVal,
                             uint16_t TachCnt) {
    // Set values for AX members directly
    this->V_Bat = V_Bat;
    this->I_Sys = I_Sys;
    this->I_Hyb = I_Hyb;
    this->I_Aux = I_Aux;
    this->PassVal = PassVal;
    this->TachCnt = TachCnt;
}

uint8_t PeuData_Receive::xor_checksum(uint8_t *data, uint8_t len) {
    // Calculate XOR checksum of data
    uint8_t checksum = 0;
    for (int i = 0; i < len; i++) {
        checksum ^= data[i];
    }
    return checksum;
}

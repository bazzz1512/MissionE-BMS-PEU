#include <ACAN2515.h>
#include "SoftwareSerial.h"
#include "RS232Communication.h"
#include <SPI.h>
#include "PEU.h"
#include "CanData.h"
#include "DriveModes.h"

// Set the frequency of the clock for the CAN bus
#define QUARTZ_FREQUENCY 8UL * 1000UL * 1000UL

// Set to true to enable debug prints
#define DEBUG false

static const uint8_t MCP2515_CS = A0;  // CS input of MCP2515 (adapt to your design)
static const uint8_t MCP2515_INT = A1;  // INT output of MCP2515 (adapt to your design)

static const uint8_t RX_PIN = D0;  // Serial RX pin
static const uint8_t TX_PIN = D1;  // Serial TX pin

PeuData_Receive peu_data; // Data that is received from the PEU
sCANReceive can_receive_data; // Data that is received from the CAN bus

ACAN2515 can(MCP2515_CS, SPI, MCP2515_INT); // CAN bus object
RS232Communication rs232(RX_PIN, TX_PIN); //


// Create timestamp variable
unsigned long previousMillis_send_data = 0;
unsigned long start_program_time = 0;

// Select drive mode
DriveModes drive_mode = DriveModes::Charge;

// Function to print an array
void printArray(const uint8_t *array, uint8_t length) {
    for (uint8_t i = 0; i < length; i++) {
        Serial.print(array[i]);
        Serial.print(' ');
    }
    Serial.println();
}

void setup() {
    //--- Switch on builtin led
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    //--- Initialize serial
    Serial.begin(57600);
    //--- Wait for serial (blink led at 10 Hz during waiting)
    while (!Serial) {
        delay(50);
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    }
    Serial.println("Initialize");
    // --- Begin SPI
    SPI.begin();
    //--- Configure ACAN2515
    Serial.println("Configure ACAN2515");
    ACAN2515Settings settings(QUARTZ_FREQUENCY, 500UL * 1000UL);  // CAN bit rate 500 kb/s
    settings.mRequestedMode = ACAN2515Settings::ListenOnlyMode;     // Select loopback mode - no transmission
    const uint16_t errorCode = can.begin(settings, [] {
        can.isr();
    });
    if (errorCode == 0) {
        Serial.println("Can initialized");
        Serial.print("Bit Rate prescaler: ");
        Serial.println(settings.mBitRatePrescaler);
        Serial.print("Propagation Segment: ");
        Serial.println(settings.mPropagationSegment);
        Serial.print("Phase segment 1: ");
        Serial.println(settings.mPhaseSegment1);
        Serial.print("Phase segment 2: ");
        Serial.println(settings.mPhaseSegment2);
        Serial.print("SJW: ");
        Serial.println(settings.mSJW);
        Serial.print("Triple Sampling: ");
        Serial.println(settings.mTripleSampling ? "yes" : "no");
        Serial.print("Actual bit rate: ");
        Serial.print(settings.actualBitRate());
        Serial.println(" bit/s");
        Serial.print("Exact bit rate ? ");
        Serial.println(settings.exactBitRate() ? "yes" : "no");
        Serial.print("Sample point: ");
        Serial.print(settings.samplePointFromBitStart());
        Serial.println("%");
    } else {
        Serial.print("Configuration error 0x");
        Serial.println(errorCode, HEX);
    }
    Serial.println("Starting!");
    // Initialize RS232
    rs232.initialize();
    // Initialize timestamp variable
    previousMillis_send_data = millis();
    start_program_time = millis();
}

//——————————————————————————————————————————————————————————————————————————————

void loop() {
    if (DEBUG)
        Serial.println("Starting loop");

    // Check if data is available on the CAN bus
    if (can.available()) {
        CANMessage frame;
        can.receive(frame);
        if (can_receive_data.receive_data(frame.id, frame.data)) {
            if (DEBUG) {
                Serial.println("Received CAN data");
                // can_receive_data.print_data();
            }
        } else {
            if (DEBUG) {
                Serial.println("Error: Unknown data type in received data");
                Serial.print("ID: ");
                Serial.println(frame.id);
                Serial.print("Data: ");
                printArray(frame.data, 8);
            }
        }
    }

    if (DEBUG)
        Serial.println("Checking RS232");

    // Initialize data buffer of max 34 bytes
    uint8_t data_buffer[34] = {0};
    // CHeck available data on hardware serial
    if (rs232.check_data(data_buffer)) {
        if (data_buffer[1] == 'X') {
            peu_data.set_ax(&data_buffer[2]);
        } else if (data_buffer[1] == 'Y') {
            peu_data.set_ay(&data_buffer[2]);
            peu_data.set_ax(&data_buffer[12]);
        } else if (data_buffer[1] == 'Z') {
            peu_data.set_az(&data_buffer[2]);
            peu_data.set_ay(&data_buffer[12]);
            peu_data.set_ax(&data_buffer[22]);
        } else {
            Serial.println("Error: Unknown data type in received data");
        }
        if (DEBUG) {
            Serial.println("Received RS232 parsed data: ");
            peu_data.print_data();
        }
    }
    if (DEBUG)
        Serial.println("Checking CAN");

    // Send data every 100 ms
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis_send_data >= 100) {
        // Save the last time a message was sent
        previousMillis_send_data = currentMillis;
        // Create message to send with RS232 data
        PeuData_Send peu_data_send;

        // Set data to send
        switch (drive_mode) {
            case DriveModes::Drive:
                peu_data_send.VBLimH = 86; // Offset 390 V
                peu_data_send.VBLimL = 202; // Offset 356 V

                peu_data_send.CMD = 0;
                // peu_data_send.CMD |= 1 << 1; // LP
                peu_data_send.CMD |= 1 << 5;

                peu_data_send.LineLim = 128;
                peu_data_send.UPSV = 230;
                peu_data_send.AuxCMD = 0;
                peu_data_send.AuxVal = 0;
                break;
            case DriveModes::UPS:
                // TODO: Change
                peu_data_send.VBLimH = 86; // Offset 390 V
                peu_data_send.VBLimL = 202; // Offset 356 V

                peu_data_send.CMD = 0;
                // peu_data_send.CMD |= 1 << 1; // LP
                peu_data_send.CMD |= 1 << 5;

                peu_data_send.LineLim = 32;
                peu_data_send.UPSV = 230;
                peu_data_send.AuxCMD = 0;
                peu_data_send.AuxVal = 0;
                break;

            case DriveModes::Charge:
                // TODO: Change
                // Get current time
                unsigned long currentMillis = millis();
                // Check if 5 seconds have passed
                if (currentMillis - start_program_time <= 5000) {
                    // Serial.println("Changing charging mode");
                    // Save the last time a message was sent
                    peu_data_send.VBLimH = 225;
                    peu_data_send.VBLimL = 0;

                    peu_data_send.CMD = 8;
                    // peu_data_send.CMD |= 1 << 5; // LP
//                    peu_data_send.CMD |= 1 << 3; // SS


                    // Mode dependent
                    peu_data_send.LineLim = 255;
                    peu_data_send.IBatLim = 58;

                    // Constant
                    peu_data_send.UPSV = 0;
                    peu_data_send.AuxCMD = 0;
                    peu_data_send.AuxVal = 0;
                }
                else {
                    peu_data_send.VBLimH = 225;
                    peu_data_send.VBLimL = 0;

                    peu_data_send.CMD = 255;
                    // peu_data_send.CMD |= 1 << 5; // LP
                    // peu_data_send.CMD |= 1 << 3; // SS



                    // Mode dependent
                    peu_data_send.LineLim = 250;
                    peu_data_send.IBatLim = 58;

                    // Constant
                    peu_data_send.UPSV = 0;
                    peu_data_send.AuxCMD = 0;
                    peu_data_send.AuxVal = 0;
                }
                break;

        }
        // Create data array from PeuData_Send struct
        if (DEBUG){
          Serial.println("Sending RS232 data to PEU");
          // peu_data_send.print_data();
        }

        uint8_t *data = peu_data_send.get_data();
        printArray(data, 11);
        rs232.send_data(data, 11);
    }
}

#include "RS232Communication.h"
#include "HardwareSerial.h"

static const long TIMEOUT_MS = 2000; // Set timeout to 2000 milliseconds, adjust this value as needed


RS232Communication::RS232Communication(uint8_t rx, uint8_t tx) : rx_pin(rx), tx_pin(tx), hardserial(rx, tx) {
    baudrate = 57600;
}

void RS232Communication::send_data(uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        hardserial.write(data[i]);
    }
}

void RS232Communication::receive_data(uint8_t *data, int len) {
    for (int i = 0; i < len; i++) {
        if (hardserial.available()) {
            data[i] = hardserial.read();
        }
    }
}

int RS232Communication::check_and_receive_data(uint8_t *data, int max_len) {
    
    for (int i = 0; i < max_len; i++) {
        if (hardserial.available()) {
            data[i] = hardserial.read();
        } else {
            return i;
        }
    }
    return true;
}

void RS232Communication::initialize() {
    hardserial.begin(baudrate);
    Serial.println("Hardserial Initialized");
}

bool RS232Communication::check_data(uint8_t* data){
    // Check if there is data available
    if (hardserial.available() < 14) {
        return false;
    }
//    Serial.println("Data available");
    uint8_t read_bytes_amount = 0;

    // Wait for 'A' to be received
    unsigned long start_time = millis();
    while (hardserial.read() != 'A') {
        if (millis() - start_time > TIMEOUT_MS) {
            Serial.println("Timeout waiting for 'A'");
            return false;
        }
    }

    // Check the status byte
    char status = hardserial.read();
    switch (status) {
        case 'Z':
            read_bytes_amount = 34;
            break;
        case 'Y':
            read_bytes_amount = 24;
            break;
        case 'X':
            read_bytes_amount = 14;
            break;
        default:
            Serial.print("Wrong status: ");
            Serial.println(status);
            return false;
    }

    // Wait for the rest of the data to be received
    start_time = millis();  // Reset the start time
    while (hardserial.available() < read_bytes_amount-2) {
        if (millis() - start_time > TIMEOUT_MS) {
            Serial.println("Timeout waiting for data");
            return false;
        }
    }

    // Parse the data
    data[0] = 'A';
    data[1] = status;
    while (hardserial.available() < read_bytes_amount-2);
    for (uint8_t i = 2; i < read_bytes_amount; i++) {
        data[i] = hardserial.read();
    }

    // Check the checksum
    uint8_t checksum = data[0];
    for (uint8_t i = 1; i < read_bytes_amount-1; i++) {
        checksum ^= data[i];
    }
    if (checksum == data[read_bytes_amount-1]) {
        return true;
    } else {
        return false;
    }

}
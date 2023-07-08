#ifndef RS232_COMMUNICATION_H
#define RS232_COMMUNICATION_H

#include <stdint.h>
#include <SoftwareSerial.h>

struct RS232Communication {
    int baudrate;
    uint8_t rx_pin;
    uint8_t tx_pin;

    HardwareSerial hardserial;

    RS232Communication(uint8_t rx, uint8_t tx);

    void send_data(uint8_t *data, int len);

    void receive_data(uint8_t *data, int len);

    int check_and_receive_data(uint8_t *data, int max_len);

    void initialize();

    bool check_data(uint8_t *data);
};

#endif

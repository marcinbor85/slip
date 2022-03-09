#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "slip.h"

void recv_message(void *state, uint8_t *data, uint32_t size);
uint8_t write_byte(void *state, uint8_t byte);

static uint8_t buf[100];

static const slip_descriptor_s slip_descriptor = {
        .buf = buf,
        .buf_size = sizeof(buf),
        .crc_seed = 0xFFFF,
        .send_recv_state = NULL,
        .recv_message = recv_message,
        .write_byte = write_byte,
};

static slip_handler_s slip;

int main(void)
{
        slip_init(&slip, &slip_descriptor);

        char *to_send = "test";
        printf("SEND: %s\n", to_send);
        slip_send_message(&slip, to_send, strlen(to_send));

        uint8_t emulated_recv[] = {0xC0, 0x74, 0x65, 0x73, 0x74, 0x1F, 0xC6, 0xC0};
        for (uint8_t i = 0; i < sizeof(emulated_recv); i++) {
                uint8_t rx = emulated_recv[i];
                printf("RX: %02X\n", rx);
                slip_read_byte(&slip, rx);
        }
        return 0;
}

void recv_message(void *state, uint8_t *data, uint32_t size)
{
        char recv_buf[size + 1];

        memcpy(recv_buf, data, size);
        recv_buf[size] = 0;

        printf("RECV: %s\n", recv_buf);
}

uint8_t write_byte(void *state, uint8_t byte) 
{
        printf("TX: %02X\n", byte);
        return 1;
}

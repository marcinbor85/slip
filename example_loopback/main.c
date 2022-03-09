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

static uint8_t loopback_buf_index;
static uint8_t loopback_buf[100];

void send(char *to_send)
{
        loopback_buf_index = 0;
        printf("SEND: %s\n", to_send);
        slip_send_message(&slip, to_send, strlen(to_send));

        for (uint8_t i = 0; i < loopback_buf_index; i++) {
                uint8_t rx = loopback_buf[i];
                slip_read_byte(&slip, rx);
        }
}

int main(void)
{
        slip_init(&slip, &slip_descriptor);

        send("test");
        send("message");
        send("value 123");
        
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
        loopback_buf[loopback_buf_index++] = byte;
        return 1;
}

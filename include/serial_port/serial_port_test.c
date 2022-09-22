#include "serial_port.h"

int main(int argc, char const* argv[])
{
    int fd;
    fd = serial_port_open(fd, "/dev/ttyS0");
    serial_port_init(fd, 115200, 0, 8, 1, 'N');

    char open_light[5]  = { 0xdd, 0x05, 0x24, 0x00, 0x00 };
    char close_light[5] = { 0xdd, 0x05, 0x24, 0x00, 0x01 };

    serial_port_send(fd, open_light, sizeof(open_light));
    sleep(10);
    serial_port_send(fd, close_light, sizeof(close_light));

    // int len;
    // char recv_buf[36];
    // while (1) {
    //     memset(recv_buf, 0, sizeof(recv_buf));
    //     len = serial_port_recv(fd, recv_buf, 36);
    //     if (len > 0) {
    //         for (size_t i = 0; i < 36; i++) {
    //             printf("%x ", recv_buf[i]);
    //         }
    //         printf("\n");
    //     }
    //     else {
    //         printf("cannot receive data\n");
    //     }
    //     usleep(1000);
    // }

    serial_port_close(fd);

    return 0;
}

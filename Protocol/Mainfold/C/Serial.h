//
// Created by wzq on 23/02/2017.
//
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdint.h>
#ifndef QRCODE_SERIAL_H
#define QRCODE_SERIAL_H


class Serial {
private:
    int fd;
public:
    Serial(char * name);

    int set_interface_attribs(int fd, int speed);
    void send(uint8_t * data,int size);
    void send(uint8_t data);

};

#endif //QRCODE_SERIAL_H

//
// Created by wzq on 23/02/2017.
//

#include "Serial.h"
Serial * serial;
UDPClient *udpClient;
int Serial::set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        printf("Error from tcgetattr: %s\n", strerror(errno));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        printf("Error from tcsetattr: %s\n", strerror(errno));
        return -1;
    }
    return 0;
};
Serial::Serial(char *name) {
    fd = open(name, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        printf("Error opening %s: %s\n", name, strerror(errno));

        exit(0);
        return;
    }
    set_interface_attribs(fd, B115200);
}
void Serial::send(uint8_t data) {
    uint8_t temp[1]={data};
    int wlen = write(fd, temp, 1);
}
void Serial::send(uint8_t *data, int size) {
    int wlen = write(fd, data, size);

}
Serial::~Serial() {
    close(fd);
}
BufferData* Serial::Read(){
    uint8_t buffer[1024];
    size_t length = read(fd,buffer,sizeof(buffer));
    BufferData* res=new BufferData;
    res->data=new uint8_t[length];
    res->len=length;
    memcpy(res->data,buffer,length);
    return res;


}
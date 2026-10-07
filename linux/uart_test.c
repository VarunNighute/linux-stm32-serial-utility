#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <string.h>

int main()
{
    int fd = open("/dev/serial0", O_RDWR | O_NOCTTY);

    if (fd < 0) {
        perror("UART open failed");
        return 1;
    }

    struct termios tty;

    tcgetattr(fd, &tty);

    cfsetispeed(&tty, B115200);
    cfsetospeed(&tty, B115200);

    tty.c_cflag |= CLOCAL | CREAD;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CRTSCTS;

    tty.c_lflag = 0;
    tty.c_iflag = 0;
    tty.c_oflag = 0;

    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 20;

    tcsetattr(fd, TCSANOW, &tty);

    tcflush(fd, TCIOFLUSH);

    char msg[] = "A";

    printf("Sending: %s\n", msg);

    write(fd, msg, strlen(msg));

    char buffer[100] = {0};

    int n = read(fd, buffer, sizeof(buffer) - 1);

    if (n > 0)
        printf("STM32 replied: %s\n", buffer);
    else
        printf("No response from STM32\n");

    close(fd);

    return 0;
}

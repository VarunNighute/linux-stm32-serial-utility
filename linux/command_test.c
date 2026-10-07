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

    /*
     * Return when at least one byte arrives.
     * Timeout after 1 second.
     */
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 10;

    tcsetattr(fd, TCSANOW, &tty);

    const char *commands[] = {
        "LED ON\n",
        "STATUS\n",
        "LED OFF\n",
        "STATUS\n"
    };

    char buffer[100];

    for (int i = 0; i < 4; i++)
    {
        printf("Sending: %s", commands[i]);

        tcflush(fd, TCIOFLUSH);

        write(fd, commands[i], strlen(commands[i]));

        int total = 0;

        memset(buffer, 0, sizeof(buffer));

        /*
         * Read until newline.
         */
        while (total < sizeof(buffer) - 1)
        {
            int n = read(fd, buffer + total,
                         sizeof(buffer) - 1 - total);

            if (n > 0)
            {
                total += n;

                if (buffer[total - 1] == '\n')
                    break;
            }
            else
            {
                break;
            }
        }

        buffer[total] = '\0';

        printf("STM32: %s", buffer);
    }

    close(fd);

    return 0;
}

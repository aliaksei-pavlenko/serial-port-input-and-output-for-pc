#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <signal.h>

static struct termios oldt;

void restore_terminal(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
}

int main(void) {
    int usb = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY);
    if (usb < 0) {
        perror("open");
        return 1;
    }

    // Save and switch terminal to raw
    struct termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    cfmakeraw(&newt);
    newt.c_lflag |= ISIG;   // keep ISIG enabled
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Make sure terminal is restored at exit
    atexit(restore_terminal);

    // Forward between stdin and USB
    char buf[256];
    while (1) {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);
        FD_SET(usb, &fds);
        int maxfd = (usb > STDIN_FILENO ? usb : STDIN_FILENO) + 1;

        if (select(maxfd, &fds, NULL, NULL, NULL) < 0) {
            perror("select");
            break;
        }

        if (FD_ISSET(STDIN_FILENO, &fds)) {
            ssize_t n = read(STDIN_FILENO, buf, sizeof(buf));
            if (n > 0) write(usb, buf, n);
        }
        if (FD_ISSET(usb, &fds)) {
            ssize_t n = read(usb, buf, sizeof(buf));
            if (n > 0) write(STDOUT_FILENO, buf, n);
        }
    }

    close(usb);
    return 0;
}

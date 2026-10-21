#include "button.h"
#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/select.h>
#include <string.h>

// Track previous button state to detect only press events (not release)
static int previous_button_state = 0;

// Debug mode - set to 1 to see raw packet data
static int debug_packets = 0;

int open_serial_port(const char* port, unsigned int baud) {
    int fd = open(port, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) return -1;
    
    fcntl(fd, F_SETFL, 0);
    struct termios tty;
    tcgetattr(fd, &tty);
    
    // Set baud rate based on parameter
    speed_t speed;
    switch(baud) {
        case 9600:
            speed = B9600;
            break;
        case 57600:
            speed = B57600;
            break;
        default:
            speed = B57600;
    }
    
    cfsetispeed(&tty, speed);
    cfsetospeed(&tty, speed);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 0;
    
    tcsetattr(fd, TCSANOW, &tty);
    
    // Flush any pending data
    tcflush(fd, TCIOFLUSH);
    
    return fd;
}

int check_hardware_input(int fd) {
    if (fd == -1) return -1;
    
    unsigned char header;
    ssize_t n = read(fd, &header, 1);
    
    if (n <= 0) return -1;
    
    if (header == 0x55) {
        // Small delay to allow more data to arrive
        usleep(2000);  // 2ms delay
        
        // Read the rest of the Enocean packet
        unsigned char packet[64];
        memset(packet, 0, sizeof(packet));
        
        // Read up to 63 more bytes (non-blocking)
        int bytes_read = read(fd, packet, 63);
        
        if (bytes_read < 1) {
            return -1;
        }
        
        // Look for MAC address byte 0x3C in the packet
        int mac_found = 0;
        int mac_pos = -1;
        for (int i = 0; i < bytes_read; i++) {
            if (packet[i] == 0x3C) {
                mac_found = 1;
                mac_pos = i;
                break;
            }
        }
        
        if (mac_found && mac_pos >= 0) {
            // Status byte is at position 6 of the packet (packet[6])
            // 0x10 = pressed, 0x00 = released
            unsigned char status = packet[6];
            int current_button_state = (status & 0x10) ? 1 : 0;
            
            FILE *debug_log = fopen("/tmp/button_debug.log", "a");
            if (debug_log) {
                fprintf(debug_log, "Status=0x%02X (state=%d, prev=%d) -> ", status, current_button_state, previous_button_state);
                if (current_button_state == 1 && previous_button_state == 0) {
                    fprintf(debug_log, "PRESS DETECTED!\n");
                } else {
                    fprintf(debug_log, "no change\n");
                }
                fclose(debug_log);
            }
            
            // Only report when transitioning from released (0) to pressed (1)
            if (current_button_state == 1 && previous_button_state == 0) {
                previous_button_state = current_button_state;
                return '1'; // Button press detected
            }
            
            // Update state for next iteration
            previous_button_state = current_button_state;
            return -1;
        }
        
        return -1;
    }
    
    return -1;
}

int kb_kbhit() {
    struct timeval tv = {0L, 0L};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
}

int kb_getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

void send_command(int fd, const char* command) {
    if (fd == -1) return;
    write(fd, command, strlen(command));
    write(fd, "\n", 1);
}

void close_serial_port(int fd) {
    if (fd != -1) close(fd);
}

void enable_button_debug() {
    debug_packets = 1;
}
#ifndef BUTTON_H
#define BUTTON_H

extern int serial_fd;           // Enocean device
extern int arduino_fd;          // Arduino device

int open_serial_port(const char* port, unsigned int baud);
void close_serial_port(int fd);
void send_command(int fd, const char* command);
int check_hardware_input(int fd);
void enable_button_debug();     // Enable debug output for button

// Renommé pour éviter le conflit avec ncurses
int kb_kbhit();
int kb_getch();

#endif
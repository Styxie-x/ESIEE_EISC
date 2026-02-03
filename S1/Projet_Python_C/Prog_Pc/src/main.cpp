#include "../lib/app/app.h"
#include "../lib/button/button.h"
#include <unistd.h>

int main() {
    // Open serial port for Enocean device (57600 baud)
    serial_fd = open_serial_port("/dev/ttyUSB0", 57600);
    
    // Open serial port for Arduino (9600 baud)
    arduino_fd = open_serial_port("/dev/ttyUSB1", 9600);
    
    init_interface();

    show_main_menu();

    close_interface();
    close_serial_port(serial_fd);
    close_serial_port(arduino_fd);
    return 0;
}
#ifndef APP_H
#define APP_H

#include <ncurses.h>

extern int seuil_temp;
extern int led_state;
extern int serial_fd;
extern int arduino_fd;
extern int current_temperature;

void init_interface();
void close_interface();
void update_display();
void handle_input(int ch);
void show_main_menu();
void menu_temperature();
void menu_button();
void menu_light();
int read_arduino_temperature();

#endif
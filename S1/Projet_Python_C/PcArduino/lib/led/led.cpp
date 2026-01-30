#include "led.h"
#include "../button/button.h"
#include "../app/app.h"
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>

void menu_light() {
    int button_pressed_time = 0;  // Time to show "button pressed" message
    
    while (1) {
        erase();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(2, 5, "=== CONTROLE ECLAIRAGE ===");
        attroff(COLOR_PAIR(1) | A_BOLD);
        
        mvprintw(4, 5, "LED: %s", (led_state == 0 ? "OFF" : (led_state == 1 ? "ON" : "BLINK")));
        mvprintw(6, 5, "1) ON");
        mvprintw(7, 5, "2) OFF");
        mvprintw(8, 5, "3) BLINK");
        mvprintw(9, 5, "0) Retour");
        
        if (arduino_fd != -1) {
            mvprintw(11, 5, "Arduino: Connecte");
        } else {
            attron(COLOR_PAIR(3));
            mvprintw(11, 5, "Arduino: Non connecte");
            attroff(COLOR_PAIR(3));
        }
        
        // Show button pressed message if recently triggered
        if (button_pressed_time > 0) {
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(13, 5, "Bouton Enocean: PRESSE!");
            attroff(COLOR_PAIR(2) | A_BOLD);
            button_pressed_time--;
        }
        
        refresh();

        int in = -1;
        
        // Check keyboard input
        if (kb_kbhit()) in = kb_getch();
        
        // Check Enocean button
        int button_press = check_hardware_input(serial_fd);
        if (button_press != -1) {
            // Toggle LED on/off
            if (led_state == 0) {
                led_state = 1;
                send_command(arduino_fd, "LED:ON");
            } else {
                led_state = 0;
                send_command(arduino_fd, "LED:OFF");
            }
            button_pressed_time = 5;  // Show message for 1 second (5 * 200ms)
        }

        if (in == '1') { 
            led_state = 1; 
            send_command(arduino_fd, "LED:ON"); 
        }
        if (in == '2') { 
            led_state = 0; 
            send_command(arduino_fd, "LED:OFF"); 
        }
        if (in == '3') { 
            led_state = 2; 
            send_command(arduino_fd, "LED:BLINK"); 
        }
        if (in == '0') return;

        usleep(200000);
    }
}
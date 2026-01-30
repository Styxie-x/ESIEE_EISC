#include "app.h"
#include "../button/button.h"
#include <string.h>
#include <unistd.h>

int seuil_temp = 25;
int led_state = 0;
int serial_fd = -1;
int arduino_fd = -1;

void init_interface() {
    initscr();              // Initialise ncurses
    cbreak();               // Désactive le buffer de ligne
    noecho();               // Ne pas afficher les touches pressées
    curs_set(0);           // Cache le curseur
    nodelay(stdscr, TRUE);  // getch() ne bloque pas le programme
    start_color();          // Active les couleurs
    
    init_pair(1, COLOR_CYAN, COLOR_BLACK);
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
}

void update_display() {
    erase(); // Nettoie l'écran en mémoire
    
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(1, 2, "=== SYSTEME DOMOTIQUE ARCH-AETHERLINX ===");
    attroff(COLOR_PAIR(1) | A_BOLD);

    // Section Température
    mvprintw(4, 2, "TEMPERATURE :");
    mvprintw(5, 4, "Seuil actuel : %d C", seuil_temp);
    
    // Section LED
    mvprintw(8, 2, "ETAT LED :");
    if (led_state == 1) {
        attron(COLOR_PAIR(2)); mvprintw(8, 15, "[ ALLUMEE ]"); attroff(COLOR_PAIR(2));
    } else {
        attron(COLOR_PAIR(3)); mvprintw(8, 15, "[ ETEINTE ]"); attroff(COLOR_PAIR(3));
    }

    // Menu
    mvprintw(12, 2, "COMMANDES : [1] ON | [2] OFF | [S] Seuil | [0] Quitter");
    mvprintw(14, 2, "Port Serie : /dev/ttyUSB0 (57600 baud)");

    refresh(); // Affiche tout d'un coup (évite le clignotement)
}

void close_interface() {
    endwin();
}

void show_main_menu() {
    while (1) {
        erase();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(2, 10, "=== MENU PRINCIPAL ===");
        attroff(COLOR_PAIR(1) | A_BOLD);
        
        mvprintw(5, 10, "1) Temperature");
        mvprintw(6, 10, "2) Eclairage (Light)");
        mvprintw(7, 10, "3) Bouton (Enocean)");
        mvprintw(9, 10, "0) Quitter");
        
        mvprintw(12, 10, "Selectionnez une option : ");
        
        refresh();

        int ch = kb_getch();
        
        if (ch == '1') {
            menu_temperature();
        } else if (ch == '2') {
            menu_light();
        } else if (ch == '3') {
            menu_button();
        } else if (ch == '0') {
            break;
        }
        
        usleep(100000);
    }
}

void menu_temperature() {
    while (1) {
        erase();
        attron(COLOR_PAIR(1) | A_BOLD);
        mvprintw(2, 5, "=== MENU TEMPERATURE ===");
        attroff(COLOR_PAIR(1) | A_BOLD);
        
        mvprintw(4, 5, "Seuil actuel : %d C", seuil_temp);
        mvprintw(6, 5, "S) Changer le seuil");
        mvprintw(7, 5, "0) Retour");
        
        refresh();

        int ch = kb_getch();
        
        if (ch == 'S' || ch == 's') {
            erase();
            mvprintw(5, 5, "Entrez le nouveau seuil (°C) : ");
            echo();
            curs_set(1);
            refresh();
            scanw("%d", &seuil_temp);
            curs_set(0);
            noecho();
        } else if (ch == '0') {
            return;
        }
        
        usleep(200000);
    }
}

void menu_button() {
    erase();
    attron(COLOR_PAIR(1) | A_BOLD);
    mvprintw(2, 5, "=== MENU BOUTON (Enocean) ===");
    attroff(COLOR_PAIR(1) | A_BOLD);
    
    mvprintw(4, 5, "Lecture du port serie en attente de signal Enocean...");
    mvprintw(5, 5, "Appuyez sur un bouton Enocean...");
    mvprintw(7, 5, "Derniere pression : En attente...");
    
    mvprintw(15, 5, "0) Retour");
    
    refresh();
    
    // Clear any pending input
    flushinp();
    
    while (1) {
        int ser = check_hardware_input(serial_fd);
        if (ser != -1) {
            mvprintw(7, 5, "Derniere pression : BOUTON DETECTE !                         ");
            refresh();
            usleep(500000); // Show message for 500ms
            mvprintw(7, 5, "Derniere pression : En attente...                          ");
            refresh();
        }
        
        if (kb_kbhit()) {
            int ch = kb_getch();
            if (ch == '0') return;
        }
        
        usleep(50000);
    }
}
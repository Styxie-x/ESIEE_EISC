


# Système "IOT" PC + Arduino




Ce projet est une solution domotique hybride permettant de superviser et de contrôler des périphériques matériels via une interface terminal (ncurses). 
Il assure la communication entre une station de base (PC sous Arch Linux), un microcontrôleur Arduino et des capteurs sans fil Enocean.


- [Système "IOT" PC + Arduino](#système-iot-pc--arduino)
  - [📋 Structure du Projet](#-structure-du-projet)
  - [🛠️ Technologies \& Compétences](#️-technologies--compétences)
  - [🔌 Connectivité](#-connectivité)
  - [📥 Installation \& Build](#-installation--build)
    - [Prérequis](#prérequis)
    - [Application PC](#application-pc)
    - [Firmware Arduino](#firmware-arduino)
  - [📖 Documentation](#-documentation)
  - [👤 Auteur(e)](#-auteure)



## 📋 Structure du Projet
```bash
.
├── Prog_Arduino/     # Firmware pour ATmega328P (PlatformIO avec Arduino.h)
│   ├── src/
│   │   └── main.cpp  # Gestion LED et capteur thermique interne
│   ├── platformio.ini
│   └── README.md
└── Prog_Pc/          # Application de contrôle C++ / Ncurses
    ├── build.sh      # Script de compilation automatisé
    ├── build_and_run.sh  # Script de compilation + exécution
    ├── CMakeLists.txt
    ├── lib/          # Modules : App (UI), Button (Série), Led, Sensor
    ├── src/          # Point d'entrée (main.cpp)
    └── README.md
```

## 🛠️ Technologies & Compétences

**Système :** Développé sur Arch Linux (compatible avec distributions Linux standards).

**Langages :** 
- C++17 pour l'application PC
- C pour le firmware Arduino

**Interface :** Ncurses pour l'interface graphique en mode texte (TUI).

**Hardware :** 
- Arduino Nano (ATmega328P)
- Dongle USB Enocean (TCM310)
- LED et capteur de température interne ATmega328P ou rand valeurs de temperature (pas de capteur disponible).

**Build System :** 
- CMake pour l'application PC
- PlatformIO pour le firmware Arduino

**Outils :** Scripting Bash pour l'automatisation de la compilation et la gestion de la structure.

## 🔌 Connectivité

- **Liaison Série Arduino :** 9600 baud sur `/dev/ttyUSB1`
- **Liaison Série Enocean :** 57600 baud sur `/dev/ttyUSB0`
- **Protocole :** Communication ASCII entre PC et Arduino

## 📥 Installation & Build

### Prérequis

```bash
# Arch Linux / Manjaro
sudo pacman -S base-devel cmake ncurses platformio

# Ubuntu / Debian
sudo apt-get install build-essential cmake libncurses-dev platformio
```

### Application PC

```bash
cd Prog_Pc
chmod +x build.sh
chmod +x build_and_run.sh
./build_and_run.sh
```

Le binaire exécutable sera généré dans le dossier `build/PcArduino` et le programme se lancera automatiquement.

### Firmware Arduino

```bash
cd Prog_Arduino
platformio run --target upload
```
Ou via VS-Code avec extension PlatformIO.

Ou via l'IDE Arduino Standard en sélectionnant la carte "Arduino Nano" et le port approprié.

## 📖 Documentation

- [README Application PC](Prog_Pc/README.md) - Détails de l'interface et des modules
- [README Firmware Arduino](Prog_Arduino/README.md) - Spécifications techniques du microcontrôleur
- [Aide Structure Projet](Prog_Pc/docs/project_structure_help.md) - Guide d'organisation du code

## 👤 Auteur(e)
- [Styxie-x](https://github.com/Styxie-x)<br>
- [F-arx / Fabio](https://github.com/F-arx)


Projet développé dans le cadre de la formation ESIEE EISC E3 S1.


**IA Generatives utiilisées : Claude 4.5 , Gemeni**

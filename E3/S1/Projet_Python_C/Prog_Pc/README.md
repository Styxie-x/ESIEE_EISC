

# Application PC - Interface Ncurses 💻


## Fonctionnement

Application C++17 / (code compatible C) assurant la communication avec l'Arduino et l'interface utilisateur en mode texte (TUI) via Ncurses.

**Objectif :** Contrôler une LED via l'Arduino et afficher les données de température sur une interface utilisateur interactive.

**Fonctionnalités principales :**
- Interface texte interactive (Ncurses) pour contrôler les LEDs (ON / OFF / BLINK)
- Lecture des données de température depuis les capteurs Arduino
- Communication série bidirectionnelle (Arduino à 9600 baud, Enocean à 57600 baud)
- Affichage dynamique des états et des données en temps réel

## Étapes d'Exécution

### Démarrage Rapide
```bash
cd Prog_Pc
./build_and_run.sh
```

Cette commande compile et exécute l'application automatiquement dans le terminal. (voir le fichier build_and_run.sh pour plus de details).

### Utilisation de l'application
1. L'interface affiche un menu principal au démarrage
2. Naviguez avec les touches du clavier
3. Contrôlez la LED (activer, désactiver, clignotement)
4. Consultez les données de température en temps réel
5. Les données reçues de l'Arduino s'affichent automatiquement

## Architecture Modulaire

### Modules Principaux

#### 1. **App Module** (`lib/app/`)
- Gestion de l'interface utilisateur Ncurses
- Affichage du menu principal et des sous-menus
- Rafraîchissement dynamique de l'écran
- Gestion des événements clavier

#### 2. **Button Module** (`lib/button/`)
- Interfaçage série (Serie) avec l'Arduino
- Ouverture/fermeture des ports série
- Envoi et réception de commandes
- Gestion des débits (9600 baud pour Arduino, 57600 pour Enocean)

#### 3. **LED Module** (`lib/led/`)
- Commandes de contrôle LED
- Gestion des états (ON, OFF, BLINK)
- Synchronisation avec l'Arduino

#### 4. **Sensor Module** (`lib/sensor/`)
Pour l'instant le code menu temperature reste en app.cpp et app.h pour melieur gestion.


## Protocole de Communication

### Commandes Sortantes (PC → Arduino)
```
LED:ON          → Active la LED en continu
LED:OFF         → Éteint la LED
LED:BLINK       → Mode clignotement automatique (500ms)
TEMP:XX         → Définit le seuil de température (XX = valeur en °C)
```

### Télémétrie Entrante (Arduino → PC)
```
TEMP:XX         → Température actualisée (transmission toutes les 500ms)
```

## Dépendances

- **CMake** ≥ 3.10
- **Ncurses** (libncurses-dev)
- **GCC/Clang** avec support C++17

## Compilation & Exécution

### Compilation Manuelle (optionnel pour debug)
```bash
cd Prog_Pc
mkdir -p build
cd build
cmake ..
make
./PcArduino
```


## Structure des Fichiers

```
Prog_Pc/
├── CMakeLists.txt          # Configuration CMake
├── build.sh                # Script de compilation uniquement
├── build_and_run.sh        # Script compilation + exécution
├── src/
│   └── main.cpp           # Point d'entrée principal
├── lib/
│   ├── app/               # Gestion interface Ncurses
│   │   ├── app.h
│   │   └── app.cpp
│   ├── button/            # Gestion communication série
│   │   ├── button.h
│   │   └── button.cpp
│   ├── led/               # Contrôle LED
│   │   ├── led.h
│   │   └── led.cpp
│   └── sensor/            # Traitement capteurs
│       ├── Temperature.h
│       └── Temperature.cpp
└── docs/
    └── project_structure_help.md
```

## Configuration des Ports Série

Dans `main.cpp`, les ports sont configurés comme suit :
- **Arduino :** `/dev/ttyUSB1` à 9600 baud
- **Enocean :** `/dev/ttyUSB0` à 57600 baud

Modifiez ces chemins si vos périphériques sont connectés sur d'autres ports.

## Utilisation

1. Démarrer l'application : `./build_and_run.sh`
2. L'interface affiche un menu principal
3. Naviguez avec les touches du clavier
4. Contrôlez la LED et consultez les données de température
5. Les données reçues de l'Arduino s'affichent en temps réel

## Dépannage

**Erreur de port série :** Vérifiez que l'Arduino et le dongle Enocean sont connectés aux bons ports USB.

**Permission refusée :** Ajoutez votre utilisateur au groupe `dialout` :
```bash
sudo usermod -a -G dialout $USER
```

**Ncurses manquant :** Installez libncurses-dev via votre gestionnaire de paquets.

## Références

- [Ncurses Documentation](https://tldp.org/HOWTO/NCURSES-Programming-HOWTO/)
- [CMake Documentation](https://cmake.org/documentation/)
- [Enocean Protocol](https://www.enocean.com/)



**Copyright (C) 2026 [Styxie-x](https://github.com/Styxie-x)**  
**License:** GNU General Public License version 3 (GPL v3)

You should have received a copy of the GNU General Public License along with this project. If not, see [https://www.gnu.org/licenses/](https://www.gnu.org/licenses/).


# Firmware Arduino (ATmega328P)

Firmware dédié à l'acquisition de données environnementales et à l'exécution de commandes actionneurs pour Arduino Nano.

## Notes Importantes

Capteur de Température : Le groupe n'avait pas de capteur externe disponible. Deux alternatives ont été explorées :
- Température aléatoire (implémentation précédente réussie)
- Capteur interne ATmega328P (implémentation actuelle, moins stable mais fonctionnel)

Le capteur thermique interne reste instable en raison des variations de température du microcontrôleur lui-même. Pour une application de production, un capteur externe (DHT22, DS18B20, etc.) serait recommandé.

## Détails Techniques Hardware

### Capteur de Température Interne

- **Canal ADC :** Canal 8 de l'ADC (capteur thermique interne)
- **Référence de tension :** 1.1V interne (configuration registres : `REFS1 | REFS0`)
- **Résolution :** 10 bits
- **Formule de conversion :** `Temperature (°C) = (raw_adc - 324) / 1.22`
  - Note : Ajustement empirique basé sur l'ATmega328P du prototype
- **Fréquence d'acquisition :** Toutes les 500ms
- **Stabilité :** À calibrer selon le microcontrôleur spécifique

### LED
- **Broche :** D3 (Pin 3)
- **Type :** Sortie digitale (GPIO)
- **Modes :** 
  - ON : LED allumée en continu
  - OFF : LED éteinte
  - BLINK : Clignotement automatique (500ms d'intervalle)

### Liaison Série
- **Débit :** 9600 baud
- **Parité :** Aucune
- **Bits de données :** 8
- **Bits d'arrêt :** 1
- **Format :** ASCII

## Protocole de Communication (ASCII)

### Télémétrie (Sortant vers PC) [PC en réception]
```
TEMP:XX        → Transmission automatique de la valeur thermique
                 XX = valeur en Celsius (par ex : TEMP:28)
                 Intervalle : 500ms
```

### Commandes (Entrant depuis PC) [Arduino en réception]
```
LED:ON         → Allume la LED de manière permanente
LED:OFF        → Éteint complètement la LED
LED:BLINK      → Mode clignotement automatique (période 500ms)
TEMP:XX        → Définit le seuil de température de consigne
                 XX = nouvelle valeur (par ex : TEMP:30)
```

## Configuration PlatformIO

Fichier **platformio.ini**
```ini
[env:nanoatmega328]
platform = atmelavr
board = nanoatmega328new
framework = arduino
upload_speed = 57600
```

**Spécifications :**
- **Plateforme :** `atmelavr` (Atmel AVR)
- **Carte :** `nanoatmega328new` (Arduino Nano v3 avec ATmega328P)
- **Framework :** Arduino
- **Vitesse de téléversement :** 57600 baud

## Algorithme Principal

### Initialisation
```
├─ Configurer LED_PIN (D3) en sortie
├─ Initialiser série à 9600 baud
└─ Initialiser variables d'état
```

### Boucle Principale (Non-bloquante)
```
├─ Toutes les 500ms :
│  ├─ Lire température interne
│  └─ Transmettre TEMP:XX au PC
├─ Gérer l'état LED :
│  ├─ Si mode BLINK : alterner OUT_LOW/OUT_HIGH
│  └─ Sinon : appliquer l'état actuel
└─ Vérifier réception de commandes :
   ├─ Parser la commande reçue
   └─ Mettre à jour LED ou seuil TEMP
```

Avantage clé : Architecture non-bloquante utilisant `millis()` pour la gestion du temps (pas de `delay()` qui bloquerait l'exécution).

## Brochage Arduino Nano

| Fonction | Broche | Pin Arduino |
|----------|--------|-------------|
| LED | D3 | 3 |
| RX (Réception) | RX | 0 |
| TX (Transmission) | TX | 1 |
| GND | GND | GND |
| +5V | VCC | VCC |


## Compilation & Téléversement

### Avec PlatformIO (Recommandé)
```bash
cd Prog_Arduino
platformio run --target upload
```

### Avec Arduino IDE
1. Ouvrir `src/main.cpp`
2. Sélectionner : **Outils → Carte → Arduino Nano**
3. Sélectionner : **Outils → Processeur → ATmega328P (Old Bootloader)**
4. Sélectionner le port COM approprié
5. Cliquer sur **Téléverser**

## Variables Globales Principales

Ces variables gèrent l'état global du firmware :

| Variable | Type | Description |
|----------|------|-------------|
| `led_mode` | enum | État courant de la LED (OFF, ON, BLINK) |
| `led_output_state` | bool | État physique de la broche LED |
| `temp_threshold` | int | Seuil de température de consigne |
| `last_blink_time` | unsigned long | Timestamp du dernier changement BLINK |
| `last_temp_send_time` | unsigned long | Timestamp du dernier envoi TEMP |

## Constantes Configurables

```cpp
const uint8_t LED_PIN = 3;                    // Broche LED
const unsigned long BLINK_INTERVAL_MS = 500;  // Intervalle clignotement
const unsigned long TEMP_SEND_INTERVAL_MS = 500;  // Intervalle envoi température
```

Modifiez ces valeurs pour adapter le comportement du firmware à vos besoins.

## Tests & Validation

### Test Série
Utilisez un moniteur série (9600 baud) pour :
1. Vérifier la réception des `TEMP:XX` toutes les 500ms
2. Envoyer `LED:ON`, `LED:OFF`, `LED:BLINK` et observer les changements en temps réel
3. Envoyer `TEMP:25` pour modifier le seuil

## Dépannage

| Problème | Cause | Solution |
|----------|-------|----------|
| Pas de température | Configuration ADC incorrecte | Vérifiez canal ADC 8 et référence 1.1V dans `read_internal_temperature()` |
| Valeurs erratiques | Capteur interne instable | Calibrer la formule selon votre ATmega328P spécifique |
| LED non réactive | Broche D3 non configurée | Vérifiez `pinMode(LED_PIN, OUTPUT)` en setup |
| Communication échouée | Débit incorrect ou câble USB | Vérifiez 9600 baud et connexion USB de l'Arduino |
| Téléversement échoue | Bootloader incompatible | Sélectionnez "ATmega328P (Old Bootloader)" en Arduino IDE |

## Ressources

- [ATmega328P Datasheet](https://ww1.microchip.com/en-us/product/ATmega328P)
- [Arduino Reference](https://www.arduino.cc/reference/en/)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [Capteurs de Température pour Arduino](https://create.arduino.cc/projecthub/Arduino_Genuino/deep-dive-into-the-arduino-uno-temperature-sensor-c6a1e5)

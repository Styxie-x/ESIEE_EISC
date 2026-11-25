# TP capteurs sans fil sur Arduino Uno

Le DHT22 mesure la température et l’humidité. L’émetteur regroupe les deux valeurs dans une trame nRF24. Le récepteur envoie une ligne par trame sur USB à 115 200 bauds et le PC peut l’enregistrer. Le mode radio par défaut est `PowerDown` entre deux envois.

## Démarrage sur Linux

1. Extraire l’archive et ouvrir un terminal dans le dossier `Capteur`. `Emmeteur` et `Recepteur` sont deux projets PlatformIO autonomes. On peut ouvrir ou copier chaque dossier séparément.
2. Vérifier les schémas de câblage du [rapport](Rapport.md). Brancher une carte à la fois et identifier son port avec `pio device list`. Avec deux cartes branchées, les ports peuvent par exemple être `/dev/ttyACM0` et `/dev/ttyACM1`. Leur ordre dépend du branchement.
3. Compiler et téléverser avec PlatformIO. Remplacer les ports des exemples par ceux relevés à l’étape précédente.

```bash
pio run -d Emmeteur -t upload --upload-port /dev/ttyACM0
pio run -d Recepteur -t upload --upload-port /dev/ttyACM1
```

Sous VS Code avec PlatformIO, on peut aussi ouvrir chaque dossier de carte et utiliser **Upload**. Pour contrôler la réception, ouvrir le moniteur du récepteur à 115 200 bauds. Une ligne a la forme `numero,temperature_c,humidite_pct`. Fermer le moniteur série avant d’exécuter l’enregistreur. Si aucune mesure n’arrive, mettre temporairement `TX_LOGS = true` dans `Emmeteur/src/main.cpp` et ouvrir le moniteur **de l’émetteur** pour voir les erreurs de lecture ou les échecs d’acquittement.

## Enregistrement et courbes

Fermer le moniteur série, puis lancer cette commande dans le dossier contenant `enregistrer.sh`. Adapter le port au récepteur.

```bash
bash enregistrer.sh /dev/ttyACM1 mesures_air.csv
```

Le récepteur produit les lignes CSV avec `Serial.print()`. Le script Bash lit le port série, ajoute le temps écoulé en secondes et enregistre le fichier. Les mesures s’affichent aussi au terminal. Arrêter avec **Ctrl+C**. Pour refaire une capture, choisir un nouveau nom de fichier.

Ouvrir `mesures_air.csv` dans LibreOffice Calc. Choisir la virgule comme séparateur et l’anglais (États-Unis) comme langue d’import pour lire les points décimaux. Insérer deux graphiques de type **XY**, avec `temps_s` en abscisse et une grandeur par graphique.

## Essais de consommation

Reprendre les scénarios du [tableau d’essais](mesures_energie.csv). Le canal et le débit sont définis en haut des deux fichiers `src/main.cpp`. Après modification de l’un de ces deux paramètres, faire le même changement dans les deux codes puis téléverser les deux programmes. Le mode de repos, la période et la puissance d’émission se règlent dans le `main.cpp` de l’émetteur. Laisser `TX_LOGS = false` et éviter les commandes manuelles pendant les mesures.

Saisir les relevés dans `mesures_energie.csv`. Les formules de puissance, d’énergie et de gain figurent dans le [rapport](Rapport.md). Avec le tableau ouvert dans Calc, les colonnes **H à L** sont respectivement la tension en V, le courant au repos et en activité en mA, et la durée au repos et en activité en ms. Pour la ligne 2, utiliser les formules `=H2*I2` et `=H2*J2` pour les puissances de phase en mW, `=H2*(I2*K2+J2*L2)/1000` pour l’énergie par cycle en mJ, et `=H2*(I2*K2+J2*L2)/(K2+L2)` pour la puissance moyenne en mW.


# # VFS Shell - Virtual File System

Un interpréteur de commandes (Shell) interactif gérant un système de fichiers virtuel entièrement en mémoire (RAM), avec une interface textuelle (TUI) stylisée.

---

## 🛠️ Spécifications de l'Architecture des Fichiers

L'arbre des fichiers du projet doit répartir les responsabilités de la manière suivante :

* **`inc/vfs.h` & `src/vfs.c` (Le Cœur)** : Gestion des structures de données (nœuds, dossiers, fichiers) et manipulation brute de la mémoire (allocation, arborescence, libération récursive).
* **`inc/tui.h` & `src/tui.c` (L'Affichage)** : Gestion des codes d'échappement ANSI pour colorer le terminal, afficher la barre d'état et le prompt dynamique.
* **`inc/shell.h` & `src/shell.c` (L'Interpréteur)** : Découpage des commandes utilisateur (`fgets`, tokenization) et routage vers les bonnes actions en modifiant le dossier courant.
* **`src/main.c` (Le Moteur)** : Initialisation de la racine du système, lancement de la boucle interactive (REPL), et nettoyage global à la fermeture.

---

## 📋 Feuille de Route de l'Implémentation (Étape par Étape)

> [!IMPORTANT]
> Ne passe à l'étape suivante que lorsque la précédente compile sans aucun *warning* (`-Wall -Wextra -Werror`) et s'exécute parfaitement.

### 🔹 Étape 1 : Les Fondations Memoire (vfs)

L'objectif est de pouvoir créer et détruire la racine du système proprement.

* **Dans `inc/vfs.h`**:
* Définir un `enum` pour le type de nœud (`DIR` ou `FILE`).
* Définir une `struct Node` contenant : le nom (chaîne de caractères), le type (`enum`), un pointeur vers le nœud `parent`, un pointeur `char* content` (pour les fichiers), et un tableau dynamique de pointeurs `struct Node children` (pour les dossiers) accompagné de sa taille actuelle (`size_t count`) et de sa capacité maximale (`size_t capacity`).


* **Dans `src/vfs.c**` :
* Implémenter une fonction de création de nœud qui alloue la structure, copie le nom, lie le parent, et initialise les pointeurs à `NULL`.
* Implémenter une fonction de libération récursive : si c'est un fichier, elle `free` le contenu ; si c'est un dossier, elle boucle sur le tableau `children` pour libérer chaque enfant en cascade, puis `free` le tableau lui-même. Enfin, elle libère le nœud lui-même.


* **Dans `src/main.c**` :
* Créer la racine (`"/"`), la libérer immédiatement et vérifier qu'il n'y a aucun crash.



### 🔹 Étape 2 : Navigation et Création (vfs + shell)

L'objectif est d'ajouter des éléments dans l'arborescence et de s'y déplacer.

* **Dans `src/vfs.c**` :
* Implémenter une fonction pour ajouter un enfant à un dossier (gérer le redimensionnement du tableau `children` avec `realloc` si `count == capacity`).
* Implémenter une fonction de résolution de chemin : elle prend une chaîne (ex: `dossier1/dossier2`) et renvoie le pointeur du nœud cible en parcourant les listes d'enfants.


* **Dans `src/shell.c**` :
* Implémenter le découpage de l'entrée utilisateur en arguments (`argc`, `argv`).
* Implémenter la commande `mkdir <nom>` (crée un dossier enfant).
* Implémenter la commande `ls` (affiche les noms des enfants du dossier actuel).
* Implémenter la commande `cd <chemin>` : elle prend un **pointeur de pointeur (`Node cwd`)** pour modifier directement le dossier actuel de l'utilisateur dans la boucle principale. Gérer `cd ..` en remontant vers le parent.



### 🔹 Étape 3 : Manipulation de Fichiers (vfs + shell)

L'objectif est de pouvoir lire et écrire des fichiers virtuels.

* **Dans `src/vfs.c**` :
* Implémenter une fonction d'écriture de fichier qui alloue ou réalloue le pointeur `content` avec le texte fourni.


* **Dans `src/shell.c**` :
* Implémenter la commande `touch <nom>` (crée un nœud de type fichier vide).
* Implémenter la commande `write <nom> "<texte>"` (écrit du texte dans le fichier).
* Implémenter la commande `cat <nom>` (affiche le contenu textuel du fichier).
* Implémenter la commande `rm <nom>` : supprime un fichier ou un dossier vide, retire son pointeur du tableau `children` du parent, décale les éléments restants pour boucher le trou, et libère sa mémoire.



### 🔹 Étape 4 : Habillage Visuel (tui)

L'objectif est de rendre le projet agréable et interactif à l'utilisation.

* **Dans `src/tui.c**` :
* Implémenter une fonction qui génère le prompt à chaque ligne en utilisant des codes ANSI (ex: texte en vert ou fond inversé pour simuler une interface système).
* Formatage propre des sorties (ex: ajouter un slash `/` à côté des dossiers lors d'un `ls`).



---

## 🚀 Commandes attendues dans le Shell final

* `ls` : Liste le contenu du dossier courant.
* `cd <destination>` : Change de dossier (gère `..` et les chemins relatifs).
* `mkdir <nom>` : Crée un sous-dossier.
* `touch <nom>` : Crée un fichier vide.
* `write <nom> "<message>"` : Écrit dans un fichier.
* `cat <nom>` : Affiche le contenu d'un fichier.
* `rm <nom>` : Supprime un fichier ou un dossier vide.
* `clear` : Efface l'écran du terminal.
* `exit` : Quitte le programme proprement (sans fuite mémoire).
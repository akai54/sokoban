# Résolution de problèmes de Sokoban

Le but de ce projet est de développer un programme capable de résoudre des problèmes de Sokoban en utilisant les algorithmes appropriés. Il existe deux versions du projet: une en C/C++ et une autre en Prolog.

## Installation

1. Extraire le fichier sokoban-2023.tgz
2. Aller dans le répertoire sokoban-2023
3. Le répertoire src contient le début du TP, le répertoire Screens-1 (respectivement Screens-2)
   contient les problèmes à 1 joueur (respectivement 2 joueurs)
4. Dans src: la commande "make" crée l'executable r0, lancer "./r0 ../Screens-1/screen-0.txt"
   permet de lire et afficher le problème considéré

### Niveaux

Dans tous les problèmes :
Les caisses sont des "$"
Les cases objectifs sont des "."
Les murs sont des "#"

Dans le programme prolog : 
Les caisses sont des "$"
Les cases objectifs sont des "g" 
Les murs sont des "#"
Les cases vides sont des "o"
### Usage - Prolog 

Afin de pouvoir tester le programme codé en prolog, il est nécessaire d'installer l'interpréteur [swipl](https://www.swi-prolog.org/)

Si cela est déjà fait, ouvrez un terminal dans le dossier courant, entrez dans l'interpreteur, ouvrez le ficher "soko.pl" et exécuter la commande suivante: 
"?- solver.". 

## TODO

- [x] Analyse du problème: comprendre les conditions de déplacement des joueurs et des caisses.
- [x] Implementation 1 joueur Prolog.
- [x] Implementation 1 joueur C/C++.
- [ ] Implementation 2 joueurs Prolog.
- [ ] Implementation 2 joueurs C/C++.
- [x] Rapport final.

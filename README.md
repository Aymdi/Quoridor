# Projet Quoridor

- [Sujet du projet](https://www.labri.fr/perso/renault/working/teaching/projets/2020-21-S6-C-Quoridor.php)

- [Page sur thor](https://thor.enseirb-matmeca.fr/ruby/projects/projetss6-quor)

# Présentation

Ce dossier contient les fichiers sources nécessaires à la compilation d'un
serveur de jeu et de différents clients capables de jouer une partie de Quoridor. 

# Compilation

Le serveur ainsi que les clients utilisent la version 2.6 de la bibliothèque GSL.
Celle-ci est disponible à cette [adresse](https://www.gnu.org/software/gsl/).
La compilation est facilitée avec l'outil make. Les commandes suivantes
permettent de compiler le projet :
$ GSL_PATH=/chemin/vers/gsl/ make 
$ GSL_PATH=/chemin/vers/gsl/ make install

# Utilisation

La commande make install crée un exécutable server dans le répertoire install,
ainsi que divers clients. Celui prend en paramètre deux bibliothèques dynamiques
(deux clients), par exemple:
$ LD_LIBRARY_PATH=/chemin/vers/gsl/lib:. ./install/server install/rush.so install/greedy.so

Il est possible de préciser la taille et la forme du plateau avec les options
-m [taile] et -t [forme]. Les formes de plateau disponibles sont:
c (plateau carré)
h (plateau en H)
s (plateau serpent)
t (plateau torique)

# Tests

Il est possible de vérifier les tests exécutés avec la commande suivante:
$ GSL_PATH=/chemin/vers/gsl make test
Celle-ci crée un exécutable alltests dans le répertoire install.

# Bonus

L'option -i permet d'afficher le déroulement de la partie dans le terminal.
Un client interactif peut être compilé avec la commande:
$ GSL_PATH=/chemin/vers/gsl make interactive
Celui-ci peut être fourni à l'exécutable server et permet à l'utilisateur
d'affronter les différents clients.
Pour se déplacer, entrer le numéro d'un sommet.
Pour placer un mur, entrer le numéro d'un sommet ainsi que la lettre h (pour
placer un mur horizontal sous le sommet) ou v (pour placer un mur vertical à
droite du sommet).

# TP_Vers_PoseVerrous

Module "Systèmes d'Exploitation"

Pour decompresser ce fichier vous pouvez utiliser les commandes :

gunzip TP_Vers.tar.gz
tar xvf TP_Vers.tar

Vous devez obtenir l'arborescence suivante :

./Commun:
commun.h case.h  coord.{h,c} test_coord.c jeu.{h,c} test_jeu.c 

./Terrain:
Makefile         init_fich_terrain.c   terrain.c        terrain.h        test_terrain.c

./Vers:
Makefile         test_vers.c        vers.c             vers.h

./Fichiers

./Verrous 
Makefile	VerDeTerre.c	vision.c	lance_vers.sh

./Signaux
Makefile	verDeTerre.c	aire.c	lance_vers.sh	signaux.h



Repertoire Commun --> ressources partagees par tous les modules 
commun.h : definition des constantes communes a tous les modules

case.h : definition d'une case du terrain,
utilisee dans la composition du terrain et pour la marque du ver

coord.{c,h} : TDA des coordonnes utilisees dans TDA terrain et TDA ver
test_coord.c : test des methodes du TDA coord

jeu.{c,h} : fonctions utilisant conjointement les TDA terrain et ver
test_jeu.c : test des fonctions du jeu

Repertoire Terrain --> TDA terrain 
terrain.{h,c} : definition et realisation du TDA terrain
init_fich_terrain.c : utilitaire de creation d'un terrain de jeu 
test_terrain.c : programme de test des primitives du TDA terrain

Repertoire Vers --> TDA des Vers
ver.{c,h} : defintion et realisation du TDA ver
test_ver.c : test des methodes du TDA ver

vers.{h,c} : definition et realisation d'une liste de vers
test_vers.c : test des methodes de la liste des vers

Reperoire Fichiers --> emplacement propose pour les fichiers
On peut y stocker le fichier terrain dans lequel evoluent les vers 
et, si vous l'utilisez, le fichier de sauvegarde de la liste des vers

Repertoire Verrous --> Repertoire de travail pour la solution du jeu des vers avec verrous
verDeTerre.c : squelette (capture des parametres) du programme d'un ver 
vision.c : squelette (capture des parametres) du programme d'affichage du terrain
Makefile : fichier de regles a utliser avec "make" pour compiler la solution par verrous si vous respectez
les noms ci-dessus
lance_vers.sh : utilitaire pour lancer simultanement plusieurs vers  (utile pour la validation du TP) 

Repertoire Signaux --> Repertoire de travail pour la solution du jeu des vers avec signaux
verDeTerre.c : squelette (capture des parametres) du programme d'un ver 
aire.c : squelette (capture des parametres) du programme de l'aire ecrivant dans le terrain
Makefile : fichier de regles a utliser avec "make" pour compiler la solution par signaux si vous respectez
les noms ci-dessus
lance_vers.sh : utilitaire pour lancer simultanement plusieurs vers  (utile pour la validation du TP) 
signaux.h : fichier contenant les noms des signaux utilises dans aire et verDeTerre (facultatif)



La compilation peux s'effectuer par :

make -f Makefile all

- soit au niveau de la racine de votre TP (repertoire TP_Vers) pour compiler tout le projet (a faire ua moins une fois apres le telechargement 
de l'archive) 

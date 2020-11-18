#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>


/*--------------------*
 * Main demon
 *--------------------*/
int
main( int nb_arg , char * tab_arg[]){
  /* Parametres */
  char fich_terrain[128] ;
  char nomprog[256] ;
  char chemin[512];
     /*----------*/

  /* Capture des parametres */
  if( nb_arg != 2 ){
      fprintf( stderr , "Usage : %s <fichier terrain>\n",
               tab_arg[0] );
      exit(-1);
  }

  strcpy( nomprog , tab_arg[0] );
  strcpy( fich_terrain , tab_arg[1] );


  printf("\n%s : ----- Debut de l'affichage du terrain ----- \n", nomprog );

  /***********/
  /* A FAIRE */
  /***********/

  /*
  On opte pour les verrous internes car on a besoin ici d'une notion de type de verrou (est-ce en mode lecture ou en mode ecriture?)
  Le programme vision.c s'occupe d'afficher tout ce qui est dans le fichier partagé à chaque modification du fichier.
  Il fera donc une lecture intégrale en vérifiant TOUS les octets du fichier.

  on sait que OPEN est atomique, le principe du programme est donc de vérifier sur le descripteur du fichier, la date de modif
  et de voir si elle a changé, pour effectuer un OPEN lui-même à son tour et pose son verrou en mode Lecture
  La pose du verrou se fera sur le fichier entier, puisqu il fait une lecture de tout le fichier p pouvoir l afficher

  verrou interne car le verrou externe bloque tout le fichier et on a pas de notion de type de verrou avec un verrou externe OR, ici,
  on a fichier qui est la base d'un jeu à temps indeterminé avec plusieurs programmes qui ouvrent le fichier en mode ecriture et qui doivent pouvoir éditer
  ce fichier ci
  en plus de ça, on n a pas de portée, donc on peut pas vérifier sur les cases du ver, si il peut ou pas "ecrire" sa marque.
  Le but étant de verrouiller les 7 cases autour du ver (toutes les cases - 1 qui vient de son corps)

  */

  int descrip_fich;
  struct flock verrou;
  descrip_fich = open(fich_terrain,0666);

  verrou.l_type = F_RDLCK;  /*verrou partagé*/
  verrou.l_whence = 0; /*sur tout le fichier*/
  verrou.l_start = 0;
  verrou.l_len = 0;
                        /*Specifying 0 for l_len has the special
                         meaning: lock all bytes starting at the location specified by
                         l_whence and l_start through to the end of file, no matter how large
                         the file grows*/


  fcntl(descrip_fich,F_SETLK,&verrou);  /* pose du verrou non bloquant */

  /*creation de la structure qui peut accéder aux détails du fichier, comme la dernière date de modification...*/
  struct stat infos_file;

  sprintf(chemin,"../Terrain/%s",fich_terrain);
  stat(chemin,&infos_file);         /*initialise le buffer infos_fich avec les données du fichier */
  printf("%s => num ino %lu la taille du fich %ld nb de \nliens:%lu\n",chemin,infos_file.st_ino,infos_file.st_size,
  infos_file.st_nlink );
  sleep(1);
  time_t last_modif=infos_file.st_mtime;
  int cpt=10;
  while(cpt > 0){

    stat(chemin,&infos_file);

    if(infos_file.st_mtime != last_modif){
      last_modif = infos_file.st_mtime ;
      terrain_afficher(descrip_fich);
    }

    cpt--;
  }


  printf("\n%s : --- Arret de l'affichage du terrain ---\n", nomprog );

  exit(0);
}

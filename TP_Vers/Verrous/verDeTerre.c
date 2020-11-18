#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>
#include <vers.h>
#include <jeu.h>

int main( int nb_arg , char * tab_arg[] ){

     /* Parametres */
     char fich_terrain[128] ;
     case_t marque = CASE_LIBRE ;
     char nomprog[128] ;

     /*----------*/

     /* Capture des parametres */
     if( nb_arg != 3 )
     {
          fprintf( stderr , "Usage : %s <fichier terrain> <marque>\n",
                   tab_arg[0] );
          exit(-1);
     }

     if( strlen(tab_arg[2]) !=1 )
     {
          fprintf( stderr , "%s : erreur marque <%s> incorrecte \n",
                   tab_arg[0] , tab_arg[2] );
          exit(-1) ;
     }

     strcpy( nomprog , tab_arg[0] );
     strcpy( fich_terrain , tab_arg[1] );
     marque = tab_arg[2][0] ;


   /* Initialisation de la generation des nombres pseudo-aleatoires */
     srandom((unsigned int)getpid());

     printf( "\n\n%s : ----- Debut du ver %c (%d) -----\n\n ",
             nomprog , marque , getpid() );

     /***********/
     /* A FAIRE */
     /***********/
     /*
     -> chaque ver va devoir poser un verrou bloquant sur les cases disponibles autour de sa tete (donc 7 cases)
     -> il pose ce verrou que lorsqu il va ecrire sur le fichier et que les cases sont disponibles
     -> on appelle dans le terminal init_fich_terrain, on initilise le ver à travers le programme verDeTerre
     -> besoin de creer une structure ver dans ce fichier pour appeler le programme jeu.c juste après
      */
      int descrip_fich;

      char chemin[128] ;
      sprintf(chemin,"../Terrain/%s",fich_terrain);
      descrip_fich = open(chemin,O_RDWR , 0644 ); /*read and writing, car le descripteur a besoin de lire pour ensuite ecrire sa marque dans le fichier*/


      int nb_lig, nb_col;
      terrain_dim_lire( descrip_fich , &nb_lig , &nb_col ) ;/*initialisation aleatoire du nb de lignes et colonnes du terrain */
      printf("il y a %d lignes et %d colonnes adresse du descripteur %p\n",nb_lig, nb_col, &descrip_fich );

  /**/
      struct flock verrou;
      verrou.l_type = F_WRLCK;  /*verrou partagé car sinon il ne pourra pas coexister avec les autres verrous ? essayer avec F_RDLCK alors*/
      verrou.l_whence = SEEK_CUR; /*debut car on n a pas encore les coordonnées du ver*/
      verrou.l_start = 0;
      verrou.l_len = 0;
                                    /*Specifying 0 for l_len has the special
                                     meaning: lock all bytes starting at the location specified by
                                     l_whence and l_start through to the end of file, no matter how large
                                     the file grows*/

      while(fcntl(descrip_fich,F_SETLKW,&verrou) == -1 )
        printf("il y a un verrou\n");  /*&& ((errno== EDEADLK) || (errno== EACCES )) le compilateur ne retrouve pas errno et les constantes//  pose du verrou bloquant, tous les vers attendent une fois chacun son tour p ecrire dans le fichier*/


      /*initialisation du ver*/

      ver_t ver; /*la fonction ci dessous renverra la case libre, on place le descripteur dessus*/
      ver.marque=marque;
      jeu_ver_initialiser( descrip_fich, nb_lig,nb_col, &ver ); /*la fonction place le ver n importe où dans le fichier donc on pose un verrou sur tout le fichier, pour éviter qu il y ait deux ou plus de vers qui ecrivent sur le même morceau du fichier */
      printf(" x = %d et y = %d la position =%ld == la marque %c\n",ver.tete.x,ver.tete.y, ver.tete.pos ,ver.marque );
      terrain_marque_ecrire(descrip_fich,ver.tete,ver.marque);

      /*levée du verrou car écriture terminée*/
      verrou.l_type = F_UNLCK;
      fcntl(descrip_fich,F_SETLKW,&verrou);


      coord_t * liste_voisins = (coord_t *)NULL ; int nb_voisins = 0; /*pour que le ver puisse se balader*/
      int ind_libre = 0 ;


      while (ind_libre != -1) {

        sleep(5);
        verrou.l_type = F_WRLCK;  /*Creation d un verrou bloquant*/
        verrou.l_whence = SEEK_SET; /*-> 0*/
        verrou.l_len = 8 * CASE_TAILLE; /*portée = l_whence+l_start*/
        verrou.l_start = ver.tete.pos;
        while(fcntl(descrip_fich,F_SETLKW,&verrou) == -1)
            printf("deadlock\n" );  /*attente pour poser le verrou à l emplacement donné*/

        terrain_voisins_rechercher(descrip_fich,	ver.tete,	&liste_voisins,  &nb_voisins );  /*rempli la liste des voisins*/

        terrain_case_libre_rechercher( descrip_fich,liste_voisins,nb_voisins,&ind_libre );  /*recherche une place parmi les voisins de la tete du ver*/
        if( ind_libre != -1 ){
          terrain_marque_ecrire(descrip_fich,liste_voisins[ind_libre],ver.marque);

          ver.tete.pos = liste_voisins[ind_libre].pos;  /*la tete se déplace*/
          ver.tete.x= liste_voisins[ind_libre].x;
          ver.tete.y=liste_voisins[ind_libre].y;

        }
        free(liste_voisins); liste_voisins = NULL;

        /*levée du verrou car écriture terminée*/
        verrou.l_type = F_UNLCK;
        if(fcntl(descrip_fich,F_SETLKW,&verrou) != -1)
          printf("verrou posé\n");
      }






      /*levée du verrou pour que tous les autres vers puissent à leur tour mettre leur tete */
      verrou.l_type = F_UNLCK;
      fcntl(descrip_fich,F_SETLKW,&verrou);

      /*on rentre dans le cas général du jeu => le ver va se déplacer donc poser des verrous sur les octets qui sont autour de sa tete*/




      close(descrip_fich);

     printf( "\n\n%s : ----- Fin du ver %c (%d) -----\n\n ",
           nomprog , marque , getpid() );

     exit(0);
}

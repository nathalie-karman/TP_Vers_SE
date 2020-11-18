
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>
#include <vers.h>
#include <jeu.h>
#include <signaux.h>
/*
* fonction(s) auxiliaire(s)
*/


static void change_marque(case_t *marque){  /* change la marque du ver*/

    (*marque )+= 1;

}
/*
 * VARIABLES GLOBALES (utilisees dans les handlers)
 */

case_t marque ='A';  // les marques des vers
ver_t petit_ver;

int in_game;
int ind_libre = 0 ;
int ind_ver=-1;
coord_t * liste_voisins = (coord_t *)NULL ;
int nb_voisins = 0; /*pour que le ver puisse se balader*/
int nb_lig, nb_col;

/*descripteur du fichier terrain*/
int descrip_fich;
/*liste de vers*/
vers_t * liste_vers = (vers_t *) NULL ;

/*
 * HANDLER
 */

static void handler_ver (int sig, siginfo_t *pid_ver, void *context){



    ind_ver = vers_pid_seek(liste_vers,pid_ver->si_pid);
    //printf("%d indice du ver\n", ind_ver);
    show_game=1;
    if( ind_ver == -1){  // il n est pas dans la liste, on l ajoute
      ver_marque_set(&petit_ver,marque);

      ver_pid_set( &petit_ver ,pid_ver->si_pid);
      jeu_ver_initialiser(descrip_fich, nb_lig, nb_col, &petit_ver);
      vers_ver_add(liste_vers, petit_ver);

      change_marque(&marque); //on passe Ã  la marque suivante

    }else{  //on fait le deplacement des vers
          petit_ver =vers_ver_get(liste_vers  ,ind_ver);

          terrain_voisins_rechercher(descrip_fich,petit_ver.tete,&liste_voisins,&nb_voisins);
          terrain_case_libre_rechercher( descrip_fich,liste_voisins,nb_voisins,&ind_libre);  /*recherche une place parmi les voisins de la tete du ver*/


          if(ind_libre != -1 && (vers_nb_get(liste_vers) > 1) ){
                //terrain_afficher(descrip_fich);

                terrain_marque_ecrire(descrip_fich,liste_voisins[ind_libre],petit_ver.marque);

                petit_ver.tete.pos = liste_voisins[ind_libre].pos;  /*la tete se dÃƒÂ©place*/
                petit_ver.tete.x = liste_voisins[ind_libre].x;
                petit_ver.tete.y =liste_voisins[ind_libre].y;

                vers_ver_set(liste_vers ,ind_ver,petit_ver); //remise du ver avec ses nouv coordonnÃ©es dans liste

          }else{ //le ver est mort on envoie un signal

                if(vers_nb_get(liste_vers) == 1){
                  in_game=0;
                  printf("\n\n\n\n\nVer %c de (pid %d) est le ver gagnant\n\n\n\n\n",liste_vers->vers[0].marque,liste_vers->vers[0].pid);

                }else{
                  printf("\n--------> ver %c mort...\n", petit_ver.marque);
                  printf("pid %d\n", pid_ver->si_pid);
                }

                kill(pid_ver->si_pid,SIGUSR2);
                vers_ver_del(liste_vers,ind_ver);

          }

    }

          //printf("marque %c\n",marque);

}


int main( int nb_arg , char * tab_arg[] ){
      char Nom_Prog[256] ;
      char fich_terrain[128] ;
      pid_t pid_aire ;

      /*----------*/
      /*
       * Capture des parametres
       */
      strcpy( Nom_Prog , tab_arg[0] );

      if( nb_arg != 2 ){
          fprintf( stderr , "Usage : %s <fichier terrain>\n",
                   Nom_Prog );
          exit(-1);
      }

      strcpy( fich_terrain , tab_arg[1] );
      pid_aire = getpid() ;

      /* Affichage du pid pour les processus verDeTerre */
      printf( "\n\t-----------------.\n") ;
      printf( "--- pid %s = %d ---\n" , Nom_Prog , pid_aire ) ;
      printf(   "\t-----------------.\n\n") ;
      show_game=0;
      /* Initialisation de la generation des nombres pseudo-aleatoires */
      srandom((unsigned int)pid_aire);

      /*----------*/

      printf("\n\t----- %s : Debut du jeu -----\n\n" , Nom_Prog );

      /***********
       * A FAIRE *
       ***********/
       /*initialisation*/

       liste_vers = vers_new();
       descrip_fich = open(fich_terrain,O_RDWR); //ouverture du fichier en lecture et ecriture
       terrain_dim_lire(descrip_fich , &nb_lig , &nb_col ) ;/*initialisation du nb de lignes et colonnes du terrain */


      /*capture des signaux The sigaction()
       function allows the calling process to examine and/or specify the action to be associated with a specific signal.
       The argument sig specifies the signal; acceptable values are defined in <signal.h>.
      */
        struct sigaction act;
        sigemptyset(&act.sa_mask);
        act.sa_flags=SA_SIGINFO;  // permettra au handler d avoir 3 parametres au lieu de 1 dont un qui est le PID
        act.sa_sigaction = handler_ver; //act.sa_handler=handler_ver; car on a indiquÃ© que c etait SA_SIGINFO avec 2 parametres en plus

        in_game=1;

      do{
            sigaction(SIGUSR1,&act,NULL);
            if(pause()){   //attente passive du processus aire
              terrain_afficher(descrip_fich);
            }
          //  printf("%d\n",in_game );

        }while(in_game);  //vers_nb_get(liste_vers)


        printf("\n\n\t----- %s : Fin du jeu -----\n\n" , Nom_Prog );
        vers_destroy(&liste_vers );
        free(liste_voisins); liste_voisins = NULL;

        close(descrip_fich);

        exit(0);

}

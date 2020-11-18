#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <commun.h>
#include <terrain.h>
#include <vers.h>
#include <signaux.h>


/*
 * VARIABLES GLOBALES (utilisees dans les handlers)
 */
pid_t pid_ver ;
int boole;
/*
 * HANDLERS
 */

void handler(int sig){

        printf( "\n\n--- Arret ver [%d] ---\n\n" , pid_ver );
        boole=0;

}

int main( int nb_arg , char * tab_arg[]){
     char nomprog[128] ;
     pid_t pid_aire ;


     /*----------*/

     /*
      * Capture des parametres
      */
     /* - nom du programme */
     strcpy( nomprog , tab_arg[0] );

     if( nb_arg != 2 ) {
          fprintf( stderr , "Usage : %s <pid aire>\n",
                   nomprog );
          exit(-1);
     }

     /* - parametres */
     pid_aire = atoi( tab_arg[1] ) ;  //on donne le pid du fichier aire qui fera la liaison des vers sur le fichier partagÃ©


     /* Initialisation de la generation des nombres pseudo-aleatoires */
     srandom((unsigned int)getpid());

     pid_ver = getpid() ;
     printf( "\n\n--- Debut ver [%d] ---\n\n" , pid_ver );

     /***********
      * A FAIRE *
      ***********/

      struct sigaction act_ver;
      //je vais garder sigaction car POSIX et plus portable mais on aurait pu mettre signal
      act_ver.sa_flags= 0;  // pas besoin de grand chose ici, tout est gÃ©rÃ© par aire.c miste Ã  zero du flag
      act_ver.sa_handler = handler; //
      sigemptyset(&act_ver.sa_mask);
        //
          //
      boole=1;
      sigaction(SIGUSR2,&act_ver,NULL);

      while(boole){

          kill(pid_aire,SIGUSR1); //on envoie les signaux pour demander Ã  se deplacer
          sleep(5); // on gere l attente ici sinon trop de signaux envoyÃ©s au processus aire

      }
      /*initialisation du ver Ã faire dans le fichier aire.c */
      // creation d un signal pour chaque ver, ils attendent un signal pour mourir aussi? Ã  voir

      exit(0);
}

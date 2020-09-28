//
// TEMPLATE
//
//
// Permet de jouer un seul tour (en ne faisant rien s'il commence ou en
// réceptionnant le coup de l'adversaire s'il ne commence pas)
// et termine le jeu.
// Ce programme vous sert de base pour construire votre propre programme


#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"
#include <unistd.h>
#include "fonction.h"


extern int debug;	/* hack to enable debug messages */


int main()
{
	char labName[50];					/* name of the labyrinth */
	char* labData;						/* data of the labyrinth */
	t_return_code ret = MOVE_OK;		/* indicates the status of the previous move */
	t_move move;						/* a move */
	t_move** itineraire=NULL;
	int player, mode;
	int sizeX,sizeY;
	int En_M, En_Y; //Energie Me & You
	Position Me, You, Tresor;
	#ifdef DEBUG
	int debug_counter=0;
	#endif

	/* connection to the server */
	connectToServer( "pc4023.polytech.upmc.fr", 1234, "TrumpvsMexico");


	/* wait for a game, and retrieve informations about it */
	waitForLabyrinth( "ASTAR timeout=100", labName, &sizeX, &sizeY);
	labData = (char*) malloc( sizeX * sizeY );
	player = getLabyrinth( labData);

	//Initialisation des Positions
	init_info(&Me, &You, &Tresor, &En_M, &En_Y, player, sizeX, sizeY);

    #ifdef DEBUG
	print_labdata(labData, sizeX, sizeY);
	printf("\nTrump : (%d,%d)", Me.x, Me.y);
	printf("\nMexico : (%d,%d)\n", You.x, You.y);
	#endif

    printf("\nMode de Jeu : 0.Passif / 1.Manuel / 2.Random / 3.Astaromove / 4.Smartmove\n");
    scanf(" %d",&mode);

	sendComment("We will build a great WALL");

	while(ret==MOVE_OK){

		/* display the labyrinth */
		printLabyrinth();
		//print_labdata(labData, sizeX, sizeY);

		if (player==1)	/* The opponent plays */
		{
		    ret = getMove( &move);
			info_suivante(move, &You, &Me, &Tresor, &En_Y, labData, sizeX, sizeY); //Modification des positions

			//sendComment("The WALL got 10 feets higher !");
		}
		else
		{
            if(mode==0){
                move.type=8;
                move.value=0;
            }
            if(mode==1){
                defmove(&move);
            }
            if(mode==2){
                randomove(&move, Me, En_M, labData, sizeX, sizeY);
            }
            if(mode==3){
                Astaromove(&move, itineraire, Me, Tresor, labData, sizeX, sizeY);
            }
            if(mode==4){
				move.type=8;
                move.value=0;
                smartmove(&move, itineraire, En_M, Me, You, Tresor, labData, sizeX, sizeY);
            }
            ret = sendMove(move);
			info_suivante(move, &Me, &You, &Tresor, &En_M, labData, sizeX, sizeY); //Modification des positions

		}
		#ifdef DEBUG
		//Test
		print_labdata(labData, sizeX, sizeY);
		printf("\nLes Gentils: (%d,%d)", Me.x, Me.y);
		printf("\nLes Méchants: (%d,%d)", You.x, You.y);
		printf("\nEnergie Gentils : %d\nEnergie Méchants : %d\n\n\n", En_M, En_Y);
		if(debug_counter==0){
			printf("\nCombien de tour avant prochaine pause :");
			scanf(" %d",&debug_counter);
		}
		debug_counter--;
		#endif

		if ((player ==1 && ret == MOVE_WIN) || (player==0 && ret == MOVE_LOSE))
		  printf("No rage N00B\n");

		if ((player ==0 && ret == MOVE_WIN) || (player==1 && ret == MOVE_LOSE))
			printf("GG WP\n");

		player = (player+1)%2;//Changement de joueur
	}

	/* we do not forget to free the allocated array */
	free(labData);
 	free_itineraire(itineraire);

	/* end the connection, because we are polite */
	closeConnection();

	return EXIT_SUCCESS;
}



#include "fonction.h"

void smartmove(t_move* move, t_move** actions, int E1, Position pos1, Position pos2, Position tresor, char* labData, int size_x, int size_y){ //Joue avec une stratégie pour gagner !

    if(astarCompare(pos1,pos2,tresor,labData,size_x,size_y)<1 && E1>=5){

       smartRotate(move,E1,pos1,pos2,tresor,labData,size_x,size_y);
    }
	if(move->type==8){
		Astaromove(move, actions, pos1, tresor, labData, size_x, size_y);
	}
}

//NOTE : SI aucun move avantageux trouvé, on change var de facon a faire des recherches sur tout le labyrinthe
void smartRotate(t_move* movef, int E1, Position pos1, Position pos2, Position tresor, char* labData, int size_x, int size_y){ //Cherche la rotation la plus avantageuse avec une amplitude de recherche de varX, varY

    char* labTest; //Labyrinthe temporaire
    t_move test_move;
	Position ptest1, ptest2, ptestt;
	int avance=0,temp=0,i=0,varX=3,varY=3; //var indique la largeur des recherches autour du tresor

    //Initialisation
	movef->type=8;
	movef->value=0;

	labTest = strdup(labData); //Malloc

	//Evaluations des lignes
	for(i=tresor.y-varY; i<tresor.y+varY; i++){

		//Rotation vers la gauche
		test_move.type=0;
		test_move.value=i;

		info_suivante(test_move, &ptest1, &ptest2, &ptestt, &E1, labTest, size_x, size_y);

		//Calcul de l'avantage de la rotation
		temp = astarCompare(ptest1, ptest2, ptestt, labTest, size_x, size_y);
		avance = update_move(movef, test_move, avance, temp); //Changement du move final si necessaire

		reset_info(pos1, &ptest1, pos2, &ptest2, tresor, &ptestt, labData, &labTest); //Reinitialisation

		//Rotation vers la droite
		test_move.type=1;
		test_move.value=i;

		info_suivante(test_move, &ptest1, &ptest2, &ptestt, &E1, labTest, size_x, size_y);

		//Calcul de l'avantage de la rotation
		temp = astarCompare(ptest1, ptest2, ptestt, labTest, size_x, size_y);
		avance = update_move(movef, test_move, avance, temp); //Changement du move final si necessaire

		reset_info(pos1, &ptest1, pos2, &ptest2, tresor, &ptestt, labData, &labTest); //Reinitialisation
	}

	//Evaluations des colonnes
	for(i=tresor.x-varX; i<tresor.x+varX; i++){

		//Rotation vers le haut
		test_move.type=2;
		test_move.value=i;

		info_suivante(test_move, &ptest1, &ptest2, &ptestt, &E1, labTest, size_x, size_y);

		//Calcul de l'avantage de la rotation
		temp = astarCompare(ptest1, ptest2, ptestt, labTest, size_x, size_y);
		avance = update_move(movef, test_move, avance, temp); //Changement du move final si necessaire

		reset_info(pos1, &ptest1, pos2, &ptest2, tresor, &ptestt, labData, &labTest); //Reinitialisation

		//Rotation vers le bas
		test_move.type=3;
		test_move.value=i;

		info_suivante(test_move, &ptest1, &ptest2, &ptestt, &E1, labTest, size_x, size_y);

		//Calcul de l'avantage de la rotation
		temp = astarCompare(ptest1, ptest2, ptestt, labTest, size_x, size_y);
		avance = update_move(movef, test_move, avance, temp); //Changement du move final si necessaire

		reset_info(pos1, &ptest1, pos2, &ptest2, tresor, &ptestt, labData, &labTest); //Reinitialisation
	}
	//Free
	free(labTest);
}

int astarCompare(Position pos1, Position pos2, Position tresor, char* labData, int size_x, int size_y){ //Retourne le nombre de coup d'avance sur l'adversaire (pos1 = Joueur Actif)

    t_move** itineraire1 = NULL;
	t_move** itineraire2 = NULL;
    int nbcoup1, nbcoup2;

    //Estimation des itineraires des joueurs
    itineraire1 = Astar(pos1, tresor, labData, size_x, size_y);
    itineraire2 = Astar(pos2, tresor, labData, size_x, size_y);

    //Estimation du nombre de coup jusqu'au tresor
    nbcoup1 = nbcoup(itineraire1);
    nbcoup2 = nbcoup(itineraire2);

    //Free
    free_itineraire(itineraire1);
    free_itineraire(itineraire2);

    return nbcoup2 - nbcoup1; //Plus c'est positif, plus c'est a notre avantage
}

int nbcoup(t_move** itineraire){//Détermine le nombre de coup necessaire pour faire l'itineraire

    int i=0;

    if (itineraire[i]!=NULL){
        while (itineraire[i]!=NULL){i++;}
        i++; //On compte le move de rang 0
    }

    return i;
}


void reset_info(Position p1, Position* pt1, Position p2, Position* pt2, Position pt, Position* ptt, char* labData, char** labTest){ //Remet les infos du labyrinthe avant un test de rotation
	*pt1 = p1;
	*pt2 = p2;
	*ptt = pt;
	strcpy(*labTest, labData);
}


int update_move(t_move* movef, t_move move, int avance, int temp){ //Change le move retenu si le move propose est plus interessant

	if(temp>avance){ //Si la rotation est plus intéressante, on la garde
		movef->type = move.type;
		movef->value = move.value;
		return temp;
	}
	else{
		return avance;
	}
}

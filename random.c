#include "fonction.h"

void randomove(t_move* move, Position pos1, int E, char* labData, int size_x, int size_y){ //Joue aléatoirement et légalement
	int tabM[] ={-1, -1, -1, -1, -1, -1, -1, -1, 1};
	int i;
	Position postemp;

	srand(time(NULL));

	//Validité de la rotation
	if(E>4){ // Suffisament d'energie pour une rotation
		for(i=0;i<4;i++){ tabM[i]=1; }
	}

	//Validité des cases voisines
	postemp = pos1; //UP
	postemp.y-=1;
	ajust_pos(&postemp, size_x, size_y);//Calcul de la position haut
	if (test_case(postemp, labData, size_x) == 1) {tabM[4] = 1;} //Test validité position haut

	postemp = pos1; //DOWN
	postemp.y+=1;
	ajust_pos(&postemp, size_x, size_y);
	if (test_case(postemp, labData, size_x) == 1) {tabM[5] = 1;}

	postemp = pos1; //LEFT
	postemp.x-=1;
	ajust_pos(&postemp, size_x, size_y);
	if (test_case(postemp, labData, size_x) == 1) {tabM[6] = 1;}

	postemp = pos1; //RIGHT
	postemp.x+=1;
	ajust_pos(&postemp, size_x, size_y);
	if (test_case(postemp, labData, size_x) == 1) {tabM[7] = 1;}

	//Aléatoire
	do {
		i = rand()%9;
	}while (tabM[i]!=1);

	move->type=i; //Type de mouvement aléatoire

	if(move->type<2){ //Si rotation Ligne
		i = rand()%size_y;
		move->value=i;
	}
	else if (move->type<4){ //Si rotation Colonne
		i = rand()%size_x;
		move->value=i;
	}

	#ifdef DEBUG
	printf("Move type :%d\n",move->type);
	printf("Move Value: %d",move->value);
	#endif

}


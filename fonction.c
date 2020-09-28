#include "fonction.h"

void defmove(t_move* move){

	int a;

	printf("\n Déplacement - ROTATE_LINE_LEFT = 0, ROTATE_LINE_RIGHT = 1, ROTATE_COLUMN_UP = 2, ROTATE_COLUMN_DOWN = 3, MOVE_UP = 4, MOVE_DOWN = 5, MOVE_LEFT = 6, MOVE_RIGHT = 7, DO_NOTHING = 8\n");

	scanf(" %d", &a);
	move->type = a;

	if(move->type <= 3){
		printf("Indice de ligne/colonne à rotater ; le (0,0) est en haut à gauche : ");
		scanf(" %d", &(move->value));
	}
	else {move->value=0;}

}

void print_labdata(char* labData, int size_x, int size_y){ //Affiche la structure du labyrinthe en binaire

	int i;
	printf("\n");
	for(i=0; i<(size_x * size_y); i++){
		if(labData[i]==0){ printf("  "); }
		else{ printf("@ "); }
		if ((i+1)%size_x == 0){printf("\n");}

	}

}

void init_info(Position* J1, Position* J2, Position* Tresor, int* En1, int* En2, int player, int size_x, int size_y){ //Initialise les infos du labyrinthe de tout les éléments (J1 : Nous / J2 : Adversaire)

	if(player==0){ //On commence
		J1->x=0;
		J1->y=size_y/2;
		J2->x=size_x-1;
		J2->y=size_y/2;
		*En2=2;
	}
	else{ //L'adversaire commence
		J2->x=0;
		J2->y=size_y/2;
		J1->x=size_x-1;
		J1->y=size_y/2;
		*En1=2;
	}
	//Position trésor
		Tresor->x=size_x/2;
		Tresor->y=size_y/2;
}

void info_suivante(t_move move, Position* pos1, Position* pos2, Position* tresor, int* E, char* labData, int size_x, int size_y){ //Actualise les infos du labyrinthe (pos1 = joueur actif)

	switch(move.type){

		case ROTATE_LINE_LEFT:
			rotation_pos(pos1, move);
			rotation_pos(pos2, move);
			rotation_pos(tresor, move);
			rotation_lab(labData, move, size_x, size_y);
			(*E)-=5;

		break;

		case ROTATE_LINE_RIGHT:
			rotation_pos(pos1, move);
			rotation_pos(pos2, move);
			rotation_pos(tresor, move);
			rotation_lab(labData, move, size_x, size_y);
			(*E)-=5;
		break;

		case ROTATE_COLUMN_UP:
			rotation_pos(pos1, move);
			rotation_pos(pos2, move);
			rotation_pos(tresor, move);
			rotation_lab(labData, move, size_x, size_y);
			(*E)-=5;
		break;

		case ROTATE_COLUMN_DOWN:
			rotation_pos(pos1, move);
			rotation_pos(pos2, move);
			rotation_pos(tresor, move);
			rotation_lab(labData, move, size_x, size_y);
			(*E)-=5;
		break;

		case MOVE_UP:
			pos1->y-=1;
			(*E)++;
		break;

		case MOVE_DOWN:
			pos1->y+=1;
			(*E)++;
		break;

		case MOVE_LEFT:
			pos1->x-=1;
			(*E)++;
		break;

		case MOVE_RIGHT:
			pos1->x+=1;
			(*E)++;
		break;

		case DO_NOTHING:
			(*E)++;
		break;
	}
	ajust_pos(pos1, size_x, size_y);
	ajust_pos(pos2, size_x, size_y);
	ajust_pos(tresor, size_x, size_y);
}

//Ajuste la position de pos pour les depassement d'extrémités
void ajust_pos(Position* pos, int size_x, int size_y){

	if(pos->y<0){ pos->y=size_y-1; } //Limite haute du labyrinthe, on passe a l'autre extremité
	if(pos->y>=size_y){ pos->y=0; } //Limite basse du labyrinthe, on passe a l'autre extremité
	if(pos->x<0){ pos->x=size_x-1; } //Limite gauche du labyrinthe, on passe a l'autre extremité
	if(pos->x>=size_x){ pos->x=0; } // Limite droite du labyrinthe, on passe a l'autre extremité

}

void rotation_pos(Position* pos, t_move move){ //Déplace l'élément en pos s'il se situe sur une rotation

	if(move.value==pos->x){	//Si notre élement en pos se situe sur un mouvement d'une colonne
		if(move.type==ROTATE_COLUMN_UP){ pos->y-=1; }
		if(move.type==ROTATE_COLUMN_DOWN){ pos->y+=1; }
	}
	if(move.value==pos->y){ //Si notre élement en pos se situe sur un mouvement d'une ligne
		if(move.type==ROTATE_LINE_LEFT){ pos->x-=1; }
		if(move.type==ROTATE_LINE_RIGHT){ pos->x+=1; }
	}
}

void rotation_lab(char* labData, t_move move, int size_x, int size_y){ //Modifie le labyrinthe en cas de rotation (POSSIBILITE : Placer les lignes de codes des if dans pos_suivante)
    int i, temp;
    if(move.type==ROTATE_LINE_LEFT){
        temp=labData[(move.value*size_x)]; // On garde en mémoire le terme du tableau qui va passer de l'autre côté du labyrinthe avec la rotation (Ici, le 1er terme)
        for(i=0;i<size_x-1;i++){ //On parcours la ligne en question en échangeant les termes vers la gauche
            labData[(move.value*size_x)+i]=labData[(move.value*size_x)+i+1];
        }
        labData[((move.value+1)*size_x)-1]=temp; // On place le terme a la fin de la ligne
    }
    if(move.type==ROTATE_LINE_RIGHT){
        temp=labData[((move.value+1)*size_x)-1]; //On garde en mémoire le dernier terme de la ligne
        for(i=size_x-1;i>0;i--){//On parcours la ligne en sens inverse en échangeant les termes vers la droite
            labData[(move.value*size_x)+i]=labData[(move.value*size_x)+i-1];
        }
        labData[(move.value*size_x)]=temp; //On place le terme au début de la ligne
    }
    if(move.type==ROTATE_COLUMN_UP){
        temp=labData[move.value]; //On garde en mémoire le 1er terme de la colonne
        for(i=1;i<size_y-1;i++){ //On parcours la colonne de haut en bas en échangeant les terme vers le haut
            labData[move.value+size_x*(i-1)]=labData[move.value+size_x*i];
        }
        labData[size_x*(size_y-1)+move.value]=temp; //On place le terme  la fin de la colonne
    }
    if(move.type==ROTATE_COLUMN_DOWN){
        temp=labData[size_x*(size_y-1)+move.value]; //On garde en mémoire le dernier terme de la colonne
        for(i=size_y-1;i>0;i--){ //On parcours la colonne de bas en haut en échangeant vers le bas
            labData[move.value+size_x*i]=labData[move.value+size_x*(i-1)];
        }
        labData[move.value]=temp; //On place le terme au début de la colonne
    }
}

//Convertit une position en deux dimensions en une position en une dimension (pour manipuler labData)
int convert(Position pos, int size_x){
	return pos.x + pos.y * size_x;
}

//Renvoie 1 si la case est libre, renvoie 0 si la case n'est pas libre
int test_case(Position pos, char* labData, int size_x){ //Il faut bien mettre size_x c-a-d longueur d'une ligne c-a-d nombre de colonnes

	if (labData[convert(pos, size_x)] == 1){return 0;}
	else {return 1;}
}




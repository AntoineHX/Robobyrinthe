
//Fait un move aléatoire
#include <time.h>
#include <stdlib.h>

//Choisit un type de mouvement aléatoire et une ligne/colonne aléatoire si rotation
void random_move(t_move* move, int size_x, int size_y){

	srand(time(NULL));
	int n1, n2;

	n1 = rand()%9;
	move->type = n1; //Choisit une type de move aléatoire
	move->value = 0; 

	if (n1==2 || n1==3){  //Cas où on bouge une colonne, il y a size_x colonnes
		n2 = rand()%size_x;
		move->value = n2; //Choisit une colonne aléatoire à bouger
	}
	else if (n1==0 || n1==1){ //Cas où on bouge une ligne, il y a size_y lignes
		n2 = rand()%size_y;
		move->value = n2; //Choisit une ligne aléatoire à bouger
	}
}

//Convertit une position en deux dimensions en une position en une dimension (pour manipuler labData)
int convert(Position pos, int size_x){
	return pos->x + pos->y * (size_x + 1);
}

//Renvoie 1 si la case en bonne, renvoie 0 si la case n'est pas bonne
int test_case(Position pos, char* labData, int size_x){

	if (labData[convert(pos, size_x)] == 1){return 0;}
	else {return 1;}

}

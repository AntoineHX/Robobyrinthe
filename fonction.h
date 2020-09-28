#ifndef __FONCTION_H__
#define __FONCTION_H__

#include <stdio.h>
#include <stdlib.h>
#include "labyrinthAPI.h"
#include <time.h>
#include <math.h>
#include <string.h>

//#define DEBUG

//DEFMOVE
typedef struct position{
	int x;
	int y;
} Position;

void defmove(t_move * move); //Fonction de mouvement manuel
void print_labdata(char* labData, int size_x, int size_y); //Affiche la structure du labyrinthe en binaire
void init_info(Position* J1, Position* J2, Position* Tresor, int* En1, int* En2, int player, int size_x, int size_y); //Initialise les infos du labyrinthe de tout les éléments (J1 : Nous / J2 : Adversaire)
void info_suivante(t_move move, Position* pos1, Position* pos2, Position* tresor, int* E, char* labData, int size_x, int size_y); //Actualise les infos du labyrinthe (pos1 = joueur actif)
void ajust_pos(Position* pos, int size_x, int size_y); //Ajuste la position de pos pour les depassement d'extrémités
void rotation_pos(Position* pos, t_move move); //Déplace l'élément en pos s'il se situe sur une rotation
void rotation_lab(char* labData, t_move move, int size_x, int size_y); //Modifie le labyrinthe en cas de rotation (POSSIBILITE : Placer les lignes de codes des if dans pos_suivante)

//RANDOM
void randomove(t_move* move, Position pos1, int E, char* labData, int size_x, int size_y); //Joue aléatoirement et légalement
int convert(Position pos, int size_x); //Convertit une position en deux dimensions en une position en une dimension (pour manipuler labData)
int test_case(Position pos, char* labData, int size_x); //Il faut bien mettre size_x c-a-d longueur d'une ligne c-a-d nombre de colonnes //Renvoie 1 si la case est libre, renvoie 0 si la case n'est pas libre

//ASTAR
typedef struct cases Case;
struct cases{
    Position pos; //Case actuelle
    int cost; //Coût du départ à cette position
    int prev; //Prevision du coût total, du départ au tresor en passant par cette position
    Case* prec; //Case d'où on vient
};

typedef struct element Element;
struct element{
    Case c;
    Element* suiv;
};
typedef Element* ListeC;

void Astaromove(t_move* move, t_move** itineraire, Position pos1, Position tresor, char* labData, int size_x, int size_y); //Joue sans rotation pour aller le plus rapidement possible au trésor
t_move** Astar(Position pos1, Position tresor, char* labData, int size_x, int size_y); // Trouve le chemin le plus rapide jusqu'au tresor
int estimation_distance(Position J, Position P, int sizeX, int sizeY); //Estimation de l'heuristique
Case newCase(Case* c, int x, int y, Position tresor,int sizeX, int sizeY); //Initialise une case voisine à c par ces coordonées
ListeC ajout_case(Case c, ListeC liste); //Ajoute c en début de liste
void verif_voisins(Position* voisins, ListeC liste); //Modifie voisins pour qu'il ne reste que des cases non évaluées
void verif_voisins_mur(Position* voisins, char* labData, int size_x); //Modifie voisins pour qu'il ne reste que des cases qui ne sont pas des murs
int verif_positions (Position P1, Position P2); //retourne 1 si P1 et P2 ont les mêmes coordonées, 0 sinon
t_move** chemin(Case c); //donne l'itinéraire depuis le départ pour aller jusqu'à c
t_move* move_pos(Position Ini, Position Fin); //Donne le mouvement pour aller de Ini a Fin
void free_itineraire(t_move** itineraire); //Libere un tableau de move
void free_liste(ListeC liste); //Libere une liste d'Element

//SMART
void smartmove(t_move* move, t_move** actions, int E1, Position pos1, Position pos2, Position tresor, char* labData, int size_x, int size_y); //Joue avec une stratégie pour gagner !
void smartRotate(t_move* movef, int E1, Position pos1, Position pos2, Position tresor, char* labData, int size_x, int size_y); //Cherche la rotation la plus avantageuse avec une amplitude de recherche de varX, varY
int astarCompare(Position pos1, Position pos2, Position tresor, char* labData, int size_x, int size_y); //Retourne le nombre de coup d'avance sur l'adversaire (pos1 = Joueur Actif)
int nbcoup(t_move** itineraire);//Détermine le nombre de coup necessaire pour faire l'itineraire
void reset_info(Position p1, Position* pt1, Position p2, Position* pt2, Position pt, Position* ptt, char* labData, char** labTest); //Remet les infos du labyrinthe avant un test de rotation
int update_move(t_move* movef, t_move move, int avance, int temp); //Change le move retenu si le move propose est plus interessant

#endif



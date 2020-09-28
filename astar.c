#include "fonction.h"

t_move** Astar(Position pos1, Position tresor, char* labData, int size_x, int size_y){ //Trouve le chemin le plus rapide vers le trésor

	//Initialisation des variables et structures
	int i;
    	ListeC closedList=NULL, openList=NULL, temp_list, min_list;
    	Position voisins[4], cp;
    	Case D; //Case de départ de l'A*
	t_move** itineraire=NULL;

        //Initialisation de la case de départ
        (D.pos).x=pos1.x;
        (D.pos).y=pos1.y;
        D.cost=0;
        D.prev=estimation_distance(D.pos, tresor, size_x,size_y);
        D.prec=NULL; //Indicateur Case de départ

        //Initialisation de l'openList
        openList=ajout_case(D,openList);

        //Algorithme A*
        while(openList!=NULL){
			//Reinitialisation des variables
			min_list=NULL;
			temp_list=openList;

            //Recherche de la case proposant la plus faible estimation
            while(temp_list!=NULL){
                if(min_list==NULL ||((min_list->c).prev) > ((temp_list->c).prev)){
                    min_list=temp_list;
                }
            temp_list=temp_list->suiv;
            }

            //Retrait de la case d'openList
            temp_list=openList;
            if(temp_list==min_list){ //Si on supprime le premier element
				openList=min_list->suiv;
			}
            else{
	            while(temp_list->suiv!=min_list){ //Si on supprime un des élément restant
			        temp_list=temp_list->suiv;
                }
                temp_list->suiv=min_list->suiv;
            }

			//Transfert de la case de openList en tete de closedList
            min_list->suiv = closedList;
	        closedList = min_list;
            cp = (closedList->c).pos; //Case considérée = permiere case de closedList

            //Verification case tresor et sortie de boucle si c'est le cas
            if(verif_positions(cp,tresor)){ //Si on est arrivé au trésor
                itineraire= chemin(closedList->c);
                break;
            }

            //Determination des voisins sinon
            voisins[0].x=cp.x+1;
            voisins[0].y=cp.y;
            ajust_pos(voisins, size_x, size_y);
            voisins[1].x=cp.x-1;
            voisins[1].y=cp.y;
            ajust_pos(voisins+1, size_x, size_y);
            voisins[2].x=cp.x;
            voisins[2].y=cp.y+1;
            ajust_pos(voisins+2, size_x, size_y);
            voisins[3].x=cp.x;
            voisins[3].y=cp.y-1;
            ajust_pos(voisins+3, size_x, size_y);

            //Verification si les voisins n'ont pas déja ete evaluées (dans openList ou closedList) ou qu'il ne soit pas des murs
            verif_voisins(voisins, openList);
            verif_voisins(voisins, closedList);
			verif_voisins_mur(voisins, labData, size_x);

            //Ajout des voisins à openList
            for(i=0;i<4;i++){
                if((voisins[i]).x!=-1){ //si elle n'as pas déja été evaluée et quelle n'est pas un mur

                    D = newCase(&(closedList->c), (voisins[i]).x, (voisins[i]).y, tresor, size_x, size_y);

                    openList=ajout_case(D,openList);
                }
            }
        }

        //Liberation de memoire lorsque que l'itineraire est determine
        free_liste(openList);
        free_liste(closedList);

	return itineraire;
}

void Astaromove(t_move* move, t_move** itineraire, Position pos1, Position tresor, char* labData, int size_x, int size_y){ //Joue sans rotation pour aller le plus rapidement possible au trésor

    //S'il n'y a pas eu de rotations, pas besoin de recalculer l'itinéraire
    if((itineraire==NULL)||(move->type==ROTATE_LINE_LEFT)||(move->type==ROTATE_LINE_RIGHT)||(move->type==ROTATE_COLUMN_UP)||(move->type==ROTATE_COLUMN_DOWN)){
	//Suppression de l'ancien itinéraire
        free_itineraire(itineraire);
        itineraire = NULL;

        itineraire= Astar(pos1, tresor, labData, size_x, size_y);
    }

    //Envoie du move
    if(itineraire==NULL){ //Pas de solution trouvé

            move->type=DO_NOTHING;
            move->value=0;
            printf("\n Pas de chemin trouvé : STAND BY\n");
    }
    else{
        move->type=(itineraire[0])->type;
        move->value=0;
        //Suppression du move dans itineraire
        free(*itineraire);
        itineraire=&(itineraire[1]);
    }
    	#ifdef DEBUG
	printf("Move type :%d\n",move->type);
	printf("Move Value: %d",move->value);
	#endif
}

int estimation_distance(Position J, Position P, int sizeX, int sizeY){

	int dx, dy;

	dx = fabs(P.x - J.x); //Différence selon les colonnes
	dy = fabs(P.y - J.y); //Différence selon les lignes

	if (dx > sizeX/2) {dx = sizeX - dx;} //Si c'est mieux de passer de l'autre côté d'un mur gauche-droite
	if (dy > sizeY/2) {dy = sizeY - dy;} //Si c'est mieux de passer de l'autre côté d'un mur haut-bas

	return dx + dy;
}

Case newCase(Case* c, int x, int y, Position tresor, int sizeX, int sizeY){ //Initialise une case voisine à c par ces coordonées (x,y)

    Case v; //case voisine de c
    (v.pos).x=x;
    (v.pos).y=y;
    #ifdef DEBUG
    if(estimation_distance(c->pos,v.pos,sizeX,sizeY)!=1){
        printf("\n newCase Error : cases non voisines :(%d,%d) -> (%d,%d)\n",(c->pos).x,(c->pos).y,(v.pos).x,(v.pos).y);
        printf("Esti : %d\n",estimation_distance(c->pos,v.pos,sizeX,sizeY));
    }
    #endif
    v.cost = c->cost+1;
    v.prev=v.cost+estimation_distance(v.pos, tresor,sizeX,sizeY);
    v.prec=c;


    return v;
}

ListeC ajout_case(Case c, ListeC liste){ //Ajoute c en début de liste
    ListeC nliste;
    nliste=(ListeC)malloc(sizeof(Element)); // Allocation mémoire?
    nliste->c=c;
    nliste->suiv=liste;

    return nliste;
}

void verif_voisins(Position* voisins, ListeC liste){ //Modifie voisins pour qu'il ne reste que des cases non évaluées qui ne soit pas des murs

    int i;
    ListeC temp_list=liste;

    while(temp_list!=NULL){
        for(i=0;i<4;i++){
            if(verif_positions(voisins[i],(temp_list->c).pos)){
                voisins[i].x=-1;
            }
        }
        temp_list=temp_list->suiv;
    }
}

void verif_voisins_mur(Position* voisins, char* labData, int size_x){ //Modifie voisins pour qu'il ne reste que des cases qui ne sont pas des murs

	int i;

	for(i=0;i<4;i++){
        if(test_case(voisins[i], labData, size_x)==0){
            voisins[i].x=-1;
        }
    }
}

int verif_positions (Position P1, Position P2){ //retourne 1 si P1 et P2 ont les mêmes coordonées, 0 sinon

    if((P1.x==P2.x)&&(P1.y==P2.y)){
        return 1;
    }
    else{
        return 0;
    }
}

t_move** chemin(Case c){ //donne l'itinéraire depuis le départ pour aller jusqu'à c

    Case* temp_case=&c;
    Position temp_pos;
    int i, nb_move=0;
    t_move** itineraire;

    while (temp_case != NULL){ //On détermine combien de mouvement seront nécessaires
        temp_case=temp_case->prec;
        nb_move++;
    }
    itineraire=(t_move**)malloc((nb_move)*sizeof(t_move*));

    //Création de l'itinéraire
    temp_case=&c;
    itineraire[nb_move-1]=NULL;//Fin de l'itinéraire

    for(i=nb_move-2;i>=0;i--){//Construction décroissante car on part du trésor pour arriver au départ
        temp_pos=temp_case->pos;
        temp_case=temp_case->prec;

        itineraire[i]= move_pos(temp_case->pos,temp_pos);

    }

    return itineraire;
}

t_move* move_pos(Position Ini, Position Fin){ //Donne le mouvement pour aller de Ini a Fin
//MOVE_UP = 4, MOVE_DOWN = 5, MOVE_LEFT = 6, MOVE_RIGHT = 7, DO_NOTHING = 8
    int dx, dy;

	t_move* pmove=(t_move*)malloc(sizeof(t_move));

    dx=Fin.x-Ini.x;
    dy=Fin.y-Ini.y;

    if(dx!=0){
        if(dx==1||dx<0){
            pmove->type=7;
        }
        if(dx==-1||dx>1){
            pmove->type=6;
        }
    }

    if(dy!=0){
        if(dy==1||dy<0){
            pmove->type=5;
        }
        if(dy==-1||dy>1){
            pmove->type=4;
        }
    }
    if(dx==0&&dy==0){
        pmove->type= 8;
    }
    pmove->value=0;

    return pmove;
}

void free_itineraire(t_move** itineraire){

    //Cas ou itineraire n'est pas defini
    if(itineraire == NULL)  {return;}

	int i=0;
	t_move* temp=itineraire[0];

	while(temp!=NULL){
        free(temp);
        temp=itineraire[i];
        i++;
    }
    free(itineraire);
}


void free_liste(ListeC liste){

    //Cas ou Liste n'est pas defini
    if(liste == NULL)  {return;}

    ListeC temp = liste, suivant = NULL;

    while(temp->suiv!=NULL){
	    suivant = temp->suiv;
        free(temp);
        temp=suivant;
    }
    free(temp);
}



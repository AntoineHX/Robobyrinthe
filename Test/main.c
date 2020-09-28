#include <stdio.h>


typedef struct position{
	int x;
	int y;
} Position;

int main (){

	Position p1,p2;
	
	p1.x = 1;
	p1.y = 2;

	p2 = p1;

	p2.x = 3;

	printf("p1.x = %d\np1.y = %d\np2.x = %d\np2.y = %d\n", p1.x,p1.y,p2.x,p2.y);

	return 0;

}

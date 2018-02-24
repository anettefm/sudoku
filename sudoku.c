#include <stdio.h>
#include <stdlib.h>

struct Cell{
    int value;
    int posx;
    int posy;
    int predefined;
    int numOfPossibilities;
    int posibleSolutions[9];
    int solved;
}Cell={0, 0, 0, 0, 9, {1,1,1,1,1,1,1,1,1}, 0};

struct Route{
    int xStart;
    int xStop;
    int yStart;
    int yStop;
};

struct Board{
    int range;
    struct Cell celle[9][9];
    int change;

};

struct Board* makeBoard( char filename[] );
void printBoard(struct Board *board);
void reducePosibleSol(struct Board *board, int x, int y, int value);
struct Route getRoute(int x, int y);


int main(int argc, char *argv[])
{
    char *filename=argv[1];

    struct Board *board = makeBoard( filename);

    
    free(board);
   return 0;
}


 struct Board* makeBoard( char filename[] ){

    FILE * fp;

    if((fp = fopen(filename, "r")) == NULL) {
      printf("No such file\n");
      return NULL;
    }

    struct Board* board = malloc( sizeof(struct Board));
    fscanf(fp, "%1d", &board->range);

    #ifdef DEBUG
        printf("range: %d \n", board->range);
    #endif

    for( int x=0; x<board->range; x++){
        for( int y=0; y<board->range; y++){
            int tempVal;
            fscanf(fp, "%1d", &tempVal);
            if (tempVal==0){
                board->celle[x][y].posx = x;
                board->celle[x][y].posy = y;
            }else{
                board->celle[x][y].value=tempVal;
                board->celle[x][y].posx = x;
                board->celle[x][y].posy = y;
                board->celle[x][y].predefined=1;
                board->celle[x][y].solved=1;
                reducePosibleSol(board, x, y, tempVal);
            }
        }
    }

    #ifdef DEBUG
        printBoard(board);
    #endif

    fclose(fp);
    return board;
}

void reducePosibleSol(struct Board *board, int x, int y, int value){
    for( int i=0 ; i<board->range; i++){
        if(board->celle[x][i].predefined==0 && board->celle[x][i].solved==0 && board->celle[x][i].posibleSolutions[value-1]==1){
            board->celle[x][i].posibleSolutions[value-1]=1;
            board->celle[x][i].numOfPossibilities-=1;
        }
        if(board->celle[i][y].predefined==0 && board->celle[i][y].solved==0 && board->celle[i][y].posibleSolutions[value-1]==1){
            board->celle[i][y].posibleSolutions[value-1]=1;
            board->celle[i][y].numOfPossibilities-=1;
        }
    }

    struct Route route = getRoute( x, y );
    for (int i = route.xStart; i<= route.xStop; i++){
        for (int j = route.yStart; j<= route.yStop; j++){
            if(board->celle[i][j].predefined==0 && board->celle[i][j].solved==0 && board->celle[i][j].posibleSolutions[value-1]==1){
                board->celle[i][j].posibleSolutions[value-1]=1;
                board->celle[i][j].numOfPossibilities-=1;
            }
        }
    }
}

struct Route getRoute(int x, int y){
    struct Route route;

    if( x < 3 ){
        route.xStart = 0;
        route.xStop = 2;
    }else if( x < 6 ){
        route.xStart = 3;
        route.xStop = 5;
    } else{
        route.xStart = 6;
        route.xStop = 8;
    }

    if( y < 3 ){
        route.yStart = 0;
        route.yStop = 2;
    }else if( y < 6 ){
        route.yStart = 3;
        route.yStop = 5;
    } else{
        route.yStart = 6;
        route.yStop = 8;
    }

    return route;
}


void printBoard(struct Board *board){
    printf("BOARD: \n-------------------------\n");
    for( int x=0; x<board->range; x++){
        for( int y=1; y<board->range; y+=3){
            printf("| %d %d %d ", board->celle[x][y-1].value, board->celle[x][y].value, board->celle[x][y+1].value);
        }
        printf("| \n" );
        if (x==2 || x==5){
            printf("-------------------------\n");
        }

    }
    printf("-------------------------\n");
}

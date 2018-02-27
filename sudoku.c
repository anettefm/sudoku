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
};

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

int makeBoard(struct Board *board, char filename[] );
void printBoard(struct Board *board);
void reducePosibleSol(struct Board *board, int y, int x, int value);
struct Route getRoute(int y, int x);
int findValue( int values[9]);
struct Board* solveBoardOneRound( struct Board *board );
void printPossibleSolutions(struct Board *board);
void initializeBoard(struct Board *board);
void printSolved(struct Board *board);
int checkSolution(struct Board *board, char filename[]);

int main(int argc, char *argv[])
{
    int ret;
    char *filename=argv[1];

    struct Board* board = malloc( sizeof(struct Board));
    /*
    initializeBoard(board);
    printPossibleSolutions(board);
    */
    makeBoard( board, filename);
    printBoard(board);
    #ifdef DEBUG

        printPossibleSolutions(board);
        printSolved(board);
        printf( "change: %d \n", board->change);
    #endif
    while (board->change==1){
        board->change=0;
        board = solveBoardOneRound( board );

        #ifdef DEBUG
            printBoard(board);
            printPossibleSolutions(board);
            printSolved(board);
            printf( "change: %d \n", board->change);
        #endif
    }
    #ifdef DEBUG
        ret= checkSolution(board, argv[2]);
        if (ret==1){
            printf("The solution is correct\n");
        } else{
            printf("The solutoin is INCORRECT\n" );
        }
    #endif
    printf("Final solution:\n" );
    printBoard(board);
    free(board);
   return 0;
}

struct Board* solveBoardOneRound( struct Board *board ){
    for( int y = 0 ; y < 9; y++){
        for( int x = 0 ; x < 9; x++){
            if (board->celle[y][x].numOfPossibilities==1 && board->celle[y][x].solved==0){
                board->celle[y][x].value=findValue(board->celle[y][x].posibleSolutions);
                board->celle[y][x].solved=1;
                board->celle[y][x].numOfPossibilities=0;
                board->change=1;
                reducePosibleSol(board, y, x, board->celle[y][x].value);
            }
        }
    }
    return board;
}

int findValue( int values[9]){
    for (int i = 0; i<9; i++){
        if (values[i]==1){
            return i+1;
        }
    }
    return -1;
}

void initializeBoard(struct Board* board){
    for( int y=0; y<9; y++){
        for( int x=0; x<9; x++){
            board->celle[y][x].value=0;
            board->celle[y][x].posx = x;
            board->celle[y][x].posy = y;
            board->celle[y][x].predefined=0;
            board->celle[y][x].numOfPossibilities=9;
            board->celle[y][x].posibleSolutions[0]=1;
            board->celle[y][x].posibleSolutions[1]=1;
            board->celle[y][x].posibleSolutions[2]=1;
            board->celle[y][x].posibleSolutions[3]=1;
            board->celle[y][x].posibleSolutions[4]=1;
            board->celle[y][x].posibleSolutions[5]=1;
            board->celle[y][x].posibleSolutions[6]=1;
            board->celle[y][x].posibleSolutions[7]=1;
            board->celle[y][x].posibleSolutions[8]=1;
            board->celle[y][x].solved=0;
        }
    }

}

int makeBoard( struct Board *board, char filename[] ){

    FILE * fp;

    if((fp = fopen(filename, "r")) == NULL) {
      printf("No such file\n");
      return -1;
    }

    initializeBoard( board);
    fscanf(fp, "%1d", &board->range);

    for( int y=0; y<board->range; y++){
        for( int x=0; x<board->range; x++){
            int tempVal;
            fscanf(fp, "%1d", &tempVal);
            if (tempVal==0){
                board->celle[y][x].posx = x;
                board->celle[y][x].posy = y;
            }else{
                board->celle[y][x].value=tempVal;
                board->celle[y][x].posx = x;
                board->celle[y][x].posy = y;
                board->celle[y][x].predefined=1;
                board->celle[y][x].solved=1;
                board->celle[y][x].numOfPossibilities=-1;
                reducePosibleSol(board, y, x, tempVal);
            }
        }
        board->change=1;
    }

    fclose(fp);
    return 1;
}

void reducePosibleSol(struct Board *board, int y, int x, int value){
    for( int i=0 ; i<board->range; i++){
        if(board->celle[y][i].predefined==0 && board->celle[y][i].solved==0 && board->celle[y][i].posibleSolutions[value-1]==1){
            board->celle[y][i].posibleSolutions[value-1]=0;
            board->celle[y][i].numOfPossibilities-=1;
        }
        if(board->celle[i][x].predefined==0 && board->celle[i][x].solved==0 && board->celle[i][x].posibleSolutions[value-1]==1){
            board->celle[i][x].posibleSolutions[value-1]=0;
            board->celle[i][x].numOfPossibilities-=1;
        }
    }

    struct Route route = getRoute( y, x );
    for (int i = route.yStart; i<= route.yStop; i++){
        for (int j = route.xStart; j<= route.xStop; j++){
            if(board->celle[i][j].predefined==0 && board->celle[i][j].solved==0 && board->celle[i][j].posibleSolutions[value-1]==1){
                board->celle[i][j].posibleSolutions[value-1]=0;
                board->celle[i][j].numOfPossibilities-=1;
            }
        }
    }
}

struct Route getRoute(int y, int x){
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
    for( int y=0; y<board->range; y++){
        for( int x=1; x<board->range; x+=3){
            printf("| %d %d %d ", board->celle[y][x-1].value, board->celle[y][x].value, board->celle[y][x+1].value);
        }
        printf("| \n" );
        if (y==2 || y==5){
            printf("-------------------------\n");
        }

    }
    printf("-------------------------\n");
}


void printPossibleSolutions(struct Board *board){
    printf("NUMBER OF POSSIBLE SOLUTIONS: \n-------------------------\n");
    for( int y=0; y<9; y++){
        for( int x=1; x<9; x+=3){
            printf("| %d %d %d ",board->celle[y][x-1].numOfPossibilities, board->celle[y][x].numOfPossibilities, board->celle[y][x+1].numOfPossibilities);
        }
        printf("| \n" );
        if (y==2 || y==5){
            printf("-------------------------\n");
        }

    }
    printf("-------------------------\n");
}
void printSolved(struct Board *board){
    printf("SOLVED: \n-------------------------\n");
    for( int y=0; y<9; y++){
        for( int x=1; x<9; x+=3){
            printf("| %d %d %d ",board->celle[y][x-1].solved, board->celle[y][x].solved, board->celle[y][x+1].solved);
        }
        printf("| \n" );
        if (y==2 || y==5){
            printf("-------------------------\n");
        }

    }
    printf("-------------------------\n");
}

int checkSolution(struct Board *board, char filename[]){
    FILE * fp;

    if((fp = fopen(filename, "r")) == NULL) {
      printf("No such file\n");

    }
    int range;
    fscanf(fp, "%1d", &range);

    for( int y=0; y<board->range; y++){
        for( int x=0; x<board->range; x++){
            int tempVal;
            fscanf(fp, "%1d", &tempVal);
            if (board->celle[y][x].value==tempVal){

            }else{
                return 0;
            }
        }
        board->change=1;
    }

    fclose(fp);
    return 1;
}

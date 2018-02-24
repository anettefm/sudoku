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

void makeBoard(struct Board *board, char filename[] );
void printBoard(struct Board *board);
void reducePosibleSol(struct Board *board, int x, int y, int value);
struct Route getRoute(int x, int y);
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
    for( int x = 0 ; x < 9; x++){
        for( int y = 0 ; y < 9; y++){
            if (board->celle[x][y].numOfPossibilities==1 && board->celle[x][y].solved==0){
                board->celle[x][y].value=findValue(board->celle[x][y].posibleSolutions);
                board->celle[x][y].solved=1;
                board->celle[x][y].numOfPossibilities=0;
                board->change=1;
                reducePosibleSol(board, x, y, board->celle[x][y].value);
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
    for( int x=0; x<9; x++){
        for( int y=0; y<9; y++){
            board->celle[x][y].value=0;
            board->celle[x][y].posx = x;
            board->celle[x][y].posy = y;
            board->celle[x][y].predefined=0;
            board->celle[x][y].numOfPossibilities=9;
            board->celle[x][y].posibleSolutions[0]=1;
            board->celle[x][y].posibleSolutions[1]=1;
            board->celle[x][y].posibleSolutions[2]=1;
            board->celle[x][y].posibleSolutions[3]=1;
            board->celle[x][y].posibleSolutions[4]=1;
            board->celle[x][y].posibleSolutions[5]=1;
            board->celle[x][y].posibleSolutions[6]=1;
            board->celle[x][y].posibleSolutions[7]=1;
            board->celle[x][y].posibleSolutions[8]=1;
            board->celle[x][y].solved=0;
        }
    }

}

void makeBoard( struct Board *board, char filename[] ){

    FILE * fp;

    if((fp = fopen(filename, "r")) == NULL) {
      printf("No such file\n");

    }

    initializeBoard( board);
    fscanf(fp, "%1d", &board->range);

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
                board->celle[x][y].numOfPossibilities=-1;
                reducePosibleSol(board, x, y, tempVal);
            }
        }
        board->change=1;
    }

    fclose(fp);
}

void reducePosibleSol(struct Board *board, int x, int y, int value){
    for( int i=0 ; i<board->range; i++){
        if(board->celle[x][i].predefined==0 && board->celle[x][i].solved==0 && board->celle[x][i].posibleSolutions[value-1]==1){
            board->celle[x][i].posibleSolutions[value-1]=0;
            board->celle[x][i].numOfPossibilities-=1;
        }
        if(board->celle[i][y].predefined==0 && board->celle[i][y].solved==0 && board->celle[i][y].posibleSolutions[value-1]==1){
            board->celle[i][y].posibleSolutions[value-1]=0;
            board->celle[i][y].numOfPossibilities-=1;
        }
    }

    struct Route route = getRoute( x, y );
    for (int i = route.xStart; i<= route.xStop; i++){
        for (int j = route.yStart; j<= route.yStop; j++){
            if(board->celle[i][j].predefined==0 && board->celle[i][j].solved==0 && board->celle[i][j].posibleSolutions[value-1]==1){
                board->celle[i][j].posibleSolutions[value-1]=0;
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


void printPossibleSolutions(struct Board *board){
    printf("NUMBER OF POSSIBLE SOLUTIONS: \n-------------------------\n");
    for( int x=0; x<9; x++){
        for( int y=1; y<9; y+=3){
            printf("| %d %d %d ",board->celle[x][y-1].numOfPossibilities, board->celle[x][y].numOfPossibilities, board->celle[x][y+1].numOfPossibilities);
        }
        printf("| \n" );
        if (x==2 || x==5){
            printf("-------------------------\n");
        }

    }
    printf("-------------------------\n");
}
void printSolved(struct Board *board){
    printf("SOLVED: \n-------------------------\n");
    for( int x=0; x<9; x++){
        for( int y=1; y<9; y+=3){
            printf("| %d %d %d ",board->celle[x][y-1].solved, board->celle[x][y].solved, board->celle[x][y+1].solved);
        }
        printf("| \n" );
        if (x==2 || x==5){
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

    for( int x=0; x<board->range; x++){
        for( int y=0; y<board->range; y++){
            int tempVal;
            fscanf(fp, "%1d", &tempVal);
            if (board->celle[x][y].value==tempVal){

            }else{
                return 0;
            }
        }
        board->change=1;
    }

    fclose(fp);
    return 1;
}

#include <stdio.h>

struct Cell{
    int posx;
    int posy;
    int predefined = 0;
    int numOfPossibilities = 9;
    int posibleSolutions[9];
    int solved = 0;
};

struct route{
    int posxStart;
    int posxStop;
    int posyStart;
    int posyStop;
};

struct Board{
    struct Cell board[9][9];
    struct Route routes[3][3];
    int change=0;

};




int main()
{

   return 0;
}

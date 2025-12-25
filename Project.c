#include <stdio.h>
#include <stdlib.h>
#include <time.h>




void DFSCheck(char board[][23], int visited[][23],
              int rows, int columns, int x, int y)
{
    char horizontalWall = 205; // ═
    char verticalWall = 186; // ║
    visited[x][y] = 1;

    // Right
    if (y+2 < 2*columns - 1 && !visited[x][y+2] &&
        board[x][y+1] != verticalWall)
        DFSCheck(board, visited, rows, columns, x, y+2);

    // Left
    if (y-2 >= 0 && !visited[x][y-2] &&
        board[x][y-1] != verticalWall)
        DFSCheck(board, visited, rows, columns, x, y-2);

    // Up
    if (x+2 < 2*rows - 1 && !visited[x+2][y] &&
        board[x+1][y] != horizontalWall)
        DFSCheck(board, visited, rows, columns, x+2, y);

    // Down
    if (x-2 >= 0 && !visited[x-2][y] &&
        board[x-1][y] != horizontalWall)
        DFSCheck(board, visited, rows, columns, x-2, y);
}

int isBoardConnected(char board[][23], int n, int m)
{
    int visited[23][23] = {0};

    DFSCheck(board, visited, n, m, 0, 0);

    for (int i = 0; i < 2*n-1; i+=2)
        for (int j = 0; j < 2*m-1; j+=2)
            if (!visited[i][j])
                return 0;

    return 1;
}



int checkDistance(char board[][23],char from, int rows , int columns , int x , int y){
    for (int i = x-2; i <= x+2 ; i++) {
        if(i<0) i++;
        if(i>2*rows - 1) break;
        for (int j = y-2; j <= y+2; j++) {
            if(j<0) j++;
            if(j>2*columns - 1) break;
            if(board[i][j]== from) return 0;
        }
    }
    return 1;
}

void printBoard(char board[][23], int rows, int columns) {
    char horizontalWall = 205; // ═
    char verticalWall = 186; // ║
    for (int i = 0; i < 2*rows-1; i++) {
        printf("\n");
        for (int j = 0; j < 2*columns-1; j++) {
            printf("%c " , board[i][j]);
        }
    }
}
int generateRandNum(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}
int main(){
    int  n, m , adI , adJ ,shadyCount,playerCount, wallCount, orientation ;
    char horizontalWall = 205; // ═
    char verticalWall = 186; // ║
    char board[23][23]={'\0'};  // Create Board
    srand(time(NULL));
    //player='Ø' , surveyor='¤' , treasure='$';
    printf("Please Enter n and m: ");
    scanf(" %d %d", &n , &m);

    for (int i = 0; i < 2*n -1; i++) {
        for (int j = 0; j < 2*m - 1; j++) {
            if(i%2==1 && j%2==0) board[i][j]=196; // ─
            else if(i%2==1 && j%2==1) board[i][j]=197; // ┼
            else if(i%2==0 && j%2==1) board[i][j]=179; // │
        }

    }

    printf("Please Enter shady count: ");
    scanf(" %d", &shadyCount);
    for (int i = 0; i < shadyCount;i++) {
        adI = generateRandNum(0,n - 1);
        adJ = generateRandNum(0,m - 1);
        if(board[2*adI][2*adJ]!='\0'){
            i--;
            continue;
        }
        board[2*adI][2*adJ]=153; //Ø
    }

    printf("Please Enter player Count: ");
    scanf(" %d", &playerCount);
    for (int i = 0; i < playerCount;i++) {
        adI = generateRandNum(0,n - 1);
        adJ = generateRandNum(0,m - 1);
        if(board[2*adI][2*adJ]!='\0' || !checkDistance(board, 153 ,n , m, 2*adI, 2*adJ )){
            i--;
            continue;
        }
        board[2*adI][2*adJ]=164; //¤
    }

    do {
        adI = generateRandNum(0,n - 1);
        adJ = generateRandNum(0,m - 1);
    } while (board[2*adI][2*adJ]!='\0' || !checkDistance(board, 153 ,n , m, 2*adI, 2*adJ ) || !checkDistance(board, 164 ,n , m, 2*adI, 2*adJ ));
    board[2*adI][2*adJ]='$';

    printf("Please Enter Walls count: "); // Create Walls
    scanf(" %d",&wallCount);

    int maxWalls = (n-1)*(m-1);
    while(wallCount > maxWalls){
        printf("More than max walls. please enter again less than %d: " , maxWalls+1);
        scanf(" %d",&wallCount);

    }
    for (int i = 0 ; i < wallCount ; i++) {
        adI = generateRandNum(0,n - 1);
        adJ = generateRandNum(0,m - 1);
        orientation = generateRandNum(0,1);

        if(orientation== 1) {
            if(board[2*adI][2*adJ+1] == verticalWall || adJ == m - 1){
                i--;
                continue;
            }
            board[2 * adI][2 * adJ + 1] = verticalWall;
            if (!isBoardConnected(board, n , m)){
                board[2 * adI][2 * adJ + 1] = 179; // │
                i--;
                continue;
            }
        }


        else if(orientation== 0)  {
            if(board[2*adI+1][2*adJ] == horizontalWall || adI == n - 1){
                i--;
                continue;
            }
            board[2*adI+1][2*adJ]= horizontalWall;
            if (!isBoardConnected(board, n , m)){
                board[2 * adI+1][2 * adJ] = 196; // ─
                i--;
                continue;
            }
        }
    }

    printBoard(board, n, m);
    getchar();
    getchar();
    return 0;
}
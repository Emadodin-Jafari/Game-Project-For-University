#include <stdio.h>
#include <stdlib.h>
#include <time.h>
void printBoard(char board[][23], int rows, int columns) {
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
        if(board[2*adI][2*adJ]!='\0'){
            i--;
            continue;
        }
        board[2*adI][2*adJ]=164; //¤
    }

    do {
        adI = generateRandNum(0,n - 1);
        adJ = generateRandNum(0,m - 1);
    } while (board[2*adI][2*adJ]!='\0');
    board[2*adI][2*adJ]='$';

    printf("Please Enter Walls count: ");
    scanf(" %d",&wallCount);
    for (int i = 0 ; i < wallCount ; i++) {
       adI = generateRandNum(0,n - 1);
        adJ = generateRandNum(0,m - 1);
        orientation = generateRandNum(0,1);
        if(orientation== 1) {
            if(board[2*adI][2*adJ+1] == verticalWall ){
                i--;
                continue;
            }
            board[2 * adI][2 * adJ + 1] = verticalWall;
            }
        else if(orientation== 0)  {
            if(board[2*adI+1][2*adJ] == horizontalWall ){
                i--;
                continue;
            }
            board[2*adI+1][2*adJ]= horizontalWall;
        }
    }

    printBoard(board, n, m);
    getchar();
    getchar();
    return 0;
}



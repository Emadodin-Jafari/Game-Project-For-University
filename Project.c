#include <stdio.h>

int main(void){
    int  n, m , adI , adJ ,shadyCount,surveyorCount, wallCount ;
    char orientation;
    char board[23][23]={'\0'};
    //shady='!' , surveyor='?' , treasure='$';
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
        printf("Please Enter Shady Address: ");
        scanf(" %d %d" , &adI , &adJ);
        board[2*adI][2*adJ]='!';
    }

    printf("Please Enter surveyor Count: ");
    scanf(" %d", &surveyorCount);
    for (int i = 0; i < surveyorCount;i++) {
        printf("Please Enter surveyor Address: ");
        scanf(" %d %d" , &adI , &adJ);
        board[2*adI][2*adJ]='?';
    }

    printf("Please Enter Treasure Address: ");
    scanf(" %d %d" , &adI , &adJ);
    board[2*adI][2*adJ]='$';

    printf("Please Enter Walls count: ");
    scanf(" %d",&wallCount);
    for (int i = 0 ; i < wallCount ; i++) {
        printf("Please Enter Wall address: ");
        scanf(" %d %d %c" , &adI , &adJ,&orientation);
        if(orientation=='V') board[adI][adJ+1]=186; // ║
        else if(orientation=='H') board[adI+1][adJ]=179; // ═
    }

    for (int i = 0; i < 2*n-1; i++) {
        printf("\n");
        for (int j = 0; j < 2*m-1; j++) {
            printf("%c " , board[i][j]);
        }
    }

    getchar();
    getchar();
    return 0;
}



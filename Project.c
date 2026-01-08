#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <math.h>

#define GREEN   2
#define RED     4
#define WHITE   7

#define MAX_TEMP_WALLS 4

char horizontalWall = 205; // ═
char verticalWall = 186;   // ║

typedef struct {
    int x, y;
    char orientation;
    int life;
} tempWall;


int maxNumber(int a , int b){
    if(a >= b) return(a);
    else return(b);
}

int minNumber(int a , int b){
    if(a <= b) return(a);
    else return(b);
}

int maxWalls(int n , int m){
    int num =  maxNumber(minNumber(n , m) , 1);
    return(num);
}


void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void DFSCheck(char board[][23], int visited[][23], int rows, int columns, int x, int y) {
    visited[x][y] = 1;
    // Right
    if (y + 2 < 2 * columns - 1 && !visited[x][y + 2] && board[x][y + 1] != verticalWall)
        DFSCheck(board, visited, rows, columns, x, y + 2);
    // Left
    if (y - 2 >= 0 && !visited[x][y - 2] && board[x][y - 1] != verticalWall)
        DFSCheck(board, visited, rows, columns, x, y - 2);
    // Down
    if (x + 2 < 2 * rows - 1 && !visited[x + 2][y] && board[x + 1][y] != horizontalWall)
        DFSCheck(board, visited, rows, columns, x + 2, y);
    // Up
    if (x - 2 >= 0 && !visited[x - 2][y] && board[x - 1][y] != horizontalWall)
        DFSCheck(board, visited, rows, columns, x - 2, y);
}

int isBoardConnected(char board[][23], int n, int m) {
    int visited[23][23] = { 0 };
    DFSCheck(board, visited, n, m, 0, 0);
    for (int i = 0; i < 2 * n - 1; i += 2)
        for (int j = 0; j < 2 * m - 1; j += 2)
            if (!visited[i][j])
                return 0;
    return 1;
}

int checkDistance(char board[][23], char from, int rows, int columns, int x, int y) {
    for (int i = x - 2; i <= x + 2; i++) {
        if (i < 0) continue;
        if (i >= 2 * rows - 1) break;
        for (int j = y - 2; j <= y + 2; j++) {
            if (j < 0) continue;
            if (j >= 2 * columns - 1) break;
            if (board[i][j] == from) return 0;
        }
    }
    return 1;
}

void printBoard(char board[][23], int rows, int columns) {
    for (int i = 0; i < 2 * rows - 1; i++) {
        for (int j = 0; j < 2 * columns - 1; j++) {
            if (board[i][j] == horizontalWall || board[i][j] == verticalWall) {
                setColor(GREEN);
                printf("%c", board[i][j]);
                setColor(WHITE);
            }
            else {
                printf("%c", board[i][j]);            }
        }
        printf("\n");
    }
}

int generateRandNum(int min, int max) {
    return (rand() % (max - min + 1)) + min;
}

void gotoxy(int x, int y) {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD cursorCoord;
    cursorCoord.X = y; 
    cursorCoord.Y = x; 
    SetConsoleCursorPosition(consoleHandle, cursorCoord);
}

void clearScreen() {
    system("cls");
}

void clearMessage(int n) {
    gotoxy(2 * n + 1, 0);
    printf("                    "); 
}

int findNearestRunner(int hunterX , int hunterY , int Runner[][2] , int runnerCount){
    int minDistance = 1000000;
    int index = -1;

    for (int i = 0; i < runnerCount; i++) {
        int distance = abs(hunterX - Runner[i][0]) + abs(hunterY - Runner[i][1]);
        if (distance < minDistance) {
            minDistance = distance;
            index = i;
        }
    }
    return index;
}

void deleteRunner(int runnerCoordinates[][2] , int runnerCount , int x , int y){
    int i;
    for (i = 0; x!= runnerCoordinates[i][0] || y!= runnerCoordinates[i][1]; i++);
    for(;i<runnerCount ; i++){
        runnerCoordinates[i][0]=runnerCoordinates[i+1][0];
        runnerCoordinates[i][1]=runnerCoordinates[i+1][1];
    }
}

int placeTempWall(int x, int y, char orientation , char board[][23] , int rows , int columns , tempWall tempWalls[][10] , int RunnerNumber , int RunnerWalls[] , int MaxWalls) {
    int idx = RunnerWalls[RunnerNumber];
        if(RunnerWalls[RunnerNumber] >= MaxWalls) return(-1);

        if(orientation == 'V' || orientation=='v'){
           if(board[2*x][2*y +1] == verticalWall) return(-2);
           if(x >= rows || y >= columns -1 ) return(-3);
            tempWalls[RunnerNumber][idx].x = x;
            tempWalls[RunnerNumber][idx].y = y;
            tempWalls[RunnerNumber][idx].orientation = orientation;
            tempWalls[RunnerNumber][idx].life = 3;
            board[2*x][2*y +1] = verticalWall;
            gotoxy(2*x , 2*y +1);
            setColor(RED);
            printf("%c", board[2*x][2*y +1]);
            setColor(WHITE);
            RunnerWalls[RunnerNumber]++;
        }
        else if(orientation == 'H' || orientation=='h'){
            if(board[2*x + 1][2*y] == horizontalWall) return(-2);
            if(x >= rows -1 || y >= columns ) return(-3);
            tempWalls[RunnerNumber][idx].x = x;
            tempWalls[RunnerNumber][idx].y = y;
            tempWalls[RunnerNumber][idx].orientation = orientation;
            tempWalls[RunnerNumber][idx].life = 3;
            board[2*x + 1][2*y ] = horizontalWall;
            gotoxy(2*x + 1 , 2*y);
            setColor(RED);
            printf("%c", board[2*x + 1][2*y]);
            setColor(WHITE);
            RunnerWalls[RunnerNumber]++;
        }
    return 1;
}
void updateTempWalls(char board[][23], tempWall tempWalls[][10], int RunnerCount, int RunnerWalls[]) {
    for (int r = 0; r < RunnerCount; r++) {

        for (int i = 0; i < RunnerWalls[r]; i++) {

            tempWalls[r][i].life--;

            if (tempWalls[r][i].life <= 0) {

                int x = tempWalls[r][i].x;
                int y = tempWalls[r][i].y;

                if (tempWalls[r][i].orientation == 'V' || tempWalls[r][i].orientation == 'v' ) {
                    board[2*x][2*y + 1] = 179;
                    gotoxy(2*x, 2*y + 1);
                    printf("%c" , 179);
                }
                else if (tempWalls[r][i].orientation == 'H' || tempWalls[r][i].orientation == 'h') {
                    board[2*x + 1][2*y] = 196;
                    gotoxy(2*x + 1, 2*y);
                    printf("%c" , 196);
                }


                for (int j = i; j < RunnerWalls[r] - 1; j++) {
                    tempWalls[r][j] = tempWalls[r][j + 1];
                }

                RunnerWalls[r]--;
                i--;
            }
        }
    }
}




    int main() {
    int n, m, adI, adJ, wallCount, orientation, hunterCount, runnerCount, treasureX, treasureY , winNumber , MaxWalls;
    int hunterCoordinates[10][2]={{0}} , runnerCoordinates[10][2]={{0}};
    int RunnerWalls[10]={0};
    int Win = 0, Lose = -1;
    char board[23][23] = { '\0' };
    char runner = 'R';
    char hunter = 'H';
    char treasure = '$';
    tempWall TempWalls[10][MAX_TEMP_WALLS];
    int tempWallCount= 0;

    srand(time(NULL));

    
    printf("Please enter number of rows and columns (Max 12 12): ");
    scanf(" %d %d", &n, &m);

    MaxWalls = maxWalls(n , m);

    if (n > 12) n = 12; if (m > 12) m = 12;
    if (n < 2) n = 2;   if (m < 2) m = 2;

    
    for (int i = 0; i < 2 * n - 1; i++) { // Create Board
        for (int j = 0; j < 2 * m - 1; j++) {
            if (i % 2 == 1 && j % 2 == 0) board[i][j] = 196;      // ─
            else if (i % 2 == 1 && j % 2 == 1) board[i][j] = 197; // ┼
            else if (i % 2 == 0 && j % 2 == 1) board[i][j] = 179; // │
            else board[i][j] = ' ';
        }
    }

   
    printf("Please enter hunters' count: ");
    scanf(" %d", &hunterCount);
    for (int i = 0; i < hunterCount;) {
        hunterCoordinates[i][0] = generateRandNum(0, n - 1);
        hunterCoordinates[i][1] = generateRandNum(0, m - 1);
        if (board[2 * hunterCoordinates[i][0]][2 * hunterCoordinates[i][1]] != ' ')
            continue;

        board[2 * hunterCoordinates[i][0]][2 * hunterCoordinates[i][1]] = hunter;
        i++;
    }

    printf("Please enter runners' Count: ");
    scanf(" %d", &runnerCount);
    if(runnerCount % 3 == 0 ) winNumber = runnerCount/3;
    else winNumber = (runnerCount/3) + 1;
    for (int i = 0; i < runnerCount;) {
        runnerCoordinates[i][0] = generateRandNum(0, n - 1);
        runnerCoordinates[i][1] = generateRandNum(0, m - 1);
        if (board[2 * runnerCoordinates[i][0]][2 * runnerCoordinates[i][1]] != ' ' ||
            !checkDistance(board, hunter, n, m, 2 * runnerCoordinates[i][0], 2 * runnerCoordinates[i][1]))
            continue;

        board[2 * runnerCoordinates[i][0]][2 * runnerCoordinates[i][1]] = runner;
        i++;
    }

    do {
        treasureX = generateRandNum(0, n - 1);
        treasureY = generateRandNum(0, m - 1);
    } while (board[2 * treasureX][2 * treasureY] != ' ' ||
        !checkDistance(board, hunter, n, m, 2 * treasureX, 2 * treasureY) ||
        !checkDistance(board, runner, n, m, 2 * treasureX, 2 * treasureY));

    board[2 * treasureX][2 * treasureY] = treasure;


    printf("Please Enter Walls' count: "); // Create Walls
    scanf(" %d", &wallCount);

    int maxWalls = (n - 1) * (m - 1);
    while (wallCount > maxWalls) {
        printf("The entered number of walls is more than max number of walls. Please enter a number less than %d: ", maxWalls + 1);
        scanf(" %d", &wallCount);
    }
    for (int i = 0; i < wallCount; i++) {
        adI = generateRandNum(0, n - 1);
        adJ = generateRandNum(0, m - 1);
        orientation = generateRandNum(0, 1);

        if (orientation == 1) {
            if (adJ == m - 1 || board[2 * adI][2 * adJ + 1] == verticalWall) {
                i--; continue;
            }
            board[2 * adI][2 * adJ + 1] = verticalWall;
            if (!isBoardConnected(board, n, m)) {
                board[2 * adI][2 * adJ + 1] = 179;  // │ (remove wall)
                i--; continue;
            }
        }
        else {
            if (adI == n - 1 || board[2 * adI + 1][2 * adJ] == horizontalWall) {
                i--; continue;
            }
            board[2 * adI + 1][2 * adJ] = horizontalWall;
            if (!isBoardConnected(board, n, m)) {
                board[2 * adI + 1][2 * adJ] = 196; // ─ (remove wall)
                i--; continue;
            }
        }
    }

    clearScreen();
    printBoard(board, n, m);//end of board creation

    int turn = 0;
    do {
        if (turn == 0) {
            for (int l = 0; l < runnerCount; l++) {
                gotoxy(2*runnerCoordinates[l][0] , 2* runnerCoordinates[l][1]);


                int runnerMove = getch();
                int moved = 0;
                int invalid = 0;

                if (runnerMove == 0 || runnerMove == 224) {
                    runnerMove = getch();
                    switch (runnerMove) {
                        case 72:  // Up
                            if (2 * runnerCoordinates[l][0] - 2 >= 0 && board[2 * runnerCoordinates[l][0] - 1][2 * runnerCoordinates[l][1]] != horizontalWall && board[2 * runnerCoordinates[l][0] - 2][2 * runnerCoordinates[l][1]] != runner)  {
                                board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1]] = ' ';
                                gotoxy(2 * runnerCoordinates[l][0], 2 * runnerCoordinates[l][1]);
                                printf(" ");
                                runnerCoordinates[l][0] -= 1;
                                moved = 1;
                            } else invalid = 1;
                            break;
                        case 80: // Down
                            if (2 * runnerCoordinates[l][0] + 2 <= 2 * n - 1 && board[2 * runnerCoordinates[l][0] + 1][2 * runnerCoordinates[l][1]] != horizontalWall && board[2 * runnerCoordinates[l][0] + 2][2 * runnerCoordinates[l][1]] != runner) {
                                board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1]] = ' ';
                                gotoxy(2 * runnerCoordinates[l][0], 2 * runnerCoordinates[l][1]);
                                printf(" ");
                                runnerCoordinates[l][0] += 1;
                                moved = 1;
                            } else invalid = 1;
                            break;
                        case 75: // Left
                            if (2 * runnerCoordinates[l][1] - 2 >= 0 && board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1] - 1] != verticalWall && board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1] - 2] != runner) {
                                board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1]] = ' ';
                                gotoxy(2 * runnerCoordinates[l][0], 2 * runnerCoordinates[l][1]);
                                printf(" ");
                                runnerCoordinates[l][1] -= 1;
                                moved = 1;
                            } else invalid = 1;
                            break;
                        case 77: //Right
                            if (2 * runnerCoordinates[l][1] + 2 <= 2 * m - 1 && board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1] + 1] != verticalWall && board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1] + 2] != runner) {
                                board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1]] = ' ';
                                gotoxy(2 * runnerCoordinates[l][0], 2 * runnerCoordinates[l][1]);
                                printf(" ");
                                runnerCoordinates[l][1] += 1;
                                moved = 1;
                            } else invalid = 1;
                            break;
                    }
                } else if (runnerMove == 32) {
                    moved = 1;
                    clearMessage(n);
                    gotoxy(2 * n + 1, 0);
                    printf("Skipped Turn.");
                }
                else if( runnerMove == 'e' || runnerMove == 'E'){
                    int x , y , result;
                    char HOV;
                    clearMessage(n);
                    gotoxy(2 * n + 1, 0);
                    do {
                        printf("Enter wall coordinates: ");
                        scanf("%d %d %c" , &x , &y , &HOV);
                        result = placeTempWall(x , y , HOV , board , n , m , TempWalls , l , RunnerWalls , MaxWalls);
                    }while(result != 1);
                    turn = 1;
                }

                if (moved) {
                    clearMessage(n);
                    board[2 * runnerCoordinates[l][0]][2 * runnerCoordinates[l][1]] = runner;
                    gotoxy(2 * runnerCoordinates[l][0], 2 * runnerCoordinates[l][1]);
                    printf("%c", runner);
                    turn =1;
                }
                else if (invalid) {
                    gotoxy(2 * n + 1, 0);
                    printf("Invalid Move! ");
                    l--;
                }


                if (board[2*treasureX][2*treasureY] == runner){
                    Win++;
                    deleteRunner(runnerCoordinates , runnerCount , treasureX , treasureY);
                    runnerCount--;
                    board[2*treasureX][2*treasureY] = treasure;
                    gotoxy(2*treasureX , 2*treasureY);
                    printf("%c" , treasure);
                }

                if (Win == winNumber ){
                    clearMessage(n);
                    gotoxy(2 * n + 2, 0);
                    printf("You Win!\n");
                    break;
                }
                if (hunterCoordinates[l][0] == runnerCoordinates[l][0] && hunterCoordinates[l][1] == runnerCoordinates[l][1]){
                    deleteRunner(runnerCoordinates , runnerCount , runnerCoordinates[l][0] , runnerCoordinates[l][1]);
                    runnerCount--;
                    board[2*hunterCoordinates[l][0]][2*hunterCoordinates[l][1]] = hunter;
                    gotoxy(2*hunterCoordinates[l][0] , 2*hunterCoordinates[l][1]);
                    printf("%c" , hunter);
                }
            }
        }
        if(Win == winNumber) {
            break;
        }
        else if (turn == 1) {
            Sleep(200);
            for (int p = 0; p < hunterCount; p++) {


                for (int k = 0; k < 2; k++) {
                    int movedStep = 0;
                    int index = findNearestRunner(hunterCoordinates[p][0] , hunterCoordinates[p][1] , runnerCoordinates , runnerCount);
                    int runnerX , runnerY;
                    runnerX = runnerCoordinates[index][0];
                    runnerY = runnerCoordinates[index][1];
                    int dy = runnerY - hunterCoordinates[p][1];
                    int dx = runnerX - hunterCoordinates[p][0];

                    if (dy != 0) {
                        if (dy > 0) {
                            if (board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1] + 1] != verticalWall &&
                                board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1] + 2] != treasure &&
                                board[2*hunterCoordinates[p][0]][2*hunterCoordinates[p][1] + 2] != hunter ) {
                                board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1]] = ' ';
                                gotoxy(2 * hunterCoordinates[p][0], 2 * hunterCoordinates[p][1]);
                                printf(" ");
                                hunterCoordinates[p][1]++;
                                movedStep = 1;
                            }
                        } else {
                            if (board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1] - 1] != verticalWall &&
                                board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1] - 2] != treasure &&
                                board[2*hunterCoordinates[p][0]][2*hunterCoordinates[p][1] - 2] != hunter ) {
                                board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1]] = ' ';
                                gotoxy(2 * hunterCoordinates[p][0], 2 * hunterCoordinates[p][1]);
                                printf(" ");
                                hunterCoordinates[p][1]--;
                                movedStep = 1;
                            }
                        }
                    }
                    if (!movedStep && dx != 0) {
                        if (dx > 0) {
                            if (board[2 * hunterCoordinates[p][0] + 1][2 * hunterCoordinates[p][1]] != horizontalWall &&
                                board[2 * hunterCoordinates[p][0] + 2][2 * hunterCoordinates[p][1]] != treasure &&
                                board[2*hunterCoordinates[p][0] + 2][2*hunterCoordinates[p][1]] != hunter ) {
                                board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1]] = ' ';
                                gotoxy(2 * hunterCoordinates[p][0], 2 * hunterCoordinates[p][1]);
                                printf(" ");
                                hunterCoordinates[p][0]++;
                                movedStep = 1;
                            }
                        } else {
                            if (board[2 * hunterCoordinates[p][0] - 1][2 * hunterCoordinates[p][1]] != horizontalWall &&
                                board[2 * hunterCoordinates[p][0] - 2][2 * hunterCoordinates[p][1]] != treasure &&
                                board[2*hunterCoordinates[p][0] - 2][2*hunterCoordinates[p][1]] != hunter ) {
                                board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1]] = ' ';
                                gotoxy(2 * hunterCoordinates[p][0], 2 * hunterCoordinates[p][1]);
                                printf(" ");
                                hunterCoordinates[p][0]--;
                                movedStep = 1;
                            }
                        }
                    }

                    board[2 * hunterCoordinates[p][0]][2 * hunterCoordinates[p][1]] = hunter;
                    gotoxy(2 * hunterCoordinates[p][0], 2 * hunterCoordinates[p][1]);
                    printf("%c", hunter);

                    if (hunterCoordinates[p][0] == runnerX && hunterCoordinates[p][1] == runnerY) {
                        deleteRunner(runnerCoordinates , runnerCount , runnerX , runnerY);
                        runnerCount--;

                        if(runnerCount == 0){
                            Lose = 1;
                            break;
                        }
                    }
                    if (movedStep) Sleep(150);
                }
                turn = 0;

                if(runnerCount == 0){
                    break;
                }
            }
        }

        for (int i = 0; i < hunterCount && Lose!=1; i++) {
            for (int j = 0; j < runnerCount && Lose!=1; j++) {
                if (hunterCoordinates[i][0] == runnerCoordinates[j][0] && hunterCoordinates[i][1] == runnerCoordinates[j][1]) Lose = 1;
            }
        }

        updateTempWalls(board , TempWalls , runnerCount , RunnerWalls);
    } while (Win < winNumber && Lose == -1);

    clearMessage(n);
    gotoxy(2 * n + 2, 0);
    if (Lose == 1) printf("You Lose!\n");






    getchar(); 
    getchar();
    return 0;
}

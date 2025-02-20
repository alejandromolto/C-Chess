#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>


// structs and definitions

#define SIZECHESS 8

typedef struct{
int row;
int col; 
} T_Coordinates;

// functions definition

void mainmenu(int* option, char* language);
void settingsmenu(int* option, char* language);
void languagemenu(int* option, char* language);
void boardInitializer(int board[8][8]);
void askMovement(int movementCount, char* colPos, int* rowPos, char* colMove, int* rowMove, char* movingPiece, char language);
void coordsTranslator(int row, char col, T_Coordinates* coordinates);
void updateboard(int board[8][8], T_Coordinates actualLocation, T_Coordinates futurelocation);
void isThreatened(int board[8][8], int movementCount, T_Coordinates actualLocation, T_Coordinates futurelocation, bool* isItThreatened);
void isLegit(int board[8][8], int movementCount, T_Coordinates actualLocation, T_Coordinates futurelocation, bool* isItLegit);
void prohibitedMoves(int board[8][8], int movementCount, T_Coordinates prohibitedSquares[4096], int* nsquares);
void isTheKingChecked(int board[8][8], int movementCount, T_Coordinates prohibitedSquares[4096], int nsquares, bool* checked);
void isTheKingCheckMated(int board[8][8], int movementCount, T_Coordinates prohibitedSquares[4096], bool* matchOver);
void printboard(int board[8][8]);
void PawnPromotion(int board[8][8], bool* change, char language);

// main function

int main(){

//Variables & arrays initialization
int movementCount = 0;
char colPos;
int rowPos = 0;
char colMove;
int rowMove = 0;
char movingPiece;
bool matchOver = false;
bool isItLegit = false;
int option = 0;
char language = 's';
bool checked;
bool futurecheck;
int nsquares;
int dnsquares;
bool change;

T_Coordinates actualLocation;
T_Coordinates futureLocation;

T_Coordinates prohibitedSquares[4096];
T_Coordinates duplicateProhibitedSquares[4096];


int board[8][8];
int boardDuplicate[8][8];

// main menu

    mainmenu(&option, &language);

    if(option == 3){ //exit
        return 0;
    }

//initializations

    boardInitializer(board);
    printf("\n\n\n");
    printboard(board);

// game loop

    while(matchOver == false){

    do{
        change = false;

        //Looking for check/checkmate/stalemate
        nsquares = 0;
        for(int i = 0; i < 4096; i++){
            prohibitedSquares[i].row = 0;
            prohibitedSquares[i].col = 0;
        }

        prohibitedMoves(board, movementCount, prohibitedSquares, &nsquares);
        isTheKingChecked(board, movementCount, prohibitedSquares, nsquares, &checked);

        if(checked == true){
            isTheKingCheckMated(board, movementCount, prohibitedSquares, &matchOver);
            if(matchOver == true){
                printf("%s", (language == 's') ? "\nJaque mate.\n" : "\nYour king has been checkmated.\n");
                if(movementCount % 2 == 0){
                    printf("%s\n", (language == 's') ? "\nLas negras ganan. Fin de la partida.\n" : "Black wins. The match has been concluded.");
                }else{
                    printf("%s\n", (language == 's') ? "\nLas blancas ganan. Fin de la partida.\n" : "White wins. The match has been concluded.");
                }
            }else{
                printf("%s\n", (language == 's') ? "\nTu rey está en jaque.\n" : "\nYour king is in check.\n");
            }
        }else{
            isTheKingCheckMated(board, movementCount, prohibitedSquares, &matchOver);
            if(matchOver == true){
                printf("%s", (language == 's') ? "\nLa partida termina en tablas. Rey ahogado.\n" : "\nThe match has cocluded in a draw. Stalemate.\n");
            }
        }

        printf("\n\n\n");
        
        // User input (movement)

        if(matchOver == false){
        askMovement(movementCount, &colPos, &rowPos, &colMove, &rowMove, &movingPiece, language);

                if(movingPiece == 'F' && colPos == 'f' && rowPos == -1){
                    prohibitedMoves(board, movementCount, prohibitedSquares, &nsquares);

                    printf("\n\n\n");
                    for(int i = 0; i < nsquares; i++){
                        printf("\n %d %d", prohibitedSquares[i].row, prohibitedSquares[i].col);
                    }
                    askMovement(movementCount, &colPos, &rowPos, &colMove, &rowMove, &movingPiece, language);
                }

        coordsTranslator(rowPos, colPos, &actualLocation);
        coordsTranslator(rowMove, colMove, &futureLocation);
        isLegit(board, movementCount, actualLocation, futureLocation, &isItLegit);

        if(isItLegit == false){
            printf("%s", (language == 's') ? "\nEl movimiento no es válido. Inténtalo de nuevo.\n" : "\nThe movement is invalid. Try again.\n");
        }

        //To check if the movement can be performed (due to the fact that you cannot leave a king in check)
        //First we perform the move in a duplicate board

        for(int i = 0; i < 8; i++){
            for(int j = 0; j < 8; j++){
                boardDuplicate[i][j] = board[i][j];
            }
        }
        
        //Then we check if this move leaves the king on check

        futurecheck = false;

        updateboard(boardDuplicate, actualLocation, futureLocation);
        prohibitedMoves(boardDuplicate, movementCount, duplicateProhibitedSquares, &dnsquares);
        isTheKingChecked(boardDuplicate, movementCount, duplicateProhibitedSquares, dnsquares, &futurecheck);

        if(futurecheck == true){
            printf("%s", (language == 's') ? "\nNo puedes dejar a tu rey en jaque.\n" : "\nYou can't leave your king in check.\n");
        }

    }
    }while(isItLegit == false || futurecheck == true);
    
    if(matchOver == false){

  
    updateboard(board, actualLocation, futureLocation);
    printf("\n\n\n");
    printboard(board);
    PawnPromotion(board, &change, language);
    if(change){
        printf("\n");
        printboard(board);
    }
    movementCount++;    
    } 
    }

    return 0;
}

// functions code

void mainmenu(int* option, char *language){

//This function displays the main menu

bool playingchess;

printf("\n\n\n\n\n\n\n"); //This has the objective of separating clearly the two menus in the terminal.

// main menu


if(*language == 'e'){ // menu in english
    printf("**WELCOME TO C-CHESS**\n"
       "Work done by alejandromolto.\n\n\n"
       "OPTIONS: \n\n"
       "(1) Play chess. \n"
       "(2) Settings \n"
       "(3) Exit \n\n\n");
}else if(*language == 's'){ // menu in spanish
    printf("**BIENVENIDO A C-CHESS**\n"
       "Trabajo hecho por alejandromolto.\n\n\n"
       "OPCIONES: \n\n"
       "(1) Jugar al ajedrez. \n"
       "(2) Opciones \n"
       "(3) Salida \n\n\n");   
}


do{
    
printf(*language == 'e' ? "Choose your option: " : "Elige tu opción: ");
scanf(" %d", option);

if (*option > 3 || *option < 1) {
    printf(*language == 'e' ? "Please choose a valid option: " : "Porfavor, elige una opción válida:");
}

}while(*option > 3 || *option < 1);


//If the choice of the user is either "playing"(1) or "exiting"(3), the menu ends. But in the case the choice is going into the "settings menu" (2),
// then there is a submenu inside the main menu.

if(*option == 1 || *option == 3){
    return;
} else if(*option == 2){
    settingsmenu(option, language);
} 
}

void settingsmenu(int* option, char* language){

//This function displays the settings menu

    printf("\n\n\n\n\n\n\n"); //This has the objective of separating clearly the menus in the terminal.
    printf("%s", *language == 'e' ? "**SETTINGS MENU**\n\n(1) Change language\n(2) Exit\n" : "**MENÚ DE CONFIGURACIÓN**\n\n(1) Cambiar idioma\n(2) Salir\n");
    printf("\n");
    
    do{
        printf(*language == 'e' ? "Choose your option: " : "Elige tu opción: ");
        scanf(" %d", option);

        if (*option > 2 || *option < 1) {
        printf(*language == 'e' ? "Please choose a valid option: " : "Porfavor, elige una opción válida:");
        }

    }while(*option > 2 || *option < 1);


    if(*option == 1){
        languagemenu(option, language);
    } else if(*option == 2){
        mainmenu(option, language);
    }
}

void languagemenu(int* option, char* language){

//This function displays the language menu

        printf("\n\n\n\n\n\n\n"); //This has the objective of separating clearly the two menus in the terminal.
        printf("%s", *language == 'e' ? "**LANGUAGE MENU**\n\n(1) Spanish\n(2) English \n(3) Exit\n" : "**MENÚ DE IDIOMA**\n\n(1) Español\n(2) Inglés \n(3) Salir");
        printf("\n\n");
            do{
            printf(*language == 'e' ? "Choose your option: " : "Elige tu opción: ");
            scanf("%d", option);

            if (*option > 3 || *option < 1) {
            printf(*language == 'e' ? "Please choose a valid option: " : "Porfavor, elige una opción válida:");
            }

            }while(*option > 3 || *option < 1);

            if(*option == 1){
                *language = 's';
                printf("\nLenguaje cambiado con éxito.");
                settingsmenu(option, language);

            }else if(*option == 2){
                *language = 'e';
                printf("\nLanguage successfully changed.");
                settingsmenu(option, language);

            }

}

void boardInitializer(int board[8][8]){

//This function defines the initial state of the chess board.

    int chessBoard[8][8] = {
        {04, 06, 05, 03, 02, 05, 06, 04},
        {01, 01, 01, 01, 01, 01, 01, 01},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0},
        {11, 11, 11, 11, 11, 11, 11, 11},
        {14, 16, 15, 13, 12, 15, 16, 14}
};

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            board[i][j] = chessBoard[i][j];
        }
    }

}

void askMovement(int movementCount, char* colPos, int* rowPos, char* colMove, int* rowMove, char* movingPiece, char language){

//This function is used to ask the user for their moves and stores every input given by the user as independent variables.

    if(movementCount % 2 == 0){
        printf("%s", (language == 's') ? "\nEs el turno de las blancas. Cuál es la pieza que quieres mover y donde está localizada: " :   "\nIt is white's turn. What is the piece you want to move and where is it located: ");
        scanf(" %c %c %d", movingPiece, colPos, rowPos);
        printf("%s", (language == 's') ? "\nY dónde la quieres mover: " : "\nAnd where do you want it to move: ");        
        scanf(" %c %d", colMove, rowMove);

    } else{
        printf("%s", (language == 's') ? "\nEs el turno de las negras. Cuál es la pieza que quieres mover y donde está localizada: " :   "\nIt is black's turn. What is the piece you want to move and where is it located: ");
        scanf(" %c %c %d", movingPiece, colPos, rowPos);
        printf("%s", (language == 's') ? "\nY dónde la quieres mover: " : "\nAnd where do you want it to move: ");        
        scanf(" %c %d", colMove, rowMove);

    }
}

void coordsTranslator(int row, char col, T_Coordinates* coordinates){

//This function converts the coordinates from chess notation (per example, f4) to matrix notation (4, 5).

    if (col == 'a') { 
        coordinates->col = 0;
    } else if (col == 'b') { 
        coordinates->col = 1;
    } else if (col == 'c') { 
        coordinates->col = 2;
    } else if (col == 'd') { 
        coordinates->col = 3;
    } else if (col == 'e') { 
        coordinates->col = 4;
    } else if (col == 'f') { 
        coordinates->col = 5;
    } else if (col == 'g') { 
        coordinates->col = 6;
    } else if (col == 'h') { 
        coordinates->col = 7;
    }

coordinates->row = 8 - row;

}

void updateboard(int board[8][8], T_Coordinates actualLocation, T_Coordinates futurelocation){

//This function modifies the board state based on the player's move.

board[futurelocation.row][futurelocation.col] = board[actualLocation.row][actualLocation.col];
board[actualLocation.row][actualLocation.col] = 0;


}

void isLegit(int board[8][8], int movementCount, T_Coordinates actualLocation, T_Coordinates futurelocation, bool* isItLegit){

//This function checks whether the move is legitimate or not.

//FIRST PHASE: GENERAL CHECK.

//Now we are going to check some general cases, and if the moves break any of the general rules (such as moving a opponents piece)
// Is the piece moving to a valid place? 

if(actualLocation.col > 7 || actualLocation.col < 0 || actualLocation.row > 7 || actualLocation.row < 0){
    *isItLegit = false;
    return;
}

if(futurelocation.col > 7 || futurelocation.col < 0 || futurelocation.row > 7 || futurelocation.row < 0){
    *isItLegit = false;
    return;
}

// Is the piece moving at all?

if(actualLocation.col == futurelocation.col && actualLocation.row == futurelocation.row){
    *isItLegit = false;
    return;
}
// Does the piece exist?

if((board[actualLocation.row][actualLocation.col] % 10) == 0){
    *isItLegit = false;
    return;
}

// Is the user moving the right color piece? (If movementCount is even then the piece must be white and if it is odd the piece must be black)

if((movementCount % 2 == 0 && (board[actualLocation.row][actualLocation.col] / 10) == 0) ||
   (movementCount % 2 != 0 && (board[actualLocation.row][actualLocation.col] / 10) == 1)){

    *isItLegit = false;
    return;

}

// Is the piece moving to a square occupied by a piece of its own color?

if(movementCount % 2 == 0 && board[futurelocation.row][futurelocation.col] / 10 == 1 ||
   movementCount % 2 != 0 && board[futurelocation.row][futurelocation.col] / 10 == 0 && 
   board[futurelocation.row][futurelocation.col] % 10 != 0){

    *isItLegit = false;
    return;

}

//SECOND PHASE: SPECIFIC CHECK.
*isItLegit = false;

int dx = futurelocation.col - actualLocation.col;
int dy = futurelocation.row - actualLocation.row;

// WHITE PAWNS (Moving up the board)
if (board[actualLocation.row][actualLocation.col] % 10 == 1 && movementCount % 2 == 0) {
    if ((dx == 0 && dy == -1 && board[futurelocation.row][futurelocation.col] / 10 == 0) ||  // Forward one step
        (dx == 0 && dy == -2 && actualLocation.row == 6 &&  // Double step from start
        board[futurelocation.row][futurelocation.col] / 10 == 0 &&
        board[actualLocation.row - 1][actualLocation.col] / 10 == 0) ||
        ((dx == 1 || dx == -1) && dy == -1 && board[futurelocation.row][futurelocation.col] / 10 == 0)){ // Capturing diagonally
        
        *isItLegit = true;
    }
    return;
}

// BLACK PAWNS (Moving down the board)
if (board[actualLocation.row][actualLocation.col] % 10 == 1 && movementCount % 2 != 0) {
    if ((dx == 0 && dy == 1 && board[futurelocation.row][futurelocation.col] / 10 == 0) ||  // Forward one step
        (dx == 0 && dy == 2 && actualLocation.row == 1 &&  // Double step from start
        board[futurelocation.row][futurelocation.col] / 10 == 0 &&
        board[actualLocation.row + 1][actualLocation.col] / 10 == 0) ||
        ((dx == 1 || dx == -1) && dy == 1 && board[futurelocation.row][futurelocation.col] / 10 != 0)) { // Capturing diagonally

        *isItLegit = true;
    }
    return;
}

// KING (moving everywhere)
if(board[actualLocation.row][actualLocation.col] % 10 == 2){
    if(dx >= -1 && dx <= 1 && dy >= -1 && dy <= 1){
        *isItLegit = true;
        return;
    }
}

// KNIGHT (moving in L)
if (board[actualLocation.row][actualLocation.col] % 10 == 6) { 
    if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) {
        *isItLegit = true;
        return;
    }
}

// ROOK (moving straight)

if(board[actualLocation.row][actualLocation.col] % 10 == 4){

if((dx != 0 && dy == 0) || (dx == 0 && dy != 0)){

if(dx == 1 || dx == -1 || dy == 1 || dy == -1){
    *isItLegit = true;
    return;
}


//#1 (The rook is moving to the right)
if(dx > 0){
    for(int i = actualLocation.col + 1; i < futurelocation.col; i++){
        if((board[actualLocation.row][i] % 10) == 0){              
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
    }

//#2 (The rook is moving to the left)
}else if(dx < 0){
    for(int i = futurelocation.col + 1; i < actualLocation.col; i++){
        if((board[actualLocation.row][i] % 10) == 0){                      
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
    }
}


// vertical movement
//#3 (The rook is moving down)
if(dy > 0){
    for(int j = actualLocation.row + 1; j < futurelocation.row; j++){
        if((board[j][actualLocation.col] % 10) == 0){                         
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
//#4(The rook is moving up)

    }
    
    }else if(dy < 0){
    for(int j = actualLocation.row - 1; j > futurelocation.row; j--){
        if((board[j][actualLocation.col] % 10) == 0){                        
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
    }
}
}
}

// BISHOP (moving diagonally)

if(board[actualLocation.row][actualLocation.col] % 10 == 5){

if(abs(dx) == abs(dy)){

if(abs(dx) == 1){
    *isItLegit = true;
    return;
} 

//#1: moving to the second quadrant
    if(dx < 0 && dy < 0){
        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row + i][futurelocation.col + i] == 0){
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }
//#2: moving to the first quadrant

    } else if(dx > 0 && dy < 0){

        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row + i][futurelocation.col - i] == 0){   
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }
//#3: moving to the third quadrant

    } else if(dx < 0 && dy > 0){
        
        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row - i][futurelocation.col + i] == 0){
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }
//#4: moving to the fourth quadrant

    } else if(dx > 0 && dy > 0){

        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row - i][futurelocation.col - i] == 0){
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }

    }

}else{
    *isItLegit = false;
    return;
}
}


//QUEEN (moving both diagonaly and straight)

if(board[actualLocation.row][actualLocation.col] % 10 == 3){

    //If its moving like a rook (same code).


if((dx != 0 && dy == 0) || (dx == 0 && dy != 0)){

if(dx == 1 || dx == -1 || dy == 1 || dy == -1){
    *isItLegit = true;
    return;
}


if(dx > 0){
    for(int i = actualLocation.col + 1; i < futurelocation.col; i++){
        if((board[actualLocation.row][i] % 10) == 0){              
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
    }

}else if(dx < 0){
    for(int i = futurelocation.col + 1; i < actualLocation.col; i++){
        if((board[actualLocation.row][i] % 10) == 0){                      
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
    }
}

if(dy > 0){
    for(int j = actualLocation.row + 1; j < futurelocation.row; j++){
        if((board[j][actualLocation.col] % 10) == 0){                         
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
        }
 

    }else if(dy < 0){
    for(int j = actualLocation.row - 1; j > futurelocation.row; j--){
        if((board[j][actualLocation.col] % 10) == 0){                        
            *isItLegit = true;
        }else{
            *isItLegit = false;
            return;
        }
    }
}
}

    
    //If its moving like a bishop (same code).

if(abs(dx) == abs(dy)){

if(abs(dx) == 1){
    *isItLegit = true;
    return;
} 

    if(dx < 0 && dy < 0){
        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row + i][futurelocation.col + i] == 0){
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }

    } else if(dx > 0 && dy < 0){

        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row + i][futurelocation.col - i] == 0){   
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }

    } else if(dx < 0 && dy > 0){

        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row - i][futurelocation.col + i] == 0){
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }

    } else if(dx > 0 && dy > 0){

        for(int i = 1; i < (abs(dx)); i++){
            if(board[futurelocation.row - i][futurelocation.col - i] == 0){
                *isItLegit = true;
            }else{
                *isItLegit = false;
                return;
            }
        }

    }

}

}

}

void isThreatened(int board[8][8], int movementCount, T_Coordinates actualLocation, T_Coordinates futurelocation, bool* isItThreatened){

*isItThreatened = false;

//This function checks if a square of the board is threteaned by a piece, by taking as input a particular movement (with the actual location and the future location)
//and returns the condition isItThreatened with wether or not the square can be considered threatened.

//If the piece is not a pawn, then the condition "threatened" is equal to the condition "legit",thus if the piece is not a pawn, then isItThreatened is equal to isItLegit:

if(board[actualLocation.row][actualLocation.col] % 10 == 2 ||
         board[actualLocation.row][actualLocation.col] % 10 == 3 || 
         board[actualLocation.row][actualLocation.col] % 10 == 4 ||
         board[actualLocation.row][actualLocation.col] % 10 == 5 ||
         board[actualLocation.row][actualLocation.col] % 10 == 6){

            isLegit(board, movementCount, actualLocation, futurelocation, isItThreatened);
         }


//Now, if the piece is a pawn, some special rules need to be added.

//GENERAL CHECK for pawns:


if(actualLocation.col > 7 || actualLocation.col < 0 || actualLocation.row > 7 || actualLocation.row < 0){
    *isItThreatened = false;
    return;
}

if(futurelocation.col > 7 || futurelocation.col < 0 || futurelocation.row > 7 || futurelocation.row < 0){
    *isItThreatened = false;
    return;
}


if(actualLocation.col == futurelocation.col && actualLocation.row == futurelocation.row){
    *isItThreatened = false;
    return;
}

if((board[actualLocation.row][actualLocation.col] % 10) == 0){
    *isItThreatened = false;
    return;
}


if((movementCount % 2 == 0 && (board[actualLocation.row][actualLocation.col] / 10) == 0) ||
   (movementCount % 2 != 0 && (board[actualLocation.row][actualLocation.col] / 10) == 1)){

    *isItThreatened = false;
    return;

}


if(movementCount % 2 == 0 && board[futurelocation.row][futurelocation.col] / 10 == 1 ||
   movementCount % 2 != 0 && board[futurelocation.row][futurelocation.col] / 10 == 0 && 
   board[futurelocation.row][futurelocation.col] % 10 != 0){

    *isItThreatened = false;
    return;

}

//SPECIAL RULES.
//The position is threatened by a pawn only if its diagonal

int dx = futurelocation.col - actualLocation.col;
int dy = futurelocation.row - actualLocation.row;


if(board[actualLocation.row][actualLocation.col] % 10 == 1){
    //White pawns
    if(board[actualLocation.row][actualLocation.col] / 10 == 1){
        if((dx == 1 || dx == -1) && dy == -1){
            *isItThreatened = true;
        }

    //Black pawns
    }else{
        if((dx == 1 || dx == -1) && dy == 1){
            *isItThreatened = true;
        }


    }
}



}

void prohibitedMoves(int board[8][8], int movementCount, T_Coordinates prohibitedSquares[4096], int* nsquares){

//This function takes the board and the movementCount and generates a list of squares where the king corresponding to the current colour cant go.


bool isItLegit;

int nblackprohibitedmoves = 0;
int nwhiteprohibitedmoves = 0;

int nextMovementCount = movementCount + 1;

for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
        if(movementCount % 2 != 0 && board[i][j] / 10 == 1 && board[i][j] % 10 != 0){    //black's moving & the piece is white...

            

            for(int k = 0; k < 8; k++){
                for(int t = 0; t < 8; t++){ //... the program checks every possible move for that piece...

                    T_Coordinates actualLocation;
                    actualLocation.row = i;
                    actualLocation.col = j;
                    
                    T_Coordinates futurelocation;
                    futurelocation.row = k;
                    futurelocation.col = t;

                    isThreatened(board, nextMovementCount, actualLocation, futurelocation, &isItLegit);  //... checks if its legit or not...

                    if(isItLegit){
                        prohibitedSquares[nblackprohibitedmoves].row = futurelocation.row;
                        prohibitedSquares[nblackprohibitedmoves].col = futurelocation.col;    // ...and the legit ones are saved in an array.
                        nblackprohibitedmoves = nblackprohibitedmoves + 1;
                    }



                }
            }


        } else if(movementCount % 2 == 0 && board[i][j] / 10 == 0 && board[i][j] % 10 != 0){ //white's moving & the piece is black

            
            for(int k = 0; k < 8; k++){
                for(int t = 0; t < 8; t++){ //... the program checks every possible move for that piece...

                    T_Coordinates actualLocation;
                    actualLocation.row = i;
                    actualLocation.col = j;

                    T_Coordinates futurelocation;
                    futurelocation.row = k;
                    futurelocation.col = t;
                    
                    isThreatened(board, nextMovementCount, actualLocation, futurelocation, &isItLegit);  //... checks if its legit or not...

                    if(isItLegit){
                        prohibitedSquares[nwhiteprohibitedmoves].row = futurelocation.row;
                        prohibitedSquares[nwhiteprohibitedmoves].col = futurelocation.col;    // ...and the legit ones are saved in an array.
                        nwhiteprohibitedmoves = nwhiteprohibitedmoves + 1;
                    }

                }
            }




        } 
    }
}

//As only one thing can be true (wether the black king is moving or the white king is moving), the nsquares variable returns the number of squares prohibited
//corresponding only to the king that is moving.

if(nwhiteprohibitedmoves != 0){
    *nsquares = nwhiteprohibitedmoves;
}else{
    *nsquares = nblackprohibitedmoves;
}

}

void isTheKingChecked(int board[8][8], int movementCount, T_Coordinates prohibitedSquares[4096], int nsquares, bool* checked){
    
//This function checks wether the king is checked or not.

    *checked = false;

    int kingrow;
    int kingcol;

    //We locate the king of the corresponding color

    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(board[i][j] % 10 == 2 &&((board[i][j] / 10 == 0 && movementCount % 2 != 0)||
            (board[i][j] / 10 == 1 && movementCount % 2 == 0))){                                
                
                kingrow = i;
                kingcol = j;

            }
        }
    }

    //Then check if the king is in any of the checked positions.

    for(int k = 0; k < nsquares; k++){
        if(prohibitedSquares[k].row == kingrow && prohibitedSquares[k].col == kingcol){
            *checked = true;
        }   
    }

}

void isTheKingCheckMated(int board[8][8], int movementCount, T_Coordinates prohibitedSquares[4096], bool* matchOver){

//This function checks if there exists a legal move such that the king is not in check. It can therefore indicate two different situations:

// If its executed with the king being in check and it returns "true" in the value matchOver, it means that the king is checkmated, and it means a victory for the opposing player. 
// If its executed with the king NOT being in check and it returns "true" in the value matchOver, it means a Stalemate, and therefore a draw. 

bool isItThreatened;
bool checked;
int nsquares;
T_Coordinates prohibitedSquaresD[4096];
int savingMoves = 0;

//The function must check if there is any legal move that can save the king ¿How?.
//Well using the function isTheKingChecked and prohibitedMoves we can calculate if there is any legal move that makes the king NOT checked in the next move. 


//To do that, we duplicate the chess board for trials (to try every possible move)

int board2[8][8];


for(int x = 0 ; x < 8; x++){
    for(int y = 0; y < 8; y++){
        board2[x][y] = board[x][y];
    }
}


//Then the magic comes. We check every piece.

for(int i = 0; i < 8; i++){
    for(int j = 0; j < 8; j++){
        if(movementCount % 2 == 0 && board[i][j] / 10 == 1 && board[i][j] % 10 != 0){    //If white is moving & the piece is white (which means that white is checked)...

            

            for(int k = 0; k < 8; k++){
                for(int t = 0; t < 8; t++){ //... the program checks every possible move for that piece...

                    for(int x = 0 ; x < 8; x++){
                        for(int y = 0; y < 8; y++){             //(we restart the duplicated board in every execution) 
                            board2[x][y] = board[x][y];
                        }
                    }

                    T_Coordinates actualLocation;
                    actualLocation.row = i;
                    actualLocation.col = j;
                    T_Coordinates futurelocation;
                    futurelocation.row = k;
                    futurelocation.col = t;

                    isThreatened(board, movementCount, actualLocation, futurelocation, &isItThreatened);  //... and checks if its legit or not.
                

                    if(isItThreatened){ //If the movement is legit... 
                        updateboard(board2, actualLocation, futurelocation); //... we perform it in a duplicated board... 
                        prohibitedMoves(board2, movementCount, prohibitedSquaresD, &nsquares); //... we calculate the checked squares in that duplicate board..
                        isTheKingChecked(board2, movementCount, prohibitedSquaresD, nsquares, &checked); //... and we look if the king is still checked.

                        if(checked == false){ //if there is at least one legal move where the king is still not checked, the king is not checkmated.
                            *matchOver = false;
                            savingMoves = savingMoves + 1;
                            return;
                        }
                    }



                }
            }


        } else if(movementCount % 2 != 0 && board[i][j] / 10 == 0 && board[i][j] % 10 != 0){ //If black is moving & the piece is black (which means that black is checked)...

            for(int k = 0; k < 8; k++){
                for(int t = 0; t < 8; t++){ //... the program checks every possible move for that piece...

                    T_Coordinates actualLocation;
                    actualLocation.row = i;
                    actualLocation.col = j;
                    T_Coordinates futurelocation;
                    futurelocation.row = k;
                    futurelocation.col = t;


                    for(int x = 0 ; x < 8; x++){
                        for(int y = 0; y < 8; y++){             //(we restart the duplicated board in every execution) 
                            board2[x][y] = board[x][y];
                        }
                    }

                    isThreatened(board, movementCount, actualLocation, futurelocation, &isItThreatened);  //... and checks if its legit or not.
                

                    if(isItThreatened){ //If the movement is legit... 
                        updateboard(board2, actualLocation, futurelocation); //... we perform it in a duplicated board... 
                        prohibitedMoves(board2, movementCount, prohibitedSquaresD, &nsquares); //... we calculate the checked squares in that duplicate board..
                        isTheKingChecked(board2, movementCount, prohibitedSquaresD, nsquares, &checked); //... and we look if the king is still checked.

                        if(checked == false){ //if there is at least one legal move where the king is still not checked, the king is not checkmated.
                            *matchOver = false;
                            savingMoves = savingMoves + 1;
                            return;

                        }
                    }

                    }
                }
            } 
        }
   }
    
    if(savingMoves == 0){
        *matchOver = true;
    }


}

void PawnPromotion(int board[8][8], bool* change, char language){   

//This function serves the purpose of promoting the pawns into either a queen, a knight, a bishop or a rook (depending on the input of the user).

char piece = 'Q';

    //For white pawns
    for(int i = 0; i < 8; i++){
        if(board[0][i] % 10 == 1){
            *change = true;
            do{
                printf("%s", (language == 's') ? "\n¿Que quieres cambiar por tu peón? Reina (Q), Caballo (N), Torre(R), Alfil (B): " : "\nWhat do you want to trade your pawn for? Queen (Q), Knight (N), Rook(R), Bishop (B): ");
                scanf(" %c", &piece);
                printf("\n");

            if(piece != 'Q' && piece != 'R' && piece != 'N' && piece != 'B' ){
                  printf("%s", (language == 's') ? "\nPor favor, introduce una pieza válida.\n" : "\nPlease introduce a valid piece\n");
            }

            }while(piece != 'Q' && piece != 'R' && piece != 'N' && piece != 'B' );

            switch(piece){
                case 'Q':   
                board[0][i] = 13;
                break;

                case 'B':
                board[0][i] = 15;
                break;

                case 'N':
                board[0][i] = 16;
                break;
            }

        }
    }

    //For black pawns
    for(int j = 0; j < 8; j++){
        if(board[7][j] % 10 == 1){
            *change = true;
            do{
            printf("%s", (language == 's') ? "\nQue quieres cambiar por tu peón: Reina (Q), Caballo (N), Torre(R), Alfil (B)\n" : "\nWhat do you want to trade your pawn for: Queen (Q), Knight (N), Rook(R), Bishop (B)\n");
            scanf("%c", &piece);
            printf("\n");

            if(piece != 'Q' && piece != 'R' && piece != 'N' && piece != 'B' ){
                  printf("%s", (language == 's') ? "\nPor favor, introduce una pieza válida.\n" : "\nPlease introduce a valid piece\n");
            }

            }while(piece != 'Q' && piece != 'R' && piece != 'N' && piece != 'B' );

            switch(piece){
                case 'Q':   
                board[7][j] = 03;
                break;

                case 'B':
                board[7][j] = 05;
                break;

                case 'N':
                board[7][j] = 06;
                break;
            }

        }
    }

}

void printboard(int board[8][8]) {

//This function displays the current state of the board.

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            switch ((board[i][j]) % 10) {
                case 0:  printf(". "); break; 
                case 1:  printf("P "); break;
                case 2:  printf("K "); break;
                case 3:  printf("Q "); break;
                case 4:  printf("R "); break;
                case 5:  printf("B "); break;
                case 6:  printf("N "); break;
                default: printf("? "); break; 
            }
        }
        printf("\n");
    }
}


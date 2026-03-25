/* ============================= */
/*        STRUCTURES             */
/* ============================= */

typedef struct
{
    int good;
    int go;
    int start;
    int over;
    int found;
    int val;

    int R[4][4];
    int B[4][4];
    int S[4][4];
    int T[4][4];

} GameState;


/* ============================= */
/*     FUNCTION PROTOTYPES       */
/* ============================= */

/* Initialization */
void initializeGame(GameState *g)
{
    int i, j;

    g->valid = 0; // 0 if invalid move, 1 if valid move
    g->go = 0; // 0 if red, 1 if blue
    g->start = 1; // 0 if normal gameplay, 1 if starting
    g->over = 0; // 0 if game running, 1 if game ends
    g->found = 0; 
    g->val = 0; // general counting variable for values

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            g->R[i][j] = 0; //red
            g->B[i][j] = 0; //blue
            g->S[i][j] = 0; //state (if may naka-occupy)
            g->T[i][j] = 0; //temp if needed
        }
    }
}

/* Display */
void displayBoard(GameState g)
{
    /*
    hi kung sino man gagawa nito, i think need mo magsearch sa web para sa colors
    need kasi madisplay yung blue and red sa board i think...
    - shen :>

    btw ok lang ata kahit iba nag-author kasi tinanong ko friend ko ano
    ginawa nila sa colors tas sabi nila kumuha lang raw sila header file
    sa web tapos inapply nalang nila sa code nila
    */
}

/* Input */
void getMove(int *row, int *col);

int isValidPos(int row, int col)
{
    int valid = 0;

    if (row>=0 && row<4 && col>=0 && col<4)
        valid = 1;

    return valid;
}

/* Game Actions */
void removePos(GameState *g, int row, int col)
{
    int changeRow[4] = {-1, 1, 0, 0};
    int changeCol[4] = {0, 0, -1, 1};

    int i;
    int neighborRow, neighborCol;

    for (i-0; i<4; i++)
    {
        neighborRow = row + changeRow[i];
        neighborCol = col + changeCol[i];
        // bali mangyayari is ccheck nya ung (-1,0) (0,1) and so on from the position

        //check kung nasa loob pa yung neighbors
        if (isValidPos(neighborRow, neighborCol)==1)
        {
            //RED TURN
            if (g->go == 0)
            {
                //check kung may blue kami
                if (g->B[neighborRow][neighborCol] == 1)
                {
                    //remove blue piece
                    g->B[neighborRow][neighborCol] = 0;
                    //update state
                    g->S[neighborRow][neighborCol] = 0;
                }
            }

            //BLUE TURN
            else
            {
                if (g->R[neighborRow][neighborCol] == 1)
                {
                    //remove red piece
                    g->R[neighborRow][neighborCol] = 0;
                    //update state
                    g->R[neighborRow][neighborCol] = 0;
                }
            }
        }
    }
}

void replacePos(GameState *g, int row, int col)
{
   int changeRow[4] = {-1, 1, 0, 0};
    int changeCol[4] = {0, 0, -1, 1};

    int i;
    int neighborRow, neighborCol;

    for (i-0; i<4; i++)
    {
        neighborRow = row + changeRow[i];
        neighborCol = col + changeCol[i];
        // bali mangyayari is ccheck nya ung (-1,0) (0,1) and so on from the position

        //check kung nasa loob pa yung neighbors
        if (isValidPos(neighborRow, neighborCol)==1)
        {
            //RED TURN
            if (g->go == 0)
            {
                //check kung may blue kami
                if (g->B[neighborRow][neighborCol] == 1)
                {
                    //remove blue piece
                    g->B[neighborRow][neighborCol] = 0;
                    //replace to red
                    g->R[neighborRow][neighborCol] = 1;
                    //update state
                    g->S[neighborRow][neighborCol] = 1;
                }
            }

            //BLUE TURN
            else
            {
                if (g->R[neighborRow][neighborCol] == 1)
                {
                    //remove red piece
                    g->R[neighborRow][neighborCol] = 0;
                    //replace to red
                    g->B[neighborRow][neighborCol] = 1;
                    //update state
                    g->R[neighborRow][neighborCol] = 1;
                }
            }
        }
    }
}

void expandPos(GameState *g, int row, int col);
void updatePos(GameState *g, int row, int col);
void nextPlayerMove(GameState *g, int row, int col);

/* Utility Functions */
int countPieces(int board[4][4]);
int countFreeCells(GameState g);

/* Game Status */
void checkGameOver(GameState *g);

void showResult(GameState g)
{
    int nRed, nBlue; // pang count sa pieces

    nRed = countPieces(g.R); //array pointing to red pieces nd so on..
    nBlue = countPieces(g.B);

    printf("| ------- GAME OVER ------- |"); // idk pa ano display adjust k nalang soon
    printf("Red pieces: %d\n", nRed);
    printf("Blue pieces: %d\n", nBlue);

    if (nRed > nBlue)
        printf("WINNER: RED\n");
    else if (nBlue > nRed)
        printf("WINNER: BLUE\n");
    else
        print("RESULT: DRAW\n");
}
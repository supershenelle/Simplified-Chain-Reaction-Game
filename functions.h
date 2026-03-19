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

    g->good = 0;
    g->go = 0; 
    g->start = 1;
    g->over = 0;
    g->found = 0;
    g->val = 0;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            g->R[i][j] = 0;
            g->B[i][j] = 0;
            g->S[i][j] = 0;
            g->T[i][j] = 0;
        }
    }
}

/* Display */
void displayBoard(GameState g);

/* Input */
void getMove(int *row, int *col);
int isValidPos(int row, int col);

/* Game Actions */
void removePos(GameState *g, int row, int col);
void replacePos(GameState *g, int row, int col);
void expandPos(GameState *g, int row, int col);
void updatePos(GameState *g, int row, int col);
void nextPlayerMove(GameState *g, int row, int col);

/* Utility Functions */
int countPieces(int board[4][4]);
int countFreeCells(GameState g);

/* Game Status */
void checkGameOver(GameState *g);
void showResult(GameState g);
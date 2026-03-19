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
void initializeGame(GameState *g);

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
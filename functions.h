#include <stdio.h>

/* ============================= */
/* STRUCTURES             */
/* ============================= */

typedef struct
{
    int good;
    int go;
    int start;
    int over;
    int found;
    int val;

    // 4x4 buffer para index 1-3 will be allowed
    int R[4][4];
    int B[4][4];
    int S[4][4];
    int T[4][4];

} GameState;

// function prototypes
void displayDivider();
void initializeGame(GameState *g); 
void displayBoard(GameState g);

void getMove(GameState *g, int *row, int *col); 
int isValidPos(GameState g, int row, int col); 

void removePos(GameState *g, int row, int col); 
void replacePos(GameState *g, int row, int col); 
void expandPos(GameState *g, int row, int col); 
void updatePos(GameState *g, int row, int col); 
void nextPlayerMove(GameState *g, int row, int col);

int countPieces(int board[4][4]);
int countFreeCells(GameState g);

void checkGameOver(GameState *g);
void showResult(GameState g);

// function implementations
void displayDivider() 
{
    printf("===========================================\n");
}

/* Initialization */
void initializeGame(GameState *g)
{
    int i, j;

    g->good = 0; // 0 if invalid move, 1 if valid move
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
    int i, j;

    // define the colors
    const char *RED = "\033[1;31m";  
    const char *BLUE = "\033[1;34m";
    const char *BOARD = "\033[0;37m";  
    const char *RESET = "\033[0m";

    printf("\n%s    1   2   3%s\n", BOARD, RESET);
    printf("%s  +---+---+---+%s\n", BOARD, RESET);

    for(i = 1; i <= 3; i++) 
    {
        printf("%s%d |%s", BOARD, i, RESET);

        for(j = 1; j <= 3; j++) 
        {
            if(g.R[i][j] == 1)
                printf(" %sR%s %s|%s", RED, RESET, BOARD, RESET);
            else if (g.B[i][j] == 1)
                printf(" %sB%s %s|%s", BLUE, RESET, BOARD, RESET);
            else
                printf("   %s|%s", BOARD, RESET); 
        }
        printf("\n%s  +---+---+---+%s\n", BOARD, RESET);
    }

    if (g.go == 0) 
        printf("Current Turn: %sRED%s\n", RED, RESET);
    else 
        printf("Current Turn: %sBLUE%s\n", BLUE, RESET);

    printf("Current Val: %d\n", g.val);
    displayDivider();
}

/* Player Input */
void getMove(GameState *g, int *row, int *col)
{
    int r, c;
    int valid = 0;

    while (!valid)
    {
        printf("--> ENTER YOUR MOVE (ROW AND COLUMN):\n");
        printf("ENTER ROW: ");
        
        if (scanf("%d", &r) == 1)
        {
            printf("ENTER COLUMN: ");
            if (scanf("%d", &c) == 1)
            {
                if (!isValidPos(*g, r, c))
                {
                    printf("\n--> USER HAS INPUTTED AN INVALID POSITION.\n");
                    printf("--> INPUT MUST BE BETWEEN 1-3 ONLY. TRY AGAIN.\n");
                    displayDivider();
                }
                else if (g->start == 1 && g->S[r][c] == 1)
                {
                    printf("\n--> POSITION IS OCCUPIED. TRY AGAIN.\n");
                    displayDivider();
                }
                else if (g->start == 0 && !((g->go == 0 && g->R[r][c] == 1) || (g->go == 1 && g->B[r][c] == 1)))
                {
                    printf("\n--> CHOOSE YOUR OWN PIECE TO EXPAND. TRY AGAIN.\n");
                    displayDivider();
                }
                else
                {
                    *row = r;
                    *col = c;
                    valid = 1;
                }
            }
            else
            {
                printf("\n--> INVALID INPUT FORMAT FOR COLUMN.\n");
                printf("--> INPUT MUST BE AN INTEGER BETWEEN 1-3. TRY AGAIN.\n");
                displayDivider();
                while (getchar() != '\n');
            }
        }
        else
        {
            printf("\n--> INVALID INPUT FORMAT FOR ROW.\n");
            printf("--> INPUT MUST BE AN INTEGER (1-3). TRY AGAIN.\n");
            displayDivider();
            while (getchar() != '\n');
        }
    }
}

/* Validation */
int isValidPos(GameState g, int row, int col)
{
    return (row >= 1 && row <= 3 && col >= 1 && col <= 3);
}

/* Game Actions */
void removePos(GameState *g, int row, int col)
{
    // FIX: Simplified to clear the cell entirely during an explosion
    g->R[row][col] = 0;
    g->B[row][col] = 0;
    g->S[row][col] = 0;
}

/* Replace Position */
void replacePos(GameState *g, int row, int col)
{
    if (row < 1 || row > 3 || col < 1 || col > 3) return;

    int wasOccupied = g->S[row][col];

    if (g->go == 0) { // Red's turn/expansion
        g->B[row][col] = 0; 
        g->R[row][col] = 1;
    } 
    else { // Blue's turn/expansion
        g->R[row][col] = 0; 
        g->B[row][col] = 1;
    }

    g->S[row][col] = 1;

    // Trigger expansion if cell was occupied and hasn't exploded yet this turn
    if (wasOccupied && g->T[row][col] == 0) 
    {
        expandPos(g, row, col);
    }
}

void expandPos(GameState *g, int row, int col) 
{
    // FIX: Set T flag IMMEDIATELY to prevent infinite recursion loop
    g->T[row][col] = 1; 
    removePos(g, row, col);

    replacePos(g, row - 1, col); // Up
    replacePos(g, row + 1, col); // Down
    replacePos(g, row, col - 1); // Left
    replacePos(g, row, col + 1); // Right
}

/* Update Position */
void updatePos(GameState *g, int row, int col) 
{
    if (g->S[row][col] == 1 && g->T[row][col] == 0) 
    {
        expandPos(g, row, col);
        g->good = 1;
    }
}

/* Next Player Move */
void nextPlayerMove(GameState *g, int row, int col) 
{
    int i, j;
    if (g->over == 1) return;
    
    if (g->start == 1) 
    {
        if (g->S[row][col] == 0) 
        {
            if (g->go == 0) g->R[row][col] = 1; 
            else            g->B[row][col] = 1;
            
            g->S[row][col] = 1; 
            g->good = 1;
        } 
        else 
        {
            printf("\033[1;33mPosition already taken! Choose an empty cell.\033[0m\n");
            g->good = 0;
        }
    } 
    else 
    {
        if ((g->go == 0 && g->R[row][col] == 1) || (g->go == 1 && g->B[row][col] == 1)) 
        {
            updatePos(g, row, col); 
            g->good = 1;
        } 
        else 
        {
            printf("\033[1;31mInvalid Move: You must select your own piece!\033[0m\n");
            g->good = 0;
        }
    }

    if (g->good == 1) 
    {
        g->go = (g->go == 0) ? 1 : 0;
        g->val = g->val + 1;

        for (i = 0; i < 4; i++) 
            for (j = 0; j < 4; j++) 
                g->T[i][j] = 0; 
        
        g->good = 0;
    }

    // FIX: End start phase only after both players have placed 3 pieces
    if (g->start == 1 && countPieces(g->R) >= 3 && countPieces(g->B) >= 3)
    {
        g->start = 0;
    }
}

/* Count Pieces */
int countPieces(int board[4][4]) 
{
    int i, j, count = 0;
    for(i = 1; i <= 3; i++)
        for(j = 1; j <= 3; j++)
            if(board[i][j])
                count++;
    return count;
}

/* Count Vacant Cells*/
int countFreeCells(GameState g) 
{
    int i, j, occupied = 0;
    for(i = 1; i <= 3; i++)
        for(j = 1; j <= 3; j++)
            if(g.S[i][j] == 1) 
                occupied++;
    return 9 - occupied;
}

/* Game Status */
void checkGameOver(GameState *g) 
{
    int rCount = countPieces(g->R);
    int bCount = countPieces(g->B);
    int free = countFreeCells(*g);

    // FIX: Game ends if board is full, turn limit (50) reached, or player wiped out
    if (free == 0 || g->val >= 50 || (!g->start && (rCount == 0 || bCount == 0)))
        g->over = 1;
}

/* Show Result */
void showResult(GameState g)
{
    int nRed = countPieces(g.R);
    int nBlue = countPieces(g.B);

    printf("| -------------- GAME OVER -------------- |\n");
    printf("%sRed%s pieces: %d\n", "\033[31m", "\033[0m", nRed);
    printf("%sBlue%s pieces: %d\n", "\033[34m", "\033[0m", nBlue);

    if (nRed > nBlue)
        printf("--> WINNER: %sRED%s\n", "\033[31m", "\033[0m");
    else if (nBlue > nRed)
        printf("--> WINNER: %sBLUE%s\n", "\033[34m", "\033[0m");
    else
        printf("--> RESULT: %sDRAW%s\n", "\033[33m", "\033[0m");
}

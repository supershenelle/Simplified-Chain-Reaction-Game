#include <stdio.h>

/* ============================= */
/* STRUCTURES             */
/* ============================= */

typedef struct
{
    int good;   /* boolean: true/false */
    int go;     /* boolean: true/false (Red/Blue) */
    int start;  /* boolean: true/false */
    int over;   /* boolean: true/false */
    int found;  /* boolean: true/false */
    int val;    /* positive integer */

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

    g->good = 0;   
    g->go = 1;    
    g->start = 1;  
    g->over = 0;   
    g->found = 0;  
    g->val = 1;   

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

void displayBoard(GameState g)
{
    int i, j;
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

    if (g.go == 1) /* go is true */
        printf("Current Turn: %sRED%s\n", RED, RESET);
    else 
        printf("Current Turn: %sBLUE%s\n", BLUE, RESET);

    printf("Current Val: %d\n", g.val);
    displayDivider();
}

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
                else if (g->start == 0 && !((g->go == 1 && g->R[r][c] == 1) || (g->go == 0 && g->B[r][c] == 1)))
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
            else { while (getchar() != '\n'); }
        }
        else { while (getchar() != '\n'); }
    }
}

int isValidPos(GameState g, int row, int col) /* C: {x in Z+ | x < 4} */
{
    return (row >= 1 && row <= 3 && col >= 1 && col <= 3);
}

void removePos(GameState *g, int row, int col) 
{
    if (g->go == 1) g->R[row][col] = 0; /* go -> R = R - {pos} */
    else            g->B[row][col] = 0; /* ~go -> B = B - {pos} */
    
    g->S[row][col] = 0; /* S = S - {pos} [cite: 31] */
}

void replacePos(GameState *g, int row, int col) 
{
    if (!isValidPos(*g, row, col)) return;

    g->found = 0; /* found = false [cite: 37] */

    /* Logic for capturing/finding pieces */
    if (g->go == 1) {
        if (g->B[row][col] == 1) { g->B[row][col] = 0; g->found = 1; }
        else if (g->R[row][col] == 1) { g->found = 1; }
        if (g->R[row][col] == 0) g->R[row][col] = 1;
    } else {
        if (g->R[row][col] == 1) { g->R[row][col] = 0; g->found = 1; }
        else if (g->B[row][col] == 1) { g->found = 1; }
        if (g->B[row][col] == 0) g->B[row][col] = 1;
    }

    /* Expansion Trigger */
    if (g->found == 1 && g->S[row][col] == 0) {
        g->S[row][col] = 1;
        g->found = 0;
    } 
    else if (g->found == 1 && g->S[row][col] == 1 && g->T[row][col] == 0) {
        g->T[row][col] = 1; /* T = T U {pos} */
        expandPos(g, row, col);
    }
}

void expandPos(GameState *g, int row, int col) 
{
    removePos(g, row, col); /* Remove(pos) [cite: 57] */
    replacePos(g, row - 1, col); /* Replace(u) */
    replacePos(g, row + 1, col); /* Replace(d) */
    replacePos(g, row, col - 1); /* Replace(k) */
    replacePos(g, row, col + 1); /* Replace(r) */
}

void updatePos(GameState *g, int row, int col)
{
    g->good = 0; /* good = false */
    if (g->S[row][col] == 0) {
        g->S[row][col] = 1;
        g->good = 1; /* good = ~good */
    } 
    else if (g->good == 0 && g->S[row][col] == 1 && g->T[row][col] == 0) {
        g->T[row][col] = 1;
        expandPos(g, row, col);
        g->good = 1; /* Resulting in good = true after expansion */
    }
}

void nextPlayerMove(GameState *g, int row, int col) 
{
    int i, j;
    if (g->over == 1) return;
    
    if (g->start == 1) {
        if (g->go == 1) g->R[row][col] = 1; /* go -> R = R U {pos} */
        else            g->B[row][col] = 1; /* ~go -> B = B U {pos} */
        g->S[row][col] = 1;
        g->good = 1;
    } 
    else {
        /* [cite: 69] If it's your turn and you pick your piece */
        if ((g->go == 1 && g->R[row][col] == 1) || (g->go == 0 && g->B[row][col] == 1)) {
            updatePos(g, row, col);
            g->good = 1;
        }
    }

    /* Transition to normal gameplay */
    if (g->start == 1 && countPieces(g->R) >= 1 && countPieces(g->B) >= 1) {
        g->start = 0;
    }

    /* Successful move completion */
    if (g->good == 1) {
        g->go = (g->go == 1) ? 0 : 1; /* go = ~go */
        g->val = g->val + 1;         /* val = val + 1 */
        for (i = 0; i < 4; i++) 
            for (j = 0; j < 4; j++) 
                g->T[i][j] = 0; /* Reset T set after turn */
        g->good = 0;
    }
}

int countPieces(int board[4][4]) 
{
    int i, j, count = 0;
    for(i = 1; i <= 3; i++)
        for(j = 1; j <= 3; j++)
            if(board[i][j]) count++;
    return count;
}

int countFreeCells(GameState g) 
{
    int i, j, occupied = 0;
    for(i = 1; i <= 3; i++)
        for(j = 1; j <= 3; j++)
            if(g.S[i][j] == 1) occupied++;
    return 9 - occupied;
}

void checkGameOver(GameState *g) 
{
    int rCount = countPieces(g->R);
    int bCount = countPieces(g->B);
    int free = countFreeCells(*g);

    /* (F = 3 OR val >= 20 OR (~start AND wipeout)) */
    if (free == 3 || g->val >= 20 || (!g->start && (rCount == 0 || bCount == 0)))
        g->over = 1;
}

void showResult(GameState g) 
{
    int nRed = countPieces(g.R);
    int nBlue = countPieces(g.B);
    printf("| -------------- GAME OVER -------------- |\n");
    printf("%sRed%s pieces: %d\n", "\033[31m", "\033[0m", nRed);
    printf("%sBlue%s pieces: %d\n", "\033[34m", "\033[0m", nBlue);

    if (nRed > nBlue)      printf("--> WINNER: %sRED%s\n", "\033[31m", "\033[0m");
    else if (nBlue > nRed) printf("--> WINNER: %sBLUE%s\n", "\033[34m", "\033[0m");
    else                   printf("--> RESULT: %sDRAW%s\n", "\033[33m", "\033[0m");
}

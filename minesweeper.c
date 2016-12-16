/************************************************************************
 * minesweeper.c														*
 *																		*
 * Author(s): Caitlin Crowe and Emily Peterson							*
 ***********************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BOARD_SIZE_MIN 5
#define BOARD_SIZE_MAX 15
#define PCT_MINES_MIN 10
#define PCT_MINES_MAX 70

typedef struct {
    bool is_mine;
    int mines;
    bool visible;
} Cell;

typedef enum {WON, LOST, INPROGRESS} Status;

/************************************************************************
 * YOU MUST NOT DEFINE ANY GLOBAL VARIABLES (i.e., OUTSIDE FUNCTIONS).  *
 * COMMUNICATION BETWEEN FUNCTIONS MUST HAPPEN ONLY VIA PARAMETERS.     *
 ************************************************************************/

/************************************************************************
 * Function declarations/prototypes										*
 ************************************************************************/
void displayMenu();

int getBoardSize();

int getPercentMines();

void initBoard(int size, Cell board[][size]);

void placeMinesOnBoard(int size, Cell board[][size], int nbrMines);

void fillInMineCountForNonMineCells(int size, Cell board[][size]);

int nbrOfMines(int size, Cell board[][size]);

int getNbrNeighborMines(int row, int col, int size, Cell board[][size]);

void displayBoard(int size, Cell board[][size], bool displayMines);

Status selectCell(int row, int col, int size, Cell board[][size]);

int nbrVisibleCells(int size, Cell board[][size]);

void setImmediateNeighborCellsVisible(int row, int col, int size, Cell board[][size]);

void setAllNeighborCellsVisible(int row, int col, int size, Cell board[][size]);


/************************************************************************
 * Main driver of the program. Uses the functions defined above.		*
 ************************************************************************/
int main()
{
    int row, col, size, nbrMines;
    char command;
    bool displayMines = false;
    Status gameState = INPROGRESS;
    
    printf("!!!!!WELCOME TO THE MINESWEEPER GAME!!!!!\n\n");
    size = getBoardSize();
    
    // declare 2D array of cells
    Cell board[size][size];
    
    initBoard(size,board);
    
    // determine number of mine to place on the board
    nbrMines = (int) (size * size * (getPercentMines()/100.0));
    
    // place mines randomly on the board
    placeMinesOnBoard(size,board,nbrMines);
    
    // For each non-mine cell, sets the neighboring mine count
    fillInMineCountForNonMineCells(size,board);
    
    displayBoard(size,board,displayMines);
    
    getchar();
    while (true) {
        printf("Enter command (m/M for command menu): ");
        scanf("%c",&command);
        
        switch (command) {
            case 'm': case 'M':
                displayMenu();
                break;
                
            case 'c': case 'C':
                do {
                    printf("Enter row and column of cell: ");
                    scanf("%d%d",&row,&col);
                    getchar();
                    if (row < 1 || row > size || col < 1 || col > size) {
                        printf("Invalid row or column values. Try again.\n");
                    }
                } while (row < 1 || row > size || col < 1 || col > size);
                row--;
                col--;
                gameState = selectCell(row,col,size,board);
                displayBoard(size,board,displayMines);
                break;
                
            case 's': case 'S':
                displayMines = true;
                displayBoard(size,board,displayMines);
                break;
                
            case 'h': case 'H':
                displayMines = false;
                displayBoard(size,board,displayMines);
                break;
                
            case 'b': case 'B':
                displayBoard(size,board,displayMines);
                break;
                
            case 'q': case 'Q':
                printf("Bye.\n");
                return 0;
                
            default:
                printf("Invalid command. Try again.\n");
        }
        
        // display appropriate message if the game is over
        if (gameState == WON) {
            printf("You found all the mines. Congratulations. Bye.\n");
            return 0;
        } else if (gameState == LOST) {
            printf("Oops. Sorry, you landed on a mine. Bye\n");
            return 0;
        }
    }
    
    return 0;
}


/************************************************************************
 * Displays list of commands allowed at the prompt						*
 ************************************************************************/
void displayMenu()
{
    printf("List of available commands:\n");
    printf("   Show Mines: s/S\n");
    printf("   Hide Mines: h/H\n");
    printf("   Select Cell: c/C\n");
    printf("   Display Board: b/B\n");
    printf("   Display Menu: m/M\n");
    printf("   Quit: q/Q\n\n");
}

/************************************************************************
 * Initializes the fields of each cell on the board as follows:			*
 * 		is_mine field to false											*
 * 		mines field to 0												*
 * 		visible field to false											*
 ************************************************************************/
void initBoard(int size, Cell board[][size])
{
    int r, c;
    for(r=0; r<size; r++){
        for(c=0; c<size; c++){
            board[r][c].is_mine = 0; //false
            board[r][c].mines = 0;
            board[r][c].visible = 0; //false
        }
    }
}

/************************************************************************
 * Places the specified number of mines randomly on the board			*
 ************************************************************************/
void placeMinesOnBoard(int size, Cell board[][size], int nbrMines)
{
    int i;
    srand(time(NULL));
    for(i=0; i<nbrMines; i++){
        //range is from 1 to size
        int r = (rand() % size);
        int c = (rand() % size);
        if(board[r][c].is_mine == 0){
            board[r][c].is_mine = 1; //true-mine is in that cell now
        }
        else if(board[r][c].is_mine == 1){
            i--;
        }
    }
}

/************************************************************************
 * For each non-mine cell on the board, set the "mines" field to the	*
 * number of mines in the immediate neighborhood.						*
 ************************************************************************/
void fillInMineCountForNonMineCells(int size, Cell board[][size])
{
    int r, c;
    for(r=0; r<size; r++){
        for(c=0; c<size; c++){
            int count = 0;
            count = getNbrNeighborMines(r, c, size, board);
            board[r][c].mines = count;
        }
    }
}

/************************************************************************
 * Counts and returns the number of mines on the board					*
 ************************************************************************/
int nbrOfMines(int size, Cell board[][size])
{
    int count = 0;
    int r, c;
    for(r=0; r<size; r++){
        for(c=0; c<size; c++){
            if (board[r][c].is_mine == 1){ //if there is a mine, then add 1 to count
                count++;
            }
        }
    }
    
    return count;
}

/************************************************************************
 * Returns the number of mines in the immediate neighborhood of a cell	*
 * at location (row,col) on the board.									*
 ************************************************************************/
int getNbrNeighborMines(int row, int col, int size, Cell board[][size])
{
    int count = 0;
    int r = row;
    int c = col;
    
    if(board[r][c].is_mine == 0){ //if there is no mine in the current cell
        if(r==0 && c==0){ //top left corner
            //check bottom, right, and bottom-right corner cell
            if(board[1][0].is_mine == 1){ //bottom
                count++;
            }
            if(board[0][1].is_mine == 1){ //right
                count++;
            }
            if(board[1][1].is_mine == 1){ //bottom-right corner
                count++;
            }
        }
        else if(r==0 &&c ==(size-1)){ //top right corner
            //check bottom, left, bottom-left corner cell
            if(board[1][(size-1)].is_mine == 1){ //bottom
                count++;
            }
            if(board[0][(size-2)].is_mine == 1){ //left
                count++;
            }
            if(board[1][(size-2)].is_mine == 1){ //bottom-left corner
                count++;
            }
        }
        else if(r==(size-1) && c==0){ //bottom left corner
            //check above, right, top-right corner
            if(board[(size-2)][0].is_mine == 1){ //above
                count++;
            }
            if(board[(size-1)][1].is_mine == 1){ //right
                count++;
            }
            if(board[(size-2)][1].is_mine == 1){ //above-right corner
                count++;
            }
        }
        else if(r==(size-1) && c==(size-1)){ //bottom right corner
            //check above, left, top-left corner
            if(board[(size-2)][(size-1)].is_mine == 1){ //above
                count++;
            }
            if(board[(size-1)][(size-2)].is_mine == 1){ //right
                count++;
            }
            if(board[(size-2)][(size-2)].is_mine == 1){ //above-right corner
                count++;
            }
        }
        else if(r>0 && r<(size-1) && c==0){ //left wall
            //check above, right, below, top-right, bottom-right
            if(board[r-1][0].is_mine == 1){ //above
                count++;
            }
            if(board[r][1].is_mine == 1){ //right
                count++;
            }
            if(board[r+1][0].is_mine == 1){ //below
                count++;
            }
            if(board[r-1][1].is_mine == 1){ //top-right
                count++;
            }
            if(board[r+1][1].is_mine == 1){ //bottom-right
                count++;
            }
        }
        else if(r>0 && r<(size-1) && c==(size-1)){ //right wall
            //check above, left, below, top-left, bottom-left
            if(board[r-1][(size-1)].is_mine == 1){ //above
                count++;
            }
            if(board[r][(size-2)].is_mine == 1){ //left
                count++;
            }
            if(board[r+1][(size-1)].is_mine == 1){ //below
                count++;
            }
            if(board[r-1][(size-2)].is_mine == 1){ //top-left
                count++;
            }
            if(board[r+1][(size-2)].is_mine == 1){ //bottom-left
                count++;
            }
        }
        else if(r==0 && c>0 && c<(size-1)){ //top wall
            //check left, right, below, bottom-left, bottom-right
            if(board[0][c-1].is_mine == 1){ //left
                count++;
            }
            if(board[0][c+1].is_mine == 1){ //right
                count++;
            }
            if(board[1][c].is_mine == 1){ //below
                count++;
            }
            if(board[1][c-1].is_mine == 1){ //bottom-left
                count++;
            }
            if(board[1][c+1].is_mine == 1){ //bottom-right
                count++;
            }
        }
        else if(r==(size-1) && c>0 && c<(size-1)){ //bottom wall
            //check above, left, right, top-left, top-right
            if(board[(size-2)][c].is_mine == 1){ //above
                count++;
            }
            if(board[(size-1)][c-1].is_mine == 1){ //left
                count++;
            }
            if(board[(size-1)][c+1].is_mine == 1){ //right
                count++;
            }
            if(board[(size-2)][c-1].is_mine == 1){ //top-left
                count++;
            }
            if(board[(size-2)][c+1].is_mine == 1){ //top-right
                count++;
            }
        }
        else{ //in the middle
            //check all around: above, left, right, bottom, top-left, top-right, bottom-left, bottom-right
            if(board[r-1][c].is_mine == 1){ //above
                count++;
            }
            if(board[r][c-1].is_mine == 1){ //left
                count++;
            }
            if(board[r][c+1].is_mine == 1){ //right
                count++;
            }
            if(board[r+1][c].is_mine == 1){ //below
                count++;
            }
            if(board[r-1][c-1].is_mine == 1){ //top-left
                count++;
            }
            if(board[r-1][c+1].is_mine == 1){ //top-right
                count++;
            }
            if(board[r+1][c-1].is_mine == 1){ //bottom-left
                count++;
            }
            if(board[r+1][c+1].is_mine == 1){ //bottom-right
                count++;
            }
        }
    }
    return count;
}

/************************************************************************
 * Displays the board. If a cell is not currently visible and has a		*
 * mine, show the mine if the displayMines is true. Used for debugging	*
 * and testing purposes.												*
 ************************************************************************/
void displayBoard(int size, Cell board[][size], bool displayMines)
{
    int r, c;
    
    //Printing Column nums on top
    printf("\n\n   ");
    for(c=1; c<=size; c++){
        printf("%3d", c);
    }
    printf("\n");
    for(r=0; r<size; r++){
        printf("%3d", (r+1));
        for(c=0; c<size; c++){
            if(displayMines == 1){
                if(board[r][c].is_mine == 1){
                    printf("  *");
                }
                else if(board[r][c].visible == 1){
                    printf("%3d", board[r][c].mines);
                }
                else if(board[r][c].visible == 0){
                    printf("  ?");
                }
            }
            else if(displayMines == 0){
                if(board[r][c].is_mine == 1){
                    printf("  ?");
                }
                else if(board[r][c].visible == 1){
                    printf("%3d", board[r][c].mines);
                }
                else if(board[r][c].visible == 0){
                    printf("  ?");
                }
            }
            
        }
        printf("\n");
    }
    printf("\n\n");
}

/************************************************************************
 * Prompts the user for board size, reads and validates the input		*
 * entered, and returns the integer if it is within valid range.		*
 * repeats this in a loop until the user enters a valid value.			*
 ************************************************************************/
int getBoardSize()
{
    int size = 0;
    while(size<5 || size>15){
        size = 0;
        printf("Enter the board size (5 .. 15): ");
        scanf("%d",&size);
    }
    
    return size;
}

/************************************************************************
 * Prompts the user for percentage of mines to place on the board,		*
 * reads and validates the input entered, and returns the integer if it	*
 * is within valid range. repeats this in a loop until the user enters	*
 * a valid value for board size.										*
 ************************************************************************/
int getPercentMines()
{
    int percent = 0;
    while(percent<10 || percent>70){
        percent = 0;
        printf("Enter the percentage of mines on the board (10 .. 70): ");
        scanf("%d",&percent);
    }
    
    return percent;
}

/************************************************************************
 * Process cell selection by user during the game						*
 ************************************************************************/
Status selectCell(int row, int col, int size, Cell board[][size])
{
    //board[row][col];
    setAllNeighborCellsVisible(row, col, size, board);
    int i = row*col-nbrOfMines(size, board);
    if(board[row][col].is_mine){
        return LOST;
    }
    else if(nbrVisibleCells(size,board) == i){
        return WON;
    }
    else{
        return INPROGRESS;
    }
}

/************************************************************************
 * Returns the number of cells that are currently visible.				*
 ************************************************************************/
int nbrVisibleCells(int size, Cell board[][size])
{
    int count = 0;
    int r, c;
    
    for(r=0; r<size; r++){
        for(c=0; c<size; c++){
            if(board[r][c].visible == 1){
                count++;
            }
        }
    }
    return count;
}

/************************************************************************
 * If the mine count of a cell at location (row,col) is zero, then make	*
 * the cells ONLY in the immediate neighborhood visible.				*
 ************************************************************************/
void setImmediateNeighborCellsVisible(int row, int col, int size, Cell board[][size])
{
    int r = row;
    int c = col;
    if(board[row][col].mines == 0){
        //Col edges
        if(col==0 || col==size ){
            if(col == 0){
                board[r][c+1].visible = 1;
            }
            if(col == size){
                board[r][c-1].visible = 1;
            }
            if(row<size){
                board[r+1][c].visible = 1;
            }
            if(row!=0){
                board[r-1][c].visible = 1;
            }
        
        //Row edges
        }else if(row == 0 || row == size){
            board[r][c+1].visible = 1;
            board[r][c-1].visible = 1;
            if(row == 0){
                board[r+1][c].visible = 1;
            }
            if(row == size){
                board[r-1][c].visible = 1;
            }
        
        //Middle
        }else{
            board[r][c+1].visible = 1;
            board[r][c-1].visible = 1;
            board[r+1][c].visible = 1;
            board[r-1][c].visible = 1;
        }
    }
}

/************************************************************************
 * If the mine count of a cell at location (row,col) is zero, then make	*
 * the cells in the immediate neighborhood visible and repeat this		*
 * process for each of the cells in this set of cells that have a mine	*
 * count of zero, and so on.											*
 ************************************************************************/
void setAllNeighborCellsVisible(int row, int col, int size, Cell board[][size])
{
    int r = row;
    int c = col;
    
    if(board[row][col].mines == 0){
        
        //Col edges
        if(col==0 || col==size ){
            if(col==0){
                setImmediateNeighborCellsVisible(r, c+1, size, board);
            }
            if(col==size){
                setImmediateNeighborCellsVisible(r, c-1, size, board);
            }
            if(row<size){
                setImmediateNeighborCellsVisible(r+1, c, size, board);
            }
            if(row!=0){
                setImmediateNeighborCellsVisible(r-1, c, size, board);
            }
            
        //Row edges
        }else if(row==0 || row==size){
            setImmediateNeighborCellsVisible(r, c+1, size, board);
            setImmediateNeighborCellsVisible(r, c-1, size, board);
            if(row==0){
                setImmediateNeighborCellsVisible(r+1, c, size, board);
            }
            if(row==size){
                setImmediateNeighborCellsVisible(r-1, c, size, board);
            }
            
        //Middle
        }else{
            setImmediateNeighborCellsVisible(r, c+1, size, board);
            setImmediateNeighborCellsVisible(r, c-1, size, board);
            setImmediateNeighborCellsVisible(r+1, c, size, board);
            setImmediateNeighborCellsVisible(r-1, c, size, board);

        }
    }
    
}


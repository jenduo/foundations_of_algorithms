/* Program to print and play checker games.

  Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
  September 2021, with the intention that it be modified by students
  to add functionality, as required by the assignment specification.

  Student Authorship Declaration:

  (1) I certify that except for the code provided in the initial skeleton file,
  the program contained in this submission is completely my own individual
  work, except where explicitly noted by further comments that provide details
  otherwise. I understand that work that has been developed by another student,
  or by me in collaboration with other students, or by non-students as a result
  of request, solicitation, or payment, may not be submitted for assessment in
  this subject. I understand that submitting for assessment work developed by
  or in collaboration with other students or non-students constitutes Academic
  Misconduct, and may be penalized by mark deductions, or by other penalties
  determined via the University of Melbourne Academic Honesty Policy, as
  described at https://academicintegrity.unimelb.edu.au.

  (2) I also certify that I have not provided a copy of this work in either
  softcopy or hardcopy or any other form to any other student, and nor will I
  do so until after the marks are released. I understand that providing my work
  to other students, regardless of my intention or any undertakings made to me
  by that other student, is also Academic Misconduct.

  (3) I further understand that providing a copy of the assignment specification
  to any form of code authoring or assignment tutoring service, or drawing the
  attention of others to such services and code that may have been made
  available via such a service, may be regarded as Student General Misconduct
  (interfering with the teaching activities of the University and/or inciting
  others to commit Academic Misconduct). I understand that an allegation of
  Student General Misconduct may arise regardless of whether or not I personally
  make use of such solutions or sought benefit from such actions.

  Signed by: QUANG CAT TUONG DUONG 12566256
  Dated:     10/10/2021

*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <assert.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE          8       // board size
#define ROWS_WITH_PIECES    3       // number of initial rows with pieces
#define CELL_EMPTY          '.'     // empty cell character
#define CELL_BPIECE         'b'     // black piece character
#define CELL_WPIECE         'w'     // white piece character
#define CELL_BTOWER         'B'     // black tower character
#define CELL_WTOWER         'W'     // white tower character
#define COST_PIECE          1       // one piece cost
#define COST_TOWER          3       // one tower cost
#define TREE_DEPTH          3       // minimax tree depth
#define COMP_ACTIONS        10      // number of computed actions

/* one type definition from my sample solution -------------------------------*/

#define FALSE -999
#define TRUE 999
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE];  // board type
typedef int arr[BOARD_SIZE][BOARD_SIZE];
typedef int arr2[BOARD_SIZE][BOARD_SIZE];

//First we should set some functions that helps us with dealing with values
// since each piece has a value, we can make a board of integers and convert when printing it

// this function converts values of pieces into its characters
char conv_piece(int x){
    if (x == 0) return CELL_EMPTY;
    if (x == 1) return CELL_BPIECE;
    if (x == 3) return CELL_BTOWER;
    if (x == -1) return CELL_WPIECE;
    if (x == -3) return CELL_WTOWER;
}

int conv_col(char x){
    if (x == 'A') return 0;
    if (x == 'B') return 1;
    if (x == 'C') return 2;
    if (x == 'D') return 3;
    if (x == 'E') return 4;
    if (x == 'F') return 5;
    if (x == 'G') return 6;
    if (x == 'H') return 7;
}


// function that tells us whose turn it is
int black_turn(int turn){
    if (turn%2==0) return FALSE;
    return TRUE;
}


// function that given coordinates, board, turn number, tells us if the move is legal
int check_valid(int fr_row, int fr_col, int to_row, int to_col, arr board, int turn) {
    // errors 1 and 2 are if the coordinates are outside the board

    if (fr_row < 0 || fr_row >= BOARD_SIZE || fr_col < 0 || fr_col >= BOARD_SIZE) return 1;
    if (to_row < 0 || to_row >= BOARD_SIZE || to_col < 0 || to_col >= BOARD_SIZE) return 2;

    // error 3 is if the piece moving is empty
    // error 4 is if the landing coordinate is occupied
    int fr_p = board[fr_row][fr_col];
    int to_p = board[to_row][to_col];
    if (conv_piece(fr_p) == CELL_EMPTY) return 3;
    if (conv_piece(to_p) != CELL_EMPTY) return 4;

    // error 5 is if the target piece is valid for the turn
    // Black's turn: moving piece's value has to be more then 0
    // White's turn: moving piece's value has to be less then 0
    if (black_turn(turn)==TRUE && fr_p < 0) return 5;
    if (black_turn(turn)==FALSE && fr_p > 0) return 5;

    // error 6 is all illegal moves
    int r_mov = to_row - fr_row;
    int c_mov = to_col - fr_col;

    // the amount of rows and colums moved have to me the same but cant be more then 2
    if (abs(r_mov) != abs(c_mov)) return 6;
    if (abs(r_mov)>2) return 6;

    // WPIECE : row movement can only be positive, down the board
    // BPIECE : col movement can only be negative, up the board
    if (conv_piece(fr_p) == CELL_WPIECE && r_mov<1) return 6;
    if (conv_piece(fr_p) == CELL_BPIECE && r_mov>-1) return 6;

    // if the movement is 2, then it is going to eat a piece
    if (abs(r_mov) == 2){
        int mid_p = board[(fr_row + to_row) / 2][(fr_col + to_col) / 2];
        // eaten piece can not be empty
        if (mid_p == 0) return 6;
        // the eat piece can not be the same sign as the moving piece
        if (fr_p < 0 && mid_p < 0) return 6;
        if (fr_p > 0 && mid_p > 0) return 6;

    }


    return TRUE;

}

// function the given move, board, does the move and returns the board
void move(int fr_row, int fr_col, int to_row, int to_col, arr board)
{
    int fr_p = board[fr_row][fr_col];
    // BPIECE: if it is moving to the first row it gets promoted to a tower
    // WPIECE: if it is moving to the final row it gets promoted to a tower
    if (to_row == 0 && conv_piece(fr_p) == CELL_BPIECE) fr_p = 3;
    if (to_row == 7 && conv_piece(fr_p) == CELL_WPIECE) fr_p = -3;

    // the target cell becomes the source cell
    // the source cell's location is not empty
    board[to_row][to_col] = fr_p;
    board[fr_row][fr_col] = 0;

    // if the piece is eating another, piece it is eating is empty
    if (abs(fr_row - to_row) == 2) board[(fr_row + to_row) / 2][(fr_col + to_col) / 2] = 0;
}

//calculates the board cost
int board_cost(arr board)
{
    int sum = 0;
    int *p = board;
    for (int i=0; i< (BOARD_SIZE*BOARD_SIZE); i++){
        sum = *p+ sum;
        *p = *(p+i);
    }
    return sum;
}


// draws the board, given the board's positions in values
void draw_board(arr board)
{
    printf("     A   B   C   D   E   F   G   H\n");
	for (int row = 0; row < BOARD_SIZE; ++row)
	{
		printf("   +---+---+---+---+---+---+---+---+\n");
		printf(" %d |", row + 1);
		for (int col = 0; col < BOARD_SIZE; ++col)	{
                printf(" %c |", conv_piece(board[row][col]));
		}
		printf("\n");
	}
	printf("   +---+---+---+---+---+---+---+---+\n");

}




// this is a function that creates a copy of a table, will be helpful for stage 1
void
copy_table(arr board, arr2 copy){
    for (int i = 0; i < BOARD_SIZE; ++i){
        for (int j = 0; j < BOARD_SIZE; ++j){
                copy[i][j]= board[i][j];
        }
    }
}



// this is the ai move

struct best {
    int fr_row;
    int fr_col;
    int to_row;
    int to_col;
};

int
do_stage1(arr board,int depth, int turn)
{
    // first we make a copy so we can use it to see it's children
    arr2 copy;
    copy_table(board,copy);

    int best_val;
    int max_player;

    // BLACK: Max player, starts with the lowest value
    // WHITE: Min player, starts with the highest value

    if (black_turn(turn)== TRUE){
        max_player = TRUE;
        best_val = INT_MIN;
    }else{
        best_val = INT_MAX;
        max_player = FALSE;
    }

    // best move
    struct best b;


    //checks each movement in a clockwise direction
    int r_mov[8] = {-1, -2, 1, 2, 1, 2, -1, -2};
    int c_mov[8] = {1, 2, 1, 2, -1, -2, -1, -2};

    for (int row = 0; row < BOARD_SIZE; ++row){
        for (int col = 0; col < BOARD_SIZE; ++col){
            for(int i = 0; i < sizeof(r_mov) ; ++i){
                // checkes each movement of each piece depending on the turn
                int fr_col = col;
                int fr_row = row;
                int to_row = row + r_mov[i];
                int to_col = col + c_mov[i];
                if (check_valid(fr_row,fr_col,to_row,to_col, board, turn)== TRUE){
                    // if the final node is reach, we make an evaluation of the board cost
                    if (depth == 0) {
                                return board_cost(board);
                        }
                    // do the valid move on the copy board
                    move(fr_row,fr_col,to_row,to_col,copy);


                    // recursive function where the depth of the boards decrease by 1 and the turn changes
                    // we do the function on the copy or stimulated board
                    int child_val = do_stage1(copy, depth - 1, turn + 1);

                    // the max player wants to maximise his score
                    // he will choose the move that gives him the highest board value

                    // the min player wants to minimise his score
                    // he will choose the move that gives him the lowest board value
                    if (max_player == TRUE && best_val < child_val||
                            max_player == FALSE && best_val > child_val){
                        best_val = child_val;
                        b.fr_row=fr_row;
                        b.fr_col=fr_col;
                        b.to_row=to_row;
                        b.to_col=to_col;

                    }
                    // after on loop, reset the board
                    copy_table(board,copy);
                }
            }
        }
    }

    //if the tree has reached the maximum depth print the turn
    if (depth == TREE_DEPTH)
    {
        // if our best value hasnt changed then a winner is decided
        if (best_val == INT_MAX) printf("BLACK WIN!");
        else if (best_val == INT_MIN) printf("WHITE WIN!");

        // else we print the move
        else {
            move(b.fr_row,b.fr_col,b.to_row,b.to_col, board);
            printf("=====================================\n");
            if (black_turn(turn) == TRUE){
                    printf("***BLACK ACTION #%d: %c%d-%c%d\n", turn, b.fr_col+'A',b.fr_row+1,b.to_col+'A',b.to_row+1);
	            } else {
	                printf("***WHITE ACTION #%d: %c%d-%c%d\n", turn, b.fr_col+'A',b.fr_row+1,b.to_col+'A',b.to_row+1);
	            }
                printf("BOARD COST: %d\n", board_cost(board));
                draw_board(board);
        }

    }
    return best_val;
}

void do_stage2(arr board, int turn)
{
    // stage 2 does stage 1 10 times
    for (int i = 1; i <= 10; ++i)
    {
        int curr_val = do_stage1(board, TREE_DEPTH, turn + i);

        // if there is already a winner the loop stops
        if (curr_val == INT_MAX || curr_val == INT_MIN) break;
    }
}

// draws the start board, different rows alternate
void
fill_board(arr board){
    for (int row = 0; row < BOARD_SIZE; row++){
        if (row < ROWS_WITH_PIECES){
            for (int col = 0; col < BOARD_SIZE; col+=2){
                if (row == 1){
                    board[row][col] = -1;
                    board[row][col+1] = 0;
                } else {
                    board[row][col] = 0;
                    board[row][col+1] = -1;
                }
            }
        } else if (BOARD_SIZE - ROWS_WITH_PIECES <= row){
            for (int col = 0; col < BOARD_SIZE; col+=2){
                if (row == 6){
                    board[row][col] = 0;
                    board[row][col+1] = 1;
                } else {
                    board[row][col] = 1;
                    board[row][col+1] = 0;
                }
            }
        } else {
            for (int col = 0; col < 8; col++){
                    board[row][col] = 0;
            }
        }
    }
}


void do_stage0()
{
    //draws the starting board
    arr board;
    fill_board(board);
    printf("BOARD SIZE: 8x8\n");
	printf("#BLACK PIECES: 12\n");
	printf("#WHITE PIECES: 12\n");
    draw_board(board);

    char fr_col, to_col;
    int fr_row, to_row;
    int turn = 0;

    // scans the each line of the file
    while(scanf("%c%d-%c%d\n", &fr_col, &fr_row, &to_col, &to_row) == 4)
    {
        // for every turn, its validity is checked, and thena new board is print
        turn ++;


        int err_type = check_valid(fr_row - 1, conv_col(fr_col), to_row - 1, conv_col(to_col), board, turn);
        switch(err_type)
        {
            case 1:
                printf("ERROR: Source cell is outside of the board.");
                return;
            case 2:
                printf("ERROR: Target cell is outside of the board.");
                return;
            case 3:
                printf("ERROR: Source cell is empty.");
                return;
            case 4:
                printf("ERROR: Target cell is not empty.");
                return;
            case 5:
                printf("ERROR: Source cell holds opponent’s piece/tower.");
                return;
            case 6:
                printf("ERROR: Illegal action.");
                return;
            default:
                move(fr_row - 1, conv_col(fr_col), to_row - 1, conv_col(to_col), board);
                printf("=====================================\n");

	            if (black_turn(turn) == TRUE){
                    printf("BLACK ACTION #%d: %c%d-%c%d\n", turn, fr_col, fr_row, to_col, to_row);
	            } else {
	                printf("WHITE ACTION #%d: %c%d-%c%d\n", turn, fr_col, fr_row, to_col, to_row);
	            }
                printf("BOARD COST: %d\n", board_cost(board));
                draw_board(board);
        }
        fr_row == FALSE;
    }
    // if the second element in the last row did change, then there is an action function
    if (fr_row != FALSE)
    {
        if (fr_col == 'A') do_stage1(board, TREE_DEPTH, turn+1);
        if (fr_col == 'P') do_stage2(board, turn);
    }
}

int main()
{
    do_stage0();
    return EXIT_SUCCESS;            // exit program with the success code
}
// algorithms are fun
/* THE END -------------------------------------------------------------------*/

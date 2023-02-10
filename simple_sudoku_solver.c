/* Program to assist in the challenge of solving sudoku puzzles.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   August 2021, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: QUANG CAT TUONG DUONG 1266256
   Dated:     10/09/2021

*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define NDIM 3		/* sudoku dimension, size of each inner square */
#define NDIG (NDIM*NDIM)
			/* total number of values in each row */
#define NGRP 3		/* number of sets each cell is a member of */
#define NSET (NGRP*NDIG)
			/* total number of sets in the sudoku */
#define NCLL (NDIG*NDIG)
			/* total number of cells in the sudoku */

#define ERROR	(-1)	/* error return value from some functions */

/* these global constant arrays provided as part of the initial skeleton,
   you may use them in your code but must not alter them in any way,
   regard them as being completely fixed. They describe the relationships
   between the cells in the sudoku and provide a basis for all of the
   sudoku processing loops */

/* there are 27 different different sets of elements that need to be
   checked against each other, this array converts set numbers to cells,
   that's why its called s2c */
int s2c[NSET][NDIM*NDIM] = {
	/* the first group of nine sets describe the sudoku's rows */
	{  0,  1,  2,  3,  4,  5,  6,  7,  8 },
	{  9, 10, 11, 12, 13, 14, 15, 16, 17 },
	{ 18, 19, 20, 21, 22, 23, 24, 25, 26 },
	{ 27, 28, 29, 30, 31, 32, 33, 34, 35 },
	{ 36, 37, 38, 39, 40, 41, 42, 43, 44 },
	{ 45, 46, 47, 48, 49, 50, 51, 52, 53 },
	{ 54, 55, 56, 57, 58, 59, 60, 61, 62 },
	{ 63, 64, 65, 66, 67, 68, 69, 70, 71 },
	{ 72, 73, 74, 75, 76, 77, 78, 79, 80 },
	/* the second group of nine sets describes the sudoku's columns */
	{  0,  9, 18, 27, 36, 45, 54, 63, 72 },
	{  1, 10, 19, 28, 37, 46, 55, 64, 73 },
	{  2, 11, 20, 29, 38, 47, 56, 65, 74 },
	{  3, 12, 21, 30, 39, 48, 57, 66, 75 },
	{  4, 13, 22, 31, 40, 49, 58, 67, 76 },
	{  5, 14, 23, 32, 41, 50, 59, 68, 77 },
	{  6, 15, 24, 33, 42, 51, 60, 69, 78 },
	{  7, 16, 25, 34, 43, 52, 61, 70, 79 },
	{  8, 17, 26, 35, 44, 53, 62, 71, 80 },
	/* the last group of nine sets describes the inner squares */
	{  0,  1,  2,  9, 10, 11, 18, 19, 20 },
	{  3,  4,  5, 12, 13, 14, 21, 22, 23 },
	{  6,  7,  8, 15, 16, 17, 24, 25, 26 },
	{ 27, 28, 29, 36, 37, 38, 45, 46, 47 },
	{ 30, 31, 32, 39, 40, 41, 48, 49, 50 },
	{ 33, 34, 35, 42, 43, 44, 51, 52, 53 },
	{ 54, 55, 56, 63, 64, 65, 72, 73, 74 },
	{ 57, 58, 59, 66, 67, 68, 75, 76, 77 },
	{ 60, 61, 62, 69, 70, 71, 78, 79, 80 },
};


/* there are 81 cells in a dimension-3 sudoku, and each cell is a
   member of three sets, this array gets filled by the function
   fill_c2s(), based on the defined contents of the array s2c[][] */
int c2s[NCLL][NGRP];

void
fill_c2s() {
	int s=0, d=0, c;
	for ( ; s<NSET; s++) {
		/* record the first set number each cell is part of */
		for (c=0; c<NDIM*NDIM; c++) {
			c2s[s2c[s][c]][d] = s;
		}
		if ((s+1)%(NGRP*NDIM) == 0) {
			d++;
		}
	}
#if 0
	/* this code available here if you want to see the array
	   cs2[][] that gets created, just change that 0 two lines back
	   to a 1 and recompile */
	for (c=0; c<NCLL; c++) {
		printf("cell %2d: sets ", c);
		for (s=0; s<NGRP; s++) {
			printf("%3d", c2s[c][s]);
		}
		printf("\n");
	}
	printf("\n");
#endif
	return;
}

/* find the row number a cell is in, counting from 1
*/
int
rownum(int c) {
	return 1 + (c/(NDIM*NDIM));
}

/* find the column number a cell is in, counting from 1
*/
int
colnum(int c) {
	return 1 + (c%(NDIM*NDIM));
}

/* find the minor square number a cell is in, counting from 1
*/
int
sqrnum(int c) {
	return 1 + 3*(c/NSET) + (c/NDIM)%NDIM;
}

/* If you wish to add further #defines, put them below this comment,
   then prototypes for the functions that you add

   The only thing you should alter above this line is to complete the
   Authorship Declaration
*/


/****************************************************************/


/****************************************************************/

/* main program controls all the action
*/
// using booleans

#define TRUE  1
#define FALSE  0

// using for our unknown array
#define KNOWN 8




/* part 1 Reading and Printing
we make a function called format that outputs a given array of 81 numbers into a neater format.
 by making this a function, we can call it at the start and at the end of the program */

void format(int input[]){
    printf("\n");
    int unknown = 0;
    for (int cell = 0; cell < NCLL; ++cell){

        /* we box every 3 values with a |, start a new line every 9 values, and seperate the squares vertically every 27 values */
        if (cell % 3 == 0 && cell%9 != 0) {
                printf("| ");
        }
        if (cell % 9 == 0) {
                printf("\n");
        }

        if (cell % 27 == 0 && cell != NCLL && cell != 0) {
                printf("------+-------+------\n");
        }
        /* every 0 inputed, we reformat to a period. we can also use this to count the number of unknowns */
        if (input[cell] == 0) {
                printf(". ");
                ++unknown;
        }
        else printf("%d ", input[cell]);
    }


    printf("\n\n %d cells are unknown\n", unknown); /* we print the amount of unknown cells after reformatting */
    if (unknown == 0){
            printf("ta daa!!!"); /* if there are no unknown cells, the sudoku is solved */
    }
}

/* part 2 Grid Checking
we can make a function that does two things:
1) tell us if the sudoku is valid
2) if the sudoku is invalid, print where */

int check_validity(int input[]){
    int valid=TRUE;
    int count=0;
    int tot_vio =0;
    int curr_set=TRUE;
    int set_vio=0;


    /* to check if the sudoku is valid, we check the amount of times each number between 1 and 9 appear in each set */
    for (int set_num = 0; set_num < NSET; ++set_num){
        for (int curr_val = 1; curr_val <= NDIG; ++curr_val){
            for (int curr_cell = 0; curr_cell < NDIG; curr_cell++){
                if (curr_val == input[s2c[set_num][curr_cell]]){
                    count++; /* using the s2c function, we can count the amount of times a number appears in a set */
                }
            }
            if (count>1){
                printf("\n");
                /* the count of the number can only be 1 or less, therefore is it a violation if it is more */
                if (set_num <9) {
                    printf("set %d (row %d): %d instances of %d", set_num, set_num+1, count, curr_val);
                        /* the set number refers to the row, col, sqr. the subset is the set number - row */
                } else if (set_num < 18) {
                     printf("set %d (col %d): %d instances of %d", set_num, set_num -8, count, curr_val);
                } else {
                    printf("set %d (sqr %d): %d instances of %d", set_num, set_num-17, count, curr_val);
                }

                /* everytime the count is more then 1, the total violations increase by 1. this also means that the current set is invalid */
                tot_vio++;
                curr_set=FALSE;
            }
            /* the count is resetted to 0 because we need to count the next number */
            count = 0;
        }
        /* is the current set is invalid, out total amount of invalid set increases */
        if (curr_set==FALSE){
            set_vio++;
        }
        curr_set = TRUE;
    }

    /* if there is more then one set violation, the soduku is invalid */
    if (set_vio != 0) {
            printf("\n\n%d different sets have violations", set_vio);
            printf("\n%d violations in total", tot_vio);
            valid = FALSE;
    }
    return valid;
}

/* part 3 strategy 1
we can write a function that checks each cell in the sudoku, and relate it to all its groups
*/

void strategy_one(int input[])
{
    int ans_found = FALSE; // answer that has to be found in strategy 1
    int loop = TRUE; // loop that controls when strategy 1 is exhausted

    printf("\n");
    printf("strategy one\n");

    int sum = 0; // sum that we can use to count the number of known variables
    while (loop == TRUE)
    {
        loop = FALSE;

        for (int cell = 0; cell < NCLL; ++cell){
            ans_found = FALSE;
            /* find the cell from that input sudoku that is unknown */

            if (input[cell] == 0){
                /* a blank array that is used to check 3 groups. if there is a known number, it changes it to 1*/
                int value[10] = {0,0,0,0,0,0,0,0,0,0};
                for (int set = 0; set < NGRP; set++){
                    for (int place = 0; place < 9; place++){
                        if (input[s2c[c2s[cell][set]][place]] != 0){
                            value[input[s2c[c2s[cell][set]][place]]] = 1;
                        }
                    }
                }

                /* sums the total amount of known values */
                for (int count= 1; count <= NDIG; ++count){
                    sum = sum + value[count];
                }

                /*if the sum is equal to the highest amount of known values for the sum (8) it will print the answer, and change it from the input */
                if (sum == KNOWN){
                    for (int ans = 1; ans <= NDIG; ++ans){
                        if (value[ans] == 0){
                            ans_found = TRUE;
                            loop = TRUE;
                            input[cell] = ans;
                            printf("row %d col %d must be %d\n", rownum(cell), colnum(cell), ans);
                        }
                    }
                }
                sum = 0;
            }

        }
         if (loop == TRUE){
            printf("\n");
            printf("strategy one\n");

        }

    }
}




int
main(int argc, char *argv[])
{
    /* scans the file and puts each value into an array of 81 values */
    int input[NCLL];
    for (int i = 0; i < NCLL; ++i) scanf("%d", &input[i]);

    /* fills the cell to set function */
    fill_c2s();

    /* reformats the user's input */
    format(input);

    /* checks the validity of the input. if is it valid, performs strategy one */
    if (check_validity(input) == TRUE){
        strategy_one(input);

        /* the solution is printed using our formatting funtion*/
        format(input);

    } else {
        return 0;
    }


	/* all done, so pack up bat and ball and head home */
	return 0;
}
/* algorithms are fun */

/****************************************************************/


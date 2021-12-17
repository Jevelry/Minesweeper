// This program was written by Kevin Huang (z5311917)
// on 8/3/2020
//
// Version 1.0.0 (2020-03-08): Assignment released.

#include <stdio.h>
#include <stdlib.h>

// Possible square states.
#define VISIBLE_SAFE    0
#define HIDDEN_SAFE     1
#define HIDDEN_MINE     2

// The size of the starting grid.
#define SIZE 8

// The possible command codes.
#define DETECT_ROW              1
#define DETECT_COL              2
#define DETECT_SQUARE           3
#define REVEAL_SQUARE           4
#define GAMEPLAY_MODE           5
#define DEBUG_MODE              6
#define REVEAL_RADIAL           7

// Add any extra #defines here.

void initialise_field (int minefield[SIZE][SIZE]);
void print_debug_minefield (int minefield[SIZE][SIZE]);
void print_gameplay_mode_minefield (int minefield[SIZE][SIZE], int *game_over);
void detect_column (int minefield[SIZE][SIZE], int column);
void detect_row (int minefield[SIZE][SIZE], int row);
void detect_square (int minefield[SIZE][SIZE], int row, int column, int square_size);
void reveal_square (int minefield[SIZE][SIZE], int row, int column, int *game_over, int *safe_first_turn);
void reveal_radial (int minefield[SIZE][SIZE], int row, int column, int *game_over, int *safe_first_turn);
void check_win (int minefield[SIZE][SIZE], int mine_pairs, int *game_won);
void check_mine_gameplay_mode (int minefield[SIZE][SIZE], int i, int j);
void check_mine_reveal_radial (int minefield[SIZE][SIZE],int i, int j, int *loop_end);
void shift_mine_safe_turn (int minefield[SIZE][SIZE]);

// Place your function prototyes here.

int main(void) {
    int minefield[SIZE][SIZE];
    int mine_pairs = 0;
    initialise_field(minefield);
    printf("Welcome to minesweeper!\n");
    printf("How many mines? ");

    // Scan in the number of pairs of mines.
    scanf("%d", &mine_pairs);
        
    // Scan in the pairs of mines and place them on the grid.
    int counter = 0;
    printf("Enter pairs:\n");
    while (counter < mine_pairs) {
        int i = 0;
        int j = 0;        
        scanf("%d""%d", &i, &j);
        if (i <= SIZE && i >= 0 && 
            j <=SIZE && j >= 0) {
            minefield[i][j] = HIDDEN_MINE;
        }
        counter++;
    }
            
    printf("Game Started\n");
    print_debug_minefield(minefield);

    // Scan in commands to play the game        
    int command_code;
    // If game_over = 1, game is over
    int game_over = 0; 
    // If game_won = 1,game is over and won
    int game_won = 0;
    // Counts clues used 
    int used_clue = 0; 
    // 0 for debug_mode, 1 for gameplay_mode
    int game_mode = 0; 
    // 0 means safe turn is off
    int safe_first_turn = 1;
    while (scanf("%d", &command_code) != -1 && game_over == 0 && game_won == 0) {
        // Runs DETECT_ROW command
        if (command_code == DETECT_ROW) {
            if (used_clue < 3) {
                int row = 0;
                scanf("%d", &row);
                detect_row (minefield, row); 
                used_clue++;
            }
            else {
                // Filler variable to compensate for extra input 
                int extra = 0; 
                scanf("%d", &extra);
                printf("Help already used\n");
            }          
        }
        
        // Runs DETECT_COLUMN command
        if (command_code == DETECT_COL) {
            if (used_clue < 3) {
                int column = 0;
                scanf("%d", &column);
                detect_column (minefield, column); 
                used_clue++;
            }   
            else {
                //Filler variable to compensate for extra input
                int extra = 0; 
                scanf("%d", &extra);
                printf("Help already used\n");
            }           
        }
        
        //Runs DETECT_SQUARE command
        if (command_code == DETECT_SQUARE) {
            if (used_clue < 3) {
                int column = 0;
                int row = 0;
                int square_size = 0;
                scanf("%d""%d""%d", &column, &row, &square_size);
                detect_square (minefield, row, column, square_size);
                used_clue++;
            }
            else {
                // Filler variables to cancel out the extra inputs
                int a = 0;
                int b = 0;
                int c = 0; 
                scanf("%d""%d""%d", &a, &b, &c);
                printf("Help already used\n");
            }             
        }
        
        // Runs REVEAL_SQUARE command
        if (command_code == REVEAL_SQUARE) {
            int row = 0;
            int column = 0;            
            scanf("%d""%d", &row, &column);
            reveal_square (minefield, row, column, &game_over, &safe_first_turn);            
        }
        
        // Runs REVEAL_RADIAL command_code
        if (command_code == REVEAL_RADIAL) {
            int row = 0;
            int column = 0;
            scanf("%d""%d", &row, &column);
            reveal_radial (minefield, row, column, &game_over, &safe_first_turn); 
        }
        // Check if game is over     
        if (game_over == 1 && safe_first_turn == 0) {
            printf ("Game over\n"); 
            if (game_mode == 1) {
                printf("xx\n/\\\n");                                       
            }         
        }
        
        // Check if player has won
        check_win(minefield,mine_pairs, &game_won);
        if (game_won == 1) {
            printf ("Game Won!\n");              
        }
        
        // Changing game mode
        if (command_code == GAMEPLAY_MODE) {
            printf("Gameplay mode activated\n");            
            game_mode = 1;
        }    
        if (command_code == DEBUG_MODE) {
            printf("Debug mode activated\n");
            game_mode = 0;          
        } 
                   
        // Print minefield after every command                 
        if (game_mode == 1) {
            print_gameplay_mode_minefield(minefield, &game_over);
        }
        else {                             
            print_debug_minefield(minefield);
        }                    
    }       
    return 0;
}

// FUNCTION AND COMMANDS******************************************************

// Set the entire minefield to HIDDEN_SAFE.
void initialise_field(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            minefield[i][j] = HIDDEN_SAFE;
            j++;
        }
        i++;
    }
}

// DEBUG MODE MINEFIELD************************
void print_debug_minefield(int minefield[SIZE][SIZE]) {
    int i = 0;
    while (i < SIZE) {
        int j = 0;
        while (j < SIZE) {
            printf("%d ", minefield[i][j]);
            j++;
        }
        printf("\n");
        i++;
    }
}

// GAMEPLAY MODE MINEFIELD*************************
void print_gameplay_mode_minefield (int minefield[SIZE][SIZE], int *game_over){
    // Prints the smiley face
    if (*game_over == 0) {
        printf("..\n");
        printf("\\/\n");
    }
    //Prints scaffold of minefield
    printf("    00 01 02 03 04 05 06 07\n");
    printf("   -------------------------\n");
    int i = 0;
    while (i < SIZE) {
        printf("0%d |", i); 
        int j = 0;
        while (j < SIZE) {
           
            // Checks for number of nearby mines
            if (minefield[i][j] == VISIBLE_SAFE) {            
                check_mine_gameplay_mode (minefield, i, j); 
            }
            // Replace mines with () when game is over
            else if (minefield[i][j] == HIDDEN_MINE && *game_over == 1) {
                printf("(");
                printf(") "); 
            }  
            // For hidden mine or hidden safe                      
            else {               
                printf("## "); 
            }               
            j++;        
        }
        i++;
        printf("|\n");                    
    }                  
    printf("   -------------------------\n");
}


//CHECK_MINE_GAMEPLAY FUNCTION************************
void check_mine_gameplay_mode (int minefield[SIZE][SIZE], int i, int j) {
    int k = 0;
    // Counter for surrounding mines 
    int mine_counter = 0;
    // Accounts for invalid array index. skips row if so 
    if ((i - 1 + k) < 0) { 
        k++;
    }    
    while (k < 3 && (i - 1 + k) < 8) {
        // Accounts for invalid array index. skips column if so       
        int l = 0;
        if ((j - 1 + l) < 0) { 
            l++;
        }
        // Adds one to mine_counter if loop goes over a mine. 
        // Scans left to right, top to bottom
        while (l < 3 && (j - 1 + l) < 8) {         
            if (minefield[i - 1 + k][j - 1 + l] == HIDDEN_MINE) {
                mine_counter++; 
            }
            l++;
        }      
        k++;
    }
    // Print number based on surrounding mines
    if (mine_counter == 0) {
        printf("   ");
    }    
    else if (mine_counter == 1) {           
        printf ("01 ");
    }
    else if (mine_counter == 2) {
        printf ("02 ");
    } 
    else if (mine_counter == 3) {
        printf ("03 ");
    }   
    else {
        printf ("04 ");
    }
}

// DETECT ROW COMMAND****************************
void detect_row (int minefield[SIZE][SIZE], int row) {   
    int counter = 0;
    int mine_counter = 0;
    // Adds one to mine_counter if loop goes over a mine
    while (counter < SIZE) {
        if ((minefield[row][counter]) == HIDDEN_MINE ) {
            mine_counter++;
        }    
        counter++;
    }
    printf("There are %d mine(s) in row %d\n", mine_counter, row);
}
    
// DETECT COLUMN COMMAND*************************
void detect_column (int minefield[SIZE][SIZE], int column) {   
    int counter = 0;
    int mine_counter = 0;
    // Adds one to mine_counter if loop goes over a mine
    while (counter < SIZE) {
        if ((minefield[counter][column]) == HIDDEN_MINE ) {
            mine_counter++; 
        }    
        counter++;
    }
    printf("There are %d mine(s) in column %d\n", mine_counter, column);                 
}

// DETECT SQUARE COMMAND************************
void detect_square (int minefield[SIZE][SIZE], int row, int column, int square_size) {
    // Represents row
    int i = 0;          
    int mine_counter = 0;
    while (i < square_size) {
        // Represents column
        int j = 0;      
        // Square checks mines from top to bottom, left to right
        while (j < square_size) {
            if (minefield[row - square_size + 2 + i]
                [column - square_size + 2 + j] == HIDDEN_MINE) {
                mine_counter++; 
            }
            j++;
        }      
        i++;
    }
    printf(
    "There are %d mine(s) in the square centered at row %d, column %d of size %d\n"
    , mine_counter, row, column, square_size);
}

// REVEAL_SQUARE COMMAND*******************************
void reveal_square (int minefield[SIZE][SIZE], int row, int column, 
                    int *game_over, int *safe_first_turn) {
    // Checks if square selected is a mine    
    if (minefield[row][column] == HIDDEN_MINE && *safe_first_turn == 0) {
        *game_over = 1;         
    }
    // Applies safe first turn
    if (minefield[row][column] == HIDDEN_MINE && *safe_first_turn == 1) {
        shift_mine_safe_turn(minefield);
        *safe_first_turn = 0;
               
    }
    // Checks if adjacent squares are mines and if so, makes selected square safe   
    if (*game_over == 0) { 
        // Represents row        
        int i = 0;    
        // Accounts for invalid index of array (i.e minefield[-1][-1] = -1). 
        // Skips row if so  
        if ((row - 1 + i) < 0) {
            i++;                
        }        
        while (i < 3 && (row - 1 + i) < 8 
               && (row - 1 + i) >= 0) {
            // Represents column
            int j = 0; 
            // Terminates loop  
            int over = 0;
            // Accounts for invalid index of array (i.e minefield[-1][-1] = -1). 
            // If so, skips column                 
            if ((column - 1 + j) < 0) {
                j++;            
            }
            // Makes selected square safe if adjacents are a mine   
            while (j < 3 && (column - 1 + j) < 8 
                   && (column - 1 + j) >= 0 && over == 0) {
                if (minefield[row - 1 + i][column - 1 + j] == HIDDEN_MINE) {
                    minefield[row][column] = VISIBLE_SAFE; 
                    // Ends loop
                    over = 1; 
                }                 
                j++;
            }      
            i++;
        } 
        // Turn safe_first_turn off      
        if (*safe_first_turn == 1) {
            *safe_first_turn = 0;
        }
    }     
    // Otherwise, reveals all adjacent sqaures
    if (*game_over == 0 && minefield[row][column] != VISIBLE_SAFE) {
        // Represents row
        int k = 0; 
        // Accounts for invalid index of array (i.e minefield[-1][-1] = -1). 
        // If so, skips row
        if ((row - 1 + k) < 0) {
            k++;            
        }
        while (k < 3 && (row - 1 + k) >= 0 && (row - 1 + k) < 8) {
            int l = 0; //represents column
            // Accounts for invalid index of array (i.e minefield[-1][-1] = -1). 
            // If so, skips column
            if ((column - 1 + l) < 0) {
                l++;        
            }          
            // Loops through making all squares visible safe 
            while (l < 3 && (column - 1 + l) < 8 
                   && (column - 1 + l) >= 0) {
                (minefield[row - 1 + k][column - 1 + l]) = VISIBLE_SAFE;
                l++;           
            }
            k++;        
        }
        // Turn safe_first_turn off 
        if (*safe_first_turn == 1) {
            *safe_first_turn = 0;
        } 
    }      
}   

// REVEAL_RADIAL COMMAND*******************************
void reveal_radial (int minefield[SIZE][SIZE], int row, int column, 
                    int *game_over, int *safe_first_turn) {
    // Checks if square selected is a mine    
    if (minefield[row][column] == HIDDEN_MINE && *safe_first_turn == 0) {
        *game_over = 1;         
    }
    // Applies safe first turn
    if (minefield[row][column] == HIDDEN_MINE && *safe_first_turn == 1) {
        shift_mine_safe_turn(minefield);
        *safe_first_turn = 0;             
    }
     
    // Checks if adjacent squares are mines and if so, makes selected square safe   
    if (*game_over == 0) { 
        // Represents row        
        int i = 0;    
        // Accounts for invalid index of array (i.e minefield[-1][-1] = -1). 
        // Skips row if so  
        if ((row - 1 + i) < 0) {
            i++;                
        }        
        while (i < 3 && (row - 1 + i) < 8 
               && (row - 1 + i) >= 0) {
            // Represents column
            int j = 0; 
            // Terminates loop  
            int over = 0;
            // Accounts for invalid index of array (i.e minefield[-1][-1] = -1). 
            // If so, skips column                 
            if ((column - 1 + j) < 0) {
                j++;            
            }
            // Makes selected square safe if adjacents are a mine   
            while (j < 3 && (column - 1 + j) < 8 
                   && (column - 1 + j) >= 0 && over == 0) {
                if (minefield[row - 1 + i][column - 1 + j] == HIDDEN_MINE) {
                    minefield[row][column] = VISIBLE_SAFE; 
                    // Ends loop
                    over = 1; 
                }                 
                j++;
            }      
            i++;
        }
        // Turn safe_first_turn off 
        if (*safe_first_turn == 1) {
            *safe_first_turn = 0;
        }  
    }     
    // Otherwise, reveals all adjacent and radial outward sqaures
    if (*game_over == 0 && minefield[row][column] != VISIBLE_SAFE) {       
        int a = 0, b = 0, c = 0, d = 0, e = 0, f = 0, g = 0, h = 0;        
        int arm_0Deg = 1, arm_45Deg = 1, arm_90Deg = 1, arm_135Deg = 1, 
            arm_180Deg = 1, arm_225Deg = 1, arm_270Deg = 1, arm_315Deg = 1;                     
        // reveals through each arm, stopping when there is adjacent mine   
        minefield[row][column] = VISIBLE_SAFE;    
        while (row - arm_0Deg >= 0 
               && a == 0) {
            minefield[row - arm_0Deg][column] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row - arm_0Deg, column, &a);
            arm_0Deg++;
        }    
        while (row - arm_45Deg >= 0
               && column + arm_45Deg < 8       
               && b == 0) {
            minefield[row - arm_45Deg][column + arm_45Deg] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row - arm_45Deg, column + arm_45Deg, &b);
            arm_45Deg++;
        }    
        while (column + arm_90Deg < 8 
               && c == 0) {
            minefield[row][column + arm_90Deg] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row, column + arm_90Deg, &c);
            arm_90Deg++;
        }    
        while (row + arm_135Deg < 8 
               && column + arm_135Deg < 8
               && d == 0) {
            minefield[row + arm_135Deg][column + arm_135Deg] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row + arm_135Deg, 
                                      column + arm_135Deg, &d);
            arm_135Deg++;
        }    
        while (row + arm_180Deg < 8 
               && e == 0) {
            minefield[row + arm_180Deg][column] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row + arm_180Deg, column, &e);
            arm_180Deg++;
        }    
        while (row + arm_225Deg < 8 
               && column - arm_225Deg >= 0
               && f == 0) {
            minefield[row + arm_225Deg][column - arm_225Deg] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row + arm_225Deg, 
                                      column - arm_225Deg, &f);
            arm_225Deg++;
        }    
        while (column - arm_270Deg >= 0 
               && g == 0) {
            minefield[row][column - arm_270Deg] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row, column - arm_270Deg, &g);
            arm_270Deg++;
        }    
        while (row - arm_315Deg >= 0 
               && column - arm_315Deg >= 0
               && h == 0) {
            minefield[row - arm_315Deg][column - arm_315Deg] = VISIBLE_SAFE;
            check_mine_reveal_radial (minefield, row - arm_315Deg, 
                                      column - arm_315Deg, &h);
            arm_315Deg++;      
        }
        // Turn safe_first_turn off 
        if (*safe_first_turn == 1) {
            *safe_first_turn = 0;
        } 
    }
} 

// CHECK_MINE_REVEAL_RADIAL FUNCTION*******************************
void check_mine_reveal_radial (int minefield[SIZE][SIZE], int i, int j, int *loop_end) {
    int k = 0;
    // Counts number of surrounding mines         
    // Accounts for invalid array index. skips row if so 
    if ((i - 1 + k) < 0) { 
        k++;
    }    
    while (k < 3 && (i - 1 + k) < 8) {
        // Accounts for invalid array index. skips column if so       
        int l = 0;
        if ((j - 1 + l) < 0) { 
            l++;
        }
        // Adds one to mine_counter if loop goes over a mine. 
        // Scans left to right, top to bottom
        while (l < 3 && (j - 1 + l) < 8) {         
            if (minefield[i - 1 + k][j - 1 + l] == HIDDEN_MINE) {
                *loop_end = 1;                 
            }
            l++;
        }      
        k++;

    }
}

// SHIFT_MINE_SAFE_TURN FUNCTION*************************
void shift_mine_safe_turn (int minefield[SIZE][SIZE]) {
    // Represents column
    int j = 7;   
    while (j >= 0) {
        // Represents row
        int i = 7;
        // Used to remember if row 7 has a mine
        int row_mine = 0;
        while (i >= 0) {
            // If row 7 has a mine, row_mine stores and remembers it
            if (i == 7 && minefield[i][j] == HIDDEN_MINE) {
                minefield[i][j] = HIDDEN_SAFE;
                row_mine = 1;
            }
            // Else, if sqaure is a mine, shift it down by 1
            else if (i >= 0 && minefield[i][j] == HIDDEN_MINE) {        
                minefield[i][j] = HIDDEN_SAFE;
                minefield[i + 1][j] = HIDDEN_MINE;
            }                          
            i--;
            // If row 7 had a mine, shift it down to row 0
            if (row_mine == 1) {
                minefield[0][j] = HIDDEN_MINE;
            } 
        }              
        j--;
    }       
}

//CHECK_WIN AFTER COMMAND************************
void check_win (int minefield[SIZE][SIZE], int mine_pairs, int *game_won){
    // Represents row
    int i = 0; 
    // Used to count all VISIBLE_SAFE squares
    int win_checker = 0;
    while (i < SIZE) {
        // Represents column
        int j = 0; 
        // Adds 1 to win_checker if square is VISIBLE_SAFE
        while (j < SIZE) {
            if (minefield[i][j] == VISIBLE_SAFE) {             
                win_checker++;                  
            }
            j++; 
        }        
        i++;
    }
    // If all squares matches expected amount, game_won = 1
    if (win_checker == 64 - mine_pairs) {
        *game_won = 1; 
    }
}          

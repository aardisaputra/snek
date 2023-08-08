#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t* state, unsigned int snum);
static char next_square(game_state_t* state, unsigned int snum);
static void update_tail(game_state_t* state, unsigned int snum);
static void update_head(game_state_t* state, unsigned int snum);

/* Task 1 */
game_state_t* create_default_state() {
  // TODO: Implement this function.
    game_state_t* new_state = malloc((sizeof(game_state_t)));
    
    new_state->num_rows = 18;
    new_state->num_snakes = 1;
    
    char* border = "####################\n";
    char* sides = "#                  #\n";

    new_state->board = malloc((sizeof(char *)*18));
    for (int i = 0; i < 18; ++i) new_state->board[i] = malloc((sizeof(char *)*21));
    strcpy(*(new_state->board), border);

    for (int i = 1; i < 17; ++i) {
       strcpy(*(new_state->board + i), sides);
    };

    strcpy(*(new_state->board + 17), border);

    snake_t* snek = malloc(sizeof(snake_t));

    (*snek).tail_row = 2;
    (*snek).tail_col = 2;
    (*snek).head_row = 2;
    (*snek).head_col = 4;
    snek->live = 1;

    (*new_state).snakes = snek;
        
    strcpy(*(new_state->board + 2), "# d>D    *         #\n");

    return new_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  for (unsigned int i = 0; i < state->num_rows; ++i) free((state->board[i]));
  free((state->snakes));
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  for (int i = 0; i < state->num_rows; ++i) fprintf(fp, "%s", (state->board[i]));
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t* state, unsigned int row, unsigned int col) {
  return state->board[row][col];
}

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t* state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  if (strchr("wasd", c) == NULL) {
    return false;
  }
  return true;
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
    if (strchr("WASDx", c) == NULL) {
        return false;
    }
    return true;
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  if (strchr("wasd^<v>WASDx", c) == NULL) {
    return false;
  }
  return true;
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/

static char body_to_tail(char c) {
  // TODO: Implement this function.
  char body[] = {'^', '<', 'v', '>', '\0'};
  char tail[] = {'w', 'a', 's', 'd', '\0'};

  for (int i = 0; i < strlen(body); i++) {
      if (c == body[i]) {
        return tail[i]; 
    }
  }
  return ' ';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  char head[] = {'W', 'A', 'S', 'D', '\0'};
  char body[] = {'^', '<', 'v', '>', '\0'};

  for (int i = 0; i < strlen(head); i++) {
    if (c == head[i]) {
        return body[i];
    }
  }

  return ' ';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
    char down[] = {'v', 's', 'S', '\0'};
    char up[] = {'^', 'w', 'W', '\0'};

    if (strchr(down, c) != NULL) {
        return cur_row + 1;
    }

    if (strchr(up, c) != NULL) {
        return cur_row - 1;
    }

    return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
    char left[] = {'<', 'a', 'A', '\0'};
    char right[] = {'>', 'd', 'D', '\0'};

    if (strchr(left , c) != NULL) {
        return cur_col - 1;
    } 

    if (strchr(right, c) != NULL) {
        return cur_col + 1;
    }

    return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/
static char next_square(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
    unsigned int row = state->snakes[snum].head_row;
    unsigned int col = state->snakes[snum].head_col;

    char head = get_board_at(state, row, col);

    char next = get_board_at(state, get_next_row(row, head), get_next_col(col, head));

    return next;
}

/*
  Task 4.3

  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/

static void update_head(game_state_t* state, unsigned int snum) {
  unsigned int curr_row = state->snakes[snum].head_row;
  unsigned int curr_col = state->snakes[snum].head_col;

  char curr_head = get_board_at(state, curr_row, curr_col);
  set_board_at(state, curr_row, curr_col, head_to_body(curr_head));
  
  unsigned int next_row = get_next_row(curr_row, curr_head);
  unsigned int next_col = get_next_col(curr_col, curr_head);
  set_board_at(state, next_row, next_col, curr_head);

  state->snakes[snum].head_row = next_row;
  state->snakes[snum].head_col = next_col;

  return;

}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
    size_t row = state->snakes[snum].tail_row;
    size_t col = state->snakes[snum].tail_col;
    char tail = get_board_at(state, row, col);
    set_board_at(state, row, col, ' ');

    size_t next_row = get_next_row(row, tail);
    size_t next_col = get_next_col(col, tail);
    char direction = get_board_at(state, next_row, next_col);
    direction = body_to_tail(direction);

    set_board_at(state, next_row, next_col, direction);

    state->snakes[snum].tail_row = next_row;
    state->snakes[snum].tail_col = next_col;

    return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  // TODO: Implement this function.

    //Check head
    //If in front of head is wall --> change head to x, live to false
    //If in front of head is fruit --> update head, skip tail, add_food
    //Update head if haven't, update tail
    
    for (unsigned int i = 0; i < state->num_snakes; i++) {
    
        unsigned int head_row = state->snakes[i].head_row;
        unsigned int head_col = state->snakes[i].head_col;

        char head = get_board_at(state, head_row, head_col);
        char next = get_board_at(state, get_next_row(head_row, head), get_next_col(head_col, head));
        bool eaten = false;

        if (next != ' ' && next != '*') {
            set_board_at(state, head_row, head_col, 'x');
            state->snakes[i].live = false;
            continue;

        } else if (next == '*') {
            eaten = true;
        }

        update_head(state, i);

        if (eaten == false) {
            update_tail(state, i);
        }

        if (eaten) {
            add_food(state);
        }
    }

  return;

}

/* Task 5 */
game_state_t* load_board(char* filename) {
  // TODO: Implement this function.
  struct game_state_t* state_ptr = malloc(sizeof(game_state_t));

  FILE* ptr = fopen(filename, "r");

  if (ptr == NULL) {
    return NULL;
  }

  size_t rows = 0;
  size_t curr = 0;
  size_t max = 0;
  char c;

  while(!feof(ptr)) {
    if (c == '\n') {
        rows++;
        if (curr >= max) {
            max = curr;
            curr = 0;
        }
    }
    c = (char) fgetc(ptr);
    curr++;
  }

  fclose(ptr);
  state_ptr->board = (char **) malloc(sizeof(char*) * rows);
  char buffer[max + 2];

  ptr = fopen(filename, "r");
  size_t row_idx = 0;
  size_t curr_row = 0;
  while (!feof(ptr)) {
    c = (char) fgetc(ptr);
    char* legal = "WASDwasd<^v>#* ";
    if (strchr(legal, c) != NULL) {
        row_idx++;
        buffer[row_idx - 1] = c;
    }
    else 
    {   
        if (c != '\n') {
            row_idx = 0;
            break;
        }
        
        row_idx++;
        buffer[row_idx - 1] = c;
        buffer[row_idx] = '\0';
        
        state_ptr->board[curr_row] = malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(state_ptr->board[curr_row], buffer);
        curr_row++;
        row_idx = 0;
    }
  }

  state_ptr->num_rows = rows;
  state_ptr->snakes = NULL;
  state_ptr->num_snakes = 0;
  
  return state_ptr;
}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t* state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t curr = state->snakes[snum];
  char tail = state->board[curr.tail_row][curr.tail_col];
  unsigned int curr_row = curr.tail_row;
  unsigned int curr_col = curr.tail_col;
  char heads[] = {'D', 'A', 'S', 'W'};
  while (strchr(heads, tail) == NULL) {
    curr_row = get_next_row(curr_row, tail);
    curr_col = get_next_col(curr_col, tail);
    tail = state->board[curr_row][curr_col];
  }
  state->snakes[snum].head_row = curr_row;
  state->snakes[snum].head_col = curr_col;
  //printf("%d", state->snakes[snum].head_row);
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  unsigned int num_snakes = 0;
  for (unsigned int i = 0; i < state->num_rows; ++i){
    char* line = state->board[i];
    printf("%s", line);
   for (unsigned int j = 0; j < strlen(line); ++j){
        if (strchr("wasd", line[j])){
            num_snakes += 1;
            state->snakes = realloc(state->snakes, sizeof(snake_t) * num_snakes);
            state->snakes[num_snakes - 1].tail_row = i;
            state->snakes[num_snakes - 1].tail_col = j;
            state->snakes[num_snakes - 1].live = true;
            find_head(state, num_snakes-1);
        }
    }
  }
  state->num_snakes = num_snakes;
  return state;
}


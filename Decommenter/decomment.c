/* Author: Mai Tung Duong
   Assignment 1
   File: decomment.c */
#include <stdio.h>
#include <stdlib.h>

/* DATA STRUCTURE */

/* State of the current program:
   code/comment/string/character constant */
enum DFAState {CODE, COMMENT, STRING, CHAR};

/* State of the a character: free or bound 
   (bound means the character is already bound
   with the previous character to form a special
   expression (comment opener or closer) */
enum CharState {FREE, BOUND};

/* Character is struct with two attributes:
   - content: the actually character it represents
   - state: it is bound or not */
struct Character {
    char content;
    enum CharState state;
};

/* CONSTANT */
/* Constant represents a free backslash 
   and a free star character*/
struct Character const slash = {'/', FREE};
struct Character const star = {'*', FREE};

/* GLOBAL VARIABLES */
int line;                           // keep track of line number
int current_comment_start_line;     // keep track of where the 
                                    // current comment starts

struct Character previous_char;     // previous character
struct Character current_char;      // current character
struct Character next_char;         // next character

enum DFAState state;                // current state

/* CHARACTER STRUCT HELPER FUNCTIONS */

/* Copy 'from' character (with free/bound state) to 'to' character */
void copy_from_to (struct Character* from, struct Character* to) {
    to -> content = from -> content;
    to -> state = from -> state;
}

/* Compare if two characters are equivalent
   It returns 1 if equal, 0 otherwise */
int is_equal (struct Character a, struct Character b) {
    return (a.content == b.content && a.state == b.state);
}

/* MAIN HELPER FUNCTIONS */

/* Initialize the variables and handle the first character.
   It intializes current_char, next_char, state, line. 
   It reads the first character from standard input stream, and 
   writes the first character of the standard output stream */

void initialization () {
    line = 1;

    current_char.content = getchar();
    current_char.state = FREE;

    next_char.content = getchar();
    next_char.state = FREE;
    
    state = (current_char.content == '\"') 
        ? STRING 
        : (current_char.content == '\'') 
            ? CHAR 
            : (current_char.content == '/') 
            && (next_char.content == '*')
                ? COMMENT
                : CODE;

    if (state == COMMENT) {
        fprintf(stdout," ");
        current_comment_start_line = line;
        next_char.state = BOUND;
    }
    else fprintf(stdout, "%c", current_char.content);
}

/* Shift the position one character (step) forward.
   It updates previous_char and current_char.
   It reads one character from standard input stream, 
   and update the next_char */

void update() {
    copy_from_to(&current_char, &previous_char);
    copy_from_to(&next_char, &current_char);
    next_char.content = getchar();
    next_char.state = FREE;
    if (current_char.content == '\n') line++;
}

/* Handler for EOF 
   It terminates and returns the exit status, and
   write to stderr in case of unterminated comment */
int eof_handler () {
    if (state == COMMENT) {
        fprintf(stderr, 
            "Error: line %d: unterminated comment\n", 
            current_comment_start_line);
        return EXIT_FAILURE; 
    }
    return EXIT_SUCCESS;
}

/* Check for double quotation 
   It changes the state to STRING once it meet an opening 
   double quotation
   It changes the state to CODE once it meet a closing 
   double quotation " */

void check_double_quotation() {
    if ((current_char.content == '\"') 
        && (previous_char.content != '\\')) {
           state = (state == STRING) ? CODE : STRING; 
    }
}

/* Check for single quotation 
   It changes the state to STRING once it meet an opening 
   single quotation
   It changes the state to CODE once it meet a closing 
   single quotation " */
void check_single_quotation() {
    if ((current_char.content == '\'') 
        && (previous_char.content != '\\')) {
            state = (state == CHAR) ? CODE : CHAR; 
    }
}

/* Check for comment opening.
   It writes a space to the standard output stream and 
   update state to COMMENT.
   It binds the next_char and retrieve current_comment_start_line */

void check_comment_opening(){
    if (is_equal(current_char, slash) && is_equal(next_char, star)) {
        state = COMMENT;
        next_char.state = BOUND;
        fprintf(stdout, " ");
        current_comment_start_line = line;    
    }
    else fprintf(stdout, "%c", current_char.content);
}

/* Check for comment closing.
   It updates state to CODE and binds the next_char. */
void check_comment_closing(){
    if (is_equal(previous_char, star) && is_equal(current_char, slash)){
        state = CODE;
        current_char.state = BOUND;
    }
}


/* MAIN FUNCTION */
/* It (1) reads the standard input stream (C source code), 
   (2) write the text with comment replaced by a space 
   to the standard output stream (decomment) 
   (3) write error and warning messages (if necessary)
   to the standard error stream
   It returns the exit status*/

int main() {
    // Initialize
    initialization();

    // Main loop
    while (1){
        // Update characters
        update();
        // Check for EOF
        if (current_char.content == EOF) return eof_handler();
        // Handle the case we are in the comment
        if (state == CODE) {
            check_double_quotation();
            check_single_quotation();
            check_comment_opening();
        }
        // Handle the case we are out of the comment
        else if (state == COMMENT) {
            if (current_char.content == '\n') fprintf(stdout, "\n");
            check_comment_closing();
        }
        else if (state == STRING){
            fprintf(stdout, "%c", current_char.content);
            check_double_quotation();
        }
        else {
            fprintf(stdout, "%c", current_char.content);
            check_single_quotation();
        }
    }
}
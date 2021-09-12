/* Author: Mai Tung Duong
   Assignment 1
   File: sgrep.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h> /* for skeleton code */
#include <unistd.h> /* for getopt */
#include "str.h"

#define MAX_STR_LEN 1023

#define FALSE 0
#define TRUE  1

/*
 * Fill out your own functions here (If you need) 
 */

/*-------------------------------------------------------------------*/
/* PrintUsage()
   print out the usage of the Simple Grep Program                    */
/*-------------------------------------------------------------------*/
void 
PrintUsage(const char* argv0) 
{
  const static char *fmt = 
	  "Simple Grep (sgrep) Usage:\n"
	  "%s pattern [stdin]\n";

  printf(fmt, argv0);
}
/*-------------------------------------------------------------------*/
/* SearchPattern()
   Your task:
   1. Do argument validation 
   - String or file argument length is no more than 1023
   - If you encounter a command-line argument that's too long, 
   print out "Error: argument is too long"
   
   2. Read the each line from standard input (stdin)
   - If you encounter a line larger than 1023 bytes, 
   print out "Error: input line is too long" 
   - Error message should be printed out to standard error (stderr)
   
   3. Check & print out the line contains a given string 
   (search-string)
      
   Tips:
   - fgets() is an useful function to read characters from file. Note 
   that the fget() reads until newline or the end-of-file is reached. 
   - fprintf(sderr, ...) should be useful for printing out error
   message to standard error

   NOTE: If there is any problem, return FALSE; if not, return TRUE  */
/*-------------------------------------------------------------------*/


// HELPER
/* This function receives two character pointer text and pattern
   It returns 1 if the string poitned by text matches the pattern
   pointed by pattern, 0 otherwise. 
   It ignores the remaining text after the match (rigourously speaking,
   the beginning of the string matches the pattern) */
int 
match(const char *text, const char* pattern){
    // printf("I: %s %s \n", text , pattern);

    // If empty pattern, we matches (reach the end of the pattern)
    if (*pattern == '\0') return 1;

    // If *, we either ignore the * or keep the * and ignore the 
    // corresponding character in text
    if (*pattern == '*' && *text != '\n') {
        return match(text, pattern + 1) || match(text + 1, pattern);
    }

    // Normal character case: if match, we go the the next character
    if (*pattern == *text) return match(text + 1, pattern + 1);
    // If not, then they dont match
    else return 0;
}
/* This function receive a character poitner pattern,.
   It clears the beginning asterisks of pattern by returning pointer
   to the first non-asterisk character */
char *
ClearBeginningStar (const char* pattern) {
  const char* p;
  p = pattern;
  while (*p == '*')  p++; 
  return (char*) p;
}

/* This function receives two character pointer text and pattern
   It returns 1 if the string pointed by text contains some text 
   matching the pattern pointed by pattern, 0 otherwise. */
int
StrHasPattern(const char* text, const char *pattern)
{
  if (*pattern == '\0') return 1;
  if (*text == '\0')
    return 1;
  const char *p = StrFindChr(text, *pattern);
    // printf("Inter: %s\n", p);

  while ( (p != NULL) && (*p != '\0')) {
    // printf("P: %s %s \n", p , pattern);
    if (match(p, pattern)) return 1;
    p = StrFindChr(p + 1, *pattern);
  }
  return 0;
}

/* This function receives a character pointer.
   It reads the stdin line by line and prints the line that 
   has a part matching the pattern.
   It prints to stderr if the line or argument is too long
   and returns exit status (true if nothing goes wrong) */

int
SearchPattern(const char *pattern)
{
  char buf[MAX_STR_LEN + 2]; 
  int len;
  
  /* 
   *  TODO: check if pattern is too long
   */

  if (StrGetLength(pattern) > 1023) {
      fprintf(stderr, "Error: argument is too long\n");
      return FALSE;
  }

  /* Read one line at a time from stdin, and process each line */
  while (fgets(buf, sizeof(buf), stdin)) {
	  
    /* check the length of an input line */
    if ((len = StrGetLength(buf)) > MAX_STR_LEN) {
      fprintf(stderr, "Error: input line is too long\n");
      return FALSE;
    }
	
    /* TODO: fill out this function */
    // beginning asterisks does not matter
    char* CleanPattern = ClearBeginningStar(pattern);
    // print if the buffer has the pattern
    if (StrHasPattern(buf, CleanPattern))
        fprintf(stdout, "%s",  buf);

  }
   
  return TRUE;
}
/*-------------------------------------------------------------------*/
/* It recevies integer argc, array of character pointers argv
   It check and parse the argument, print the lines that match
   argv[1] and return axit status*/

int 
main(const int argc, const char *argv[]) 
{
  /* Do argument check and parsing */
  if (argc < 2) {
	  fprintf(stderr, "Error: argument parsing error\n");
	  PrintUsage(argv[0]);
	  return (EXIT_FAILURE);
  }

  return SearchPattern(argv[1]) ? EXIT_SUCCESS:EXIT_FAILURE;
}

/* Author: Mai Tung Duong
   Assignment 2
   File: str.h */

#ifndef _STR_H_
#define _STR_H_
#include <limits.h> /* for LONG_MIN, LONG_MAX */
#include <unistd.h> /* for typedef of size_t */
#include <ctype.h> /* for isdigit, isspace */

/* Part 1 */
/*  This function receives two charracter pointers pcDest and pcSrc.
    It copy the string content from from pcSrc to pcDest.
    It returns the address of the destination.*/
size_t StrGetLength(const char* pcSrc);


/* This function receives two charracter pointers pcS1 and pcS2. 
   This function compare 2 strings pointed by pcS1 and pcS2
   It returns the a negative number if pcS1 is smaller, positive number 
   if pcS1 is bigger and 0 if they are equal*/
char *StrCopy(char *pcDest, const char* pcSrc);

/* This function receives a character pointer pcHaystack 
   and an integer (represents character) c.
   This function finds and returns the position of character c
   in string pointed by pcHaystack (returns NULL if not found)*/
int StrCompare(const char* pcS1, const char* pcS2);


/* This function receives a character pointer pcHaystack 
   and an integer (represents character) c.
   This function finds and returns the position of character c
   in string pointed by pcHaystack (returns NULL if not found)*/
char *StrFindChr(const char *pcHaystack, int c);

/* This function receives two charracter pointers pcHaystack and pcNeedle. 
   This function finds and returns the pointer point to of substring Needle
   in string pointed by pcHaystack (returns NULL if not found)*/
char *StrFindStr(const char* pcHaystack, const char *pcNeedle);

/* This function receive two charracter pointers pcDest and pcSrc. 
   This function append the string at pcSrc to the end of pcDest
   (overwriting NULL character of string pointed by pcDest). 
   and return the destination */
char *StrConcat(char *pcDest, const char* pcSrc);

/* This function receives charracter pointer nptr, 
   a character pointer pointer endptr and an integer base
   It returns the long int represented by the string in base 10 
   (overflow will result in LONG_MAX (+) or LONG_MIN (-))
   If endptr is not NULL, *endpt points to the first 
   invalid character */
long int StrToLong(const char *nptr, char **endptr, int base);

/* Helper */
/* Check if string pointed by a starts with string b, ignoring the
   NULL character. */
int StartsWith(const char* a, const char *b);

/* Check if a long int x will overlfow if we append a digit
   t to it. */
int overflow (long x, int t);


#endif /* _STR_H_ */

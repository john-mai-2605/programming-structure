/* Author: Mai Tung Duong
   Assignment 2
   File: str.c */

#include <assert.h> /* to use assert() */
#include <stdio.h>
#include <stdlib.h> /* for strtol() */
#include <string.h>
#include "str.h"

/* Your task is: 
   1. Rewrite the body of "Part 1" functions - remove the current
      body that simply calls the corresponding C standard library
      function.
   2. Write appropriate comment per each function
*/

/* Part 1 */
/*-------------------------------------------------------------------*/
size_t StrGetLength(const char* pcSrc)
{
  const char *pcEnd;
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */
  pcEnd = pcSrc;
	
  while (*pcEnd) /* null character and FALSE are identical. */
    pcEnd++;

  return (size_t)(pcEnd - pcSrc);
}

/*-------------------------------------------------------------------*/
/*  This function receives two charracter pointers pcDest and pcSrc.
    It copy the string content from from pcSrc to pcDest.
    It returns the address of the destination.*/
char *StrCopy(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  assert(pcDest); /* NULL address, 0, and FALSE are identical. */
  assert(pcSrc); /* NULL address, 0, and FALSE are identical. */

  size_t i = 0;
  while(*(pcSrc + i) != '\0') {
    *(pcDest + i) = *(pcSrc + i);
    i++;
  }
  *(pcDest + i) = '\0';
  return pcDest;
}

/*-------------------------------------------------------------------*/
/* This function receives two charracter pointers pcS1 and pcS2. 
   This function compare 2 strings pointed by pcS1 and pcS2
   It returns the a negative number if pcS1 is smaller, positive number
   if pcS1 is bigger and 0 if they are equal*/
int StrCompare(const char* pcS1, const char* pcS2)
{
  /* TODO: fill this function */
  assert(pcS1);
  assert(pcS2);

  size_t i = 0;
  while (*(pcS1 + i) != '\0' && *(pcS2 + i) != '\0') {
    if (*(pcS1 + i) - *(pcS2 + i) != 0)
      return *(pcS1 + i) - *(pcS2 + i);
    i++;
  }
  return *(pcS1 + i) - *(pcS2 + i);
}
/*-------------------------------------------------------------------*/
/* This function receives a character pointer pcHaystack 
   and an integer (represents character) c.
   This function finds and returns the position of character c
   in string pointed by pcHaystack (returns NULL if not found)*/
char *StrFindChr(const char* pcHaystack, int c)
{
  /* TODO: fill this function */
  assert(pcHaystack);

  const char* p;
  p = pcHaystack;
  while (*p != '\0') {
    if (*p == (char) c)
      return (char*) p;
    p++;
  }
  if (c == '\0')
    return (char*) p;
  else
    return NULL;
}
/*-------------------------------------------------------------------*/

// HELPER //
/* Check if string pointed by a starts with string b, ignoring the
   NULL character. */
int StartsWith(const char* a, const char *b) {
    size_t i = 0;
    while( *(b + i) != '\0') {
      if (*(a + i) == *(b + i)) i++;
      else return 0;
    }
    return 1;
}

/* This function receives two charracter pointers pcHaystack and pcNeedle. 
   This function finds and returns the pointer point to of substring Needle
   in string pointed by pcHaystack (returns NULL if not found)*/
char *StrFindStr(const char* pcHaystack, const char *pcNeedle)
{
  /* TODO: fill this function */
  assert(pcHaystack);
  assert(pcNeedle);

  if (*pcNeedle == '\0')
    return (char*) pcHaystack;
  const char *p = StrFindChr(pcHaystack, *pcNeedle);

  while ( (p != NULL) && (*p != '\0')) {
    if (StartsWith(p, pcNeedle)) return (char*) p;
    p = StrFindChr(p + 1, *pcNeedle);
  }

  return NULL;
}
/*-------------------------------------------------------------------*/
/* This function receive two charracter pointers pcDest and pcSrc. 
   This function append the string at pcSrc to the end of pcDest
   (overwriting NULL character of string pointed by pcDest). 
   and return the destination */
char *StrConcat(char *pcDest, const char* pcSrc)
{
  /* TODO: fill this function */
  assert(pcSrc);
  assert(pcDest);
  char* endDest = pcDest + StrGetLength(pcDest);
  StrCopy(endDest, pcSrc);
  return pcDest;
}

/*-------------------------------------------------------------------*/
/* Helper: check if a long int x will overlfow if we append a digit
   t to it. */
int overflow (long x, int t) {
  return x > (LONG_MAX - t) / 10;
}
/* This function receives charracter pointer nptr, 
   a character pointer pointer endptr and an integer base
   It returns the long int represented by the string in base 10 
   (overflow will result in LONG_MAX (+) or LONG_MIN (-))
   If endptr is not NULL, *endpt points to the first 
   invalid character */
long int StrToLong(const char *nptr, char **endptr, int base)
{
  assert(nptr);
  /* handle only when base is 10 */
  if (base != 10) return 0;

  /* TODO: fill this function */
  char* b;
  int sign = 1;
  int oflow = 0;
  // handle intial spaces and get the sign
  b = (char*) nptr;
  while(isspace(*b) && *b != '\0') b++;
  switch (*b) {
    case '\0':
      if (endptr) *endptr = (char*) b;
      return 0;
    case '+':
      b++;
      break;
    case '-':
      sign = -1;
      b++;
      break;
    default:;
  }
  long x = 0;
  int temp;

  // get the long int represented by the string
  while (*b && isdigit(*b)) {
      temp = *b - '0';
      if (!oflow && overflow(x, temp))
        oflow = 1;
      x = 10 * x + temp;
      b++;
  }
  // handle overflow and set endptr
  if (endptr) *endptr = (char*) b;
  if (oflow && sign == 1) return LONG_MAX;
  if (oflow && sign == -1) return LONG_MIN;
  return sign*x;
}

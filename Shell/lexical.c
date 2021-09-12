#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "lexical.h"

enum {MAX_LINE_SIZE = 1024};

enum {FALSE, TRUE};


/*--------------------------------------------------------------------*/

void freeToken(void *pvItem, void *pvExtra)

/* Free token pvItem.  pvExtra is unused. */

{
   struct Token *psToken = (struct Token*)pvItem;
   free(psToken->pcValue);
   free(psToken);
}

/*--------------------------------------------------------------------*/

void printNumberToken(void *pvItem, void *pvExtra)

/* Print token pvItem to stdout iff it is a number.  pvExtra is
   unused. */

{
   struct Token *psToken = (struct Token*)pvItem;
   if (psToken->eType == TOKEN_NUMBER)
      printf("%s ", psToken->pcValue);
}

/*--------------------------------------------------------------------*/

void printWordToken(void *pvItem, void *pvExtra)

/* Print token pvItem to stdout iff it is a word.  pvExtra is
   unused. */

{
   struct Token *psToken = (struct Token*)pvItem;
   if (psToken->eType == TOKEN_WORD)
      printf("%s ", psToken->pcValue);
}

/*--------------------------------------------------------------------*/

struct Token *makeToken(enum TokenType eTokenType,
   char *pcValue)

/* Create and return a Token whose type is eTokenType and whose
   value consists of string pcValue.  Return NULL if insufficient
   memory is available.  The caller owns the Token. */

{
   struct Token *psToken;

   psToken = (struct Token*)malloc(sizeof(struct Token));
   if (psToken == NULL)
      return NULL;

   psToken->eType = eTokenType;

   psToken->pcValue = (char*)malloc(strlen(pcValue) + 1);
   if (psToken->pcValue == NULL)
   {
      free(psToken);
      return NULL;
   }

   strcpy(psToken->pcValue, pcValue);

   return psToken;
}

/*--------------------------------------------------------------------*/

int lexLine(const char *pcLine, DynArray_T oTokens)

/* Lexically analyze string pcLine.  Populate oTokens with the
   tokens that pcLine contains.  Return 1 (TRUE) if successful, or
   0 (FALSE) otherwise.  In the latter case, oTokens may contain
   tokens that were discovered before the error. The caller owns the
   tokens placed in oTokens. */

/* lexLine() uses a DFA approach.  It "reads" its characters from
   pcLine. */

{
   enum LexState {STATE_START, STATE_IN_NUMBER, STATE_IN_WORD};

   enum LexState eState = STATE_START;

   int iLineIndex = 0;
   int iValueIndex = 0;
   char c;
   char acValue[MAX_LINE_SIZE];
   struct Token *psToken;

   assert(pcLine != NULL);
   assert(oTokens != NULL);

   int inquote = 0;
   for (;;)
   {
      /* "Read" the next character from pcLine. */
      c = pcLine[iLineIndex++];

      switch (eState)
      {
         case STATE_START:
            if ((c == '\n') || (c == '\0')) {
               if (inquote) fprintf(stderr, "./ish: Could not find quote pair\n");
               return !inquote;
            }
            else if (isdigit(c))
            {
               acValue[iValueIndex++] = c;
               eState = STATE_IN_NUMBER;
            }
            else if (isalpha(c))
            {
               acValue[iValueIndex++] = c;
               eState = STATE_IN_WORD;
            }
            else if (isspace(c))
            {
                  if (!inquote) eState = STATE_START;
                  else acValue[iValueIndex++] = c;
            }
            else if (c == '\"') 
               inquote = 1 - inquote;
            else
            {
               acValue[iValueIndex++] = c;
               eState = STATE_IN_WORD;
               // fprintf(stderr, "Invalid line\n");
               // return FALSE;
            }
            break;

         case STATE_IN_NUMBER:
            if ((c == '\n') || (c == '\0'))
            {
               /* Create a NUMBER token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_NUMBER, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               if (inquote) fprintf(stderr, "./ish: Could not find quote pair\n");
               return !inquote;
            }
            else if (isdigit(c))
            {
               acValue[iValueIndex++] = c;
               eState = STATE_IN_NUMBER;
            }
            else if (isspace(c)) {
               if (!inquote) {
                  /* Create a NUMBER token. */
                  acValue[iValueIndex] = '\0';
                  psToken = makeToken(TOKEN_NUMBER, acValue);
                  if (psToken == NULL)
                  {
                     fprintf(stderr, "Cannot allocate memory\n");
                     return FALSE;
                  }
                  if (! DynArray_add(oTokens, psToken))
                  {
                     fprintf(stderr, "Cannot allocate memory\n");
                     return FALSE;
                  }
                  iValueIndex = 0;

                  eState = STATE_START;
               }
               else acValue[iValueIndex++] = c;
            }
            else if (c == '\"') 
               inquote = 1 - inquote;
            else
            {
               acValue[iValueIndex++] = c;
               // fprintf(stderr, "Invalid line\n");
               // return FALSE;
            }
            break;

         case STATE_IN_WORD:
            if ((c == '\n') || (c == '\0'))
            {
               /* Create a WORD token. */
               acValue[iValueIndex] = '\0';
               psToken = makeToken(TOKEN_WORD, acValue);
               if (psToken == NULL)
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               if (! DynArray_add(oTokens, psToken))
               {
                  fprintf(stderr, "Cannot allocate memory\n");
                  return FALSE;
               }
               iValueIndex = 0;

               if (inquote) fprintf(stderr, "./ish: Could not find quote pair\n");
               return !inquote;
            }
            else if (isalpha(c))
            {
               acValue[iValueIndex++] = c;
               eState = STATE_IN_WORD;
            }
            else if (isspace(c)) {
               if (!inquote)           
               {
                  /* Create a WORD token. */
                  acValue[iValueIndex] = '\0';
                  psToken = makeToken(TOKEN_WORD, acValue);
                  if (psToken == NULL)
                  {
                     fprintf(stderr, "Cannot allocate memory\n");
                     return FALSE;
                  }
                  if (! DynArray_add(oTokens, psToken))
                  {
                     fprintf(stderr, "Cannot allocate memory\n");
                     return FALSE;
                  }
                  iValueIndex = 0;

                  eState = STATE_START;
               }
               else acValue[iValueIndex++] = c;
            }
            else if (c == '\"')
               inquote = 1 - inquote;
            else
            {
               acValue[iValueIndex++] = c;
               // fprintf(stderr, "Invalid line\n");
               // return FALSE;
            }
            break;

         default:
            assert(FALSE);
      }
   }
}
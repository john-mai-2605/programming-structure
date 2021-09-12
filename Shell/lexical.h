/*--------------------------------------------------------------------*/

/* A Token is either a number or a word, expressed as a string. */
enum TokenType {TOKEN_NUMBER, TOKEN_WORD};

struct Token
{
   enum TokenType eType;
   /* The type of the token. */

   char *pcValue;
   /* The string which is the token's value. */
};

void freeToken(void *pvItem, void *pvExtra);
int lexLine(const char *pcLine, DynArray_T oTokens);
struct Token *makeToken(enum TokenType eTokenType,
   char *pcValue);
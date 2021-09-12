/*--------------------------------------------------------------------*/
/* ish.c                                                              */
/* Original Author: Tung-Duong Mai                                    */
/*--------------------------------------------------------------------*/

#include "dynarray.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include "sighandler.h"
#include "lexical.h"

/*--------------------------------------------------------------------*/

enum {MAX_LINE_SIZE = 1024};

enum {FALSE, TRUE};

/*--------------------------------------------------------------------*/

int main(void)

/* Read a line from stdin, and write to stdout each number and word
   that it contains.  Repeat until EOF.  Return 0 iff successful. */

{
   char acLine[MAX_LINE_SIZE];
   DynArray_T oTokens;
   int iSuccessful;

   sigset_t sSet;
   sigemptyset(&sSet);
   sigaddset(&sSet, SIGINT);
   sigaddset(&sSet, SIGQUIT);
   sigaddset(&sSet, SIGALRM);
   sigaddset(&sSet, SIGCHLD);
   sigprocmask(SIG_UNBLOCK, &sSet, NULL);
   
   signal(SIGINT, SIG_IGN);
   signal(SIGQUIT, sigquit_handler);
   signal(SIGCHLD, sigchld_handler);

   pid_t most_recent_pid = -1;
   char *homedir = getenv("HOME");
   char dir[strlen(homedir)+7];
   strcpy(dir, homedir);
   char *path = strcat(dir, "/.ishrc");
   // printf("%s\n", path);

   FILE* fd = fopen(path, "r");

   if (chdir(homedir) < 0) perror("./ish");
   if (!fd) fd = stdin;
   while (1)
   {  

      printf("%% ");
      if (fd != stdin && fgets(acLine, MAX_LINE_SIZE, fd) == NULL) fd = stdin;
      if (fd == stdin && fgets(acLine, MAX_LINE_SIZE, fd) == NULL) break;
      oTokens = DynArray_new(0);
      if (oTokens == NULL)
      {
         fprintf(stderr, "Cannot allocate memory\n");
         exit(EXIT_FAILURE);
      }

      iSuccessful = lexLine(acLine, oTokens);
      if (iSuccessful)
      { 
         int pid, status;
         int bg = 0;
         printf("%s", acLine);
         fflush(NULL);
         int argc = DynArray_getLength(oTokens);
         if (argc == 0) continue;
         struct Token *tok = DynArray_get(oTokens, argc-1);
         if (strcmp(tok->pcValue, "&") == 0) {
            bg = 1;
            argc--;
         }
         if (argc == 0) continue;
         char *argv[argc+1];
         for (int i = 0; i < argc; i++) {
            tok = DynArray_get(oTokens, i);
            argv[i] = tok->pcValue;
         }
         argv[argc] = NULL;

         char* last_arg = argv[argc-1];
         int len = strlen(last_arg);

         if (argc > 1 && last_arg[len - 1] == '&') {
            bg = 1;
            last_arg[len - 1] = '\0';
         }

         assert(argc > 0);
         assert(argv[argc] == NULL);

         if (strcmp(argv[0], "setenv") == 0) {
            switch (argc) {
               case 3:
                  if (setenv(argv[1], argv[2], 1) < 0) perror("/ish");
                  break;
               case 2:
                  if (setenv(argv[1], "", 1) < 0) perror("./ish");
                  break;
               default:
                  fprintf(stderr, "./ish: setenv takes one or two parameters\n");
            }
         }
         else if (strcmp(argv[0], "unsetenv") == 0) {
            switch (argc) {
               case 2:
                  if (unsetenv(argv[1]) < 0) perror("./ish");
                  break;
               default:
                  fprintf(stderr, "./ish: setenv takes one or two parameters\n");
            }
         }
         else if (strcmp(argv[0], "cd") == 0) {
            switch (argc) {
               char *path;
               case 1:
                  path = getenv("HOME");
                  if (chdir(path) < 0) perror("./ish");
                  break;
               case 2:
                  path = argv[1];
                  if (chdir(path) < 0) perror("./ish");
                  break;
               default:
                  fprintf(stderr, "./ish: cd takes one parameters\n");
            }
         }
         else if (strcmp(argv[0], "exit") == 0) {
            switch (argc) {
               case 1:
                  exit(0);
                  break;
               default:
                  fprintf(stderr, "./ish: exit does not take any parameters\n");
            }
         }
         else if (strcmp(argv[0], "fg") == 0) {
            switch (argc) {
               case 1:
                  if (most_recent_pid >= 0)
                     waitpid(most_recent_pid, &status, 0);
                  break;
               default:
                  fprintf(stderr, "./ish: fg does not take any parameters\n");
            }
         }
         else {
            fflush(NULL);
            pid = fork();
            if (pid == 0) {
               fflush(NULL);
               signal(SIGINT, exit_handler);
               signal(SIGQUIT, exit_handler);
               signal(SIGCHLD, exit_handler);
               execvp(argv[0], argv);
               perror("./ish");
               exit(-1);
            }
            else {
               most_recent_pid = pid;
               if (bg) continue;
               waitpid(pid, &status, 0);
            }
         }
      }

      DynArray_map(oTokens, freeToken, NULL);
      DynArray_free(oTokens);
   }
   printf("\n");
   return 0;
}
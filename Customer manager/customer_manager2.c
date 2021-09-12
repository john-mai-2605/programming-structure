/* Author: Mai Tung Duong
   Assignment 3
   File: customer_manager2.c */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/* HELPER */
enum {HASH_MULTIPLIER = 65599};
static int hash_function(const char *pcKey, int iBucketCount)

/* Return a hash code for pcKey that is between 0 and iBucketCount-1,
   inclusive. Adapted from the EE209 lecture notes. */
{
   int i;
   unsigned int uiHash = 0U;
   for (i = 0; pcKey[i] != '\0'; i++)
      uiHash = uiHash * (unsigned int)HASH_MULTIPLIER
               + (unsigned int)pcKey[i];
   return (int)(uiHash % (unsigned int)iBucketCount);
}

/*--------------------------------------------------------------------
Data structure: Hashed table
*/
#define BUCKET_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
  struct UserInfo *nextId;   // next node
  struct UserInfo *nextName; // next node  
  struct UserInfo *prevId;   // next node
  struct UserInfo *prevName; // next node
};

struct DB {
  struct UserInfo **IdToUser;
  struct UserInfo **NameToUser;
  int curBuckSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
};

/* Linked list manipulation */
void 
insertNodeId (DB_T d, struct UserInfo* new_user, int hashId) {
  new_user -> nextId = d->IdToUser[hashId];
  new_user -> prevId = NULL;

  if (d->IdToUser[hashId]) 
    d->IdToUser[hashId] -> prevId = new_user;
  d->IdToUser[hashId] = new_user;
}

void 
insertNodeName (DB_T d, struct UserInfo* new_user, int hashName) {
  new_user -> nextName = d->NameToUser[hashName];
  new_user -> prevName = NULL;

  if (d->NameToUser[hashName]) 
    d->NameToUser[hashName] -> prevName = new_user;
  
  d->NameToUser[hashName] = new_user;
}

void 
removeNodeName (DB_T d, struct UserInfo* p, int hashName) {
  if (!p->prevName && !p->nextName) {
    d->NameToUser[hashName] = p->nextName;
  }
  else if (!p->prevName && p->nextName) {
    d->NameToUser[hashName] = p->nextName;
    p->nextName->prevName = NULL;
  }
  else if (!p->nextName) {
    p->prevName->nextName = NULL;
  }
  else {
    p->prevName->nextName = p->nextName;
    p->nextName->prevName = p->prevName;
  }
}

void 
removeNodeId (DB_T d, struct UserInfo* p, int hashId) {
  if (!p->prevId && !p->nextId) {
    d->IdToUser[hashId] = p->nextId;
  }
  else if (!p->prevId && p->nextId) {
    d->IdToUser[hashId] = p->nextId;
    p->nextId->prevId = NULL;
  }
  else if (!p->nextId) {
    p->prevId->nextId = NULL;
  }
  else {
    p->prevId->nextId = p->nextId;
    p->nextId->prevId = p->prevId;
  }
}

/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
/* create and return a db structure */
DB_T
CreateCustomerDB(void)
{
  /* Uncomment and use the following implementation if you want 
  */
  DB_T d;
  
  d = (DB_T) calloc(1, sizeof(struct DB));
  if (d == NULL) {
    fprintf(stderr, "Can't allocate a memory for DB_T\n");
    return NULL;
  }
  d->curBuckSize = BUCKET_SIZE;
  d->IdToUser = (struct UserInfo **)calloc(d->curBuckSize,
               sizeof(struct UserInfo*));
  if (d->IdToUser == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curBuckSize);   
    free(d);
    return NULL;
  }

  d->NameToUser = (struct UserInfo **)calloc(d->curBuckSize,
               sizeof(struct UserInfo*));
  if (d->NameToUser == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curBuckSize);   
    free(d);
    return NULL;
  }
  d->numItems = 0;
  return d;
}
/*-------------------------------------------------------------------*/
/* destory db and its associated memory */
void
DestroyCustomerDB(DB_T d)
{
  /* fill out this function */
  if(d) {
    struct UserInfo *p;
    struct UserInfo *nextp;
    for (int b = 0; b < d->curBuckSize; b++){
      if (!d->IdToUser[b]) continue;
      for (p = d->IdToUser[b]; p != NULL; p = nextp) {
        nextp = p->nextId;
        // assert(p);
        // assert(p->id);
        // assert(p->name);
        free(p->id);
        free(p->name);
        free(p);
      }
    }
    
    // assert(d);
    // assert(d->IdToUser);
    // assert(d->NameToUser);
    free(d->NameToUser);
    free(d->IdToUser);
    free(d);
  };
}
/*-------------------------------------------------------------------*/
/* register a customer with (name, id, purchase) */
int
RegisterCustomer(DB_T d, const char *id,
		 const char *name, const int purchase)
{
  /* fill out this function */
  /* d, id and name should not be NULL */
  if (!d || !id || !name) return -1;

  /* purchase amount should be positive */
  if (purchase <= 0) return -1;

  /* Expand the table array if it reaches -.75 bucket size */
  if (d->numItems >= 0.75*d->curBuckSize && d->curBuckSize <= 500000){
      // printf("Now\n");
      // printTable(d);
    int oldSize = d->curBuckSize;
    d->curBuckSize = oldSize*2;
    struct UserInfo** temp1 = 
      realloc(d->IdToUser, (d->curBuckSize)*sizeof(struct UserInfo*));
    if (!temp1) {
      d->curBuckSize = oldSize;
      return -1;
    }
    struct UserInfo** temp2 = 
      realloc(d->NameToUser,(d->curBuckSize)*sizeof(struct UserInfo*));
    if (!temp2) {
      d->curBuckSize = oldSize;
      free(temp1);
      return -1;
    }
    d->IdToUser = temp1;
    memset(d->IdToUser+oldSize, 0, oldSize*sizeof(struct UserInfo*));
    
    d->NameToUser = temp2;
    memset(d->NameToUser+oldSize, 0, oldSize*sizeof(struct UserInfo*));

    struct UserInfo *p;
    struct UserInfo *nextp;

    for (int b = 0; b < oldSize; b++){
      if (d->IdToUser[b] == NULL) continue;
      for (p = d->IdToUser[b]; p != NULL; p = nextp) {
        nextp = p->nextId;
        int newhashId = hash_function(p->id, d->curBuckSize); 
        if (newhashId != b) {
            removeNodeId(d, p, b);
            insertNodeId(d, p, newhashId);
        }
      }
    }

    for (int b = 0; b < oldSize; b++){
      if (d->NameToUser[b] == NULL) continue;
      for (p = d->NameToUser[b]; p != NULL; p = nextp) {
        nextp = p->nextName;
        int newhashName = hash_function(p->name, d->curBuckSize); 
        if (newhashName != b) {
            removeNodeName(d, p, b);
            insertNodeName(d, p, newhashName);
        }
      }
    }
    // printf("Then\n");
    // printTable(d);
  }
 
  /* Check if we already register id or name */
  int hashId = hash_function(id, d->curBuckSize);
  int hashName = hash_function(name, d->curBuckSize);
  
  struct UserInfo *p;
  struct UserInfo *nextp;

  for (p = d->IdToUser[hashId]; p != NULL; p = nextp) {
    if (strcmp(p->id, id) == 0)
      return -1;
    nextp = p->nextId;
  }

  for (p = d->NameToUser[hashName]; p != NULL; p = nextp) {
    if (strcmp(p->name, name) == 0)
      return -1;
    nextp = p->nextName;
  }

  /* Add new user */
  struct UserInfo *new_user =  
    (struct UserInfo*)malloc(sizeof(struct UserInfo));
  new_user -> name = (char*)malloc(strlen(name) + 1);
  if (!new_user-> name) {
    fprintf(stderr, "Can't allocate a memory for string name\n");
    return -1;
  }
  strcpy(new_user -> name, name);
  new_user -> id =  (char*)malloc(strlen(id) + 1);
  if (!new_user-> id) {
    fprintf(stderr, "Can't allocate a memory for string id\n");   
    free(new_user->name);
    return -1;
  }  
  
  strcpy(new_user -> id, id);
  new_user -> purchase = purchase;
 
  insertNodeId(d, new_user, hashId);
  insertNodeName(d, new_user, hashName);
  // printTable(d);

  /* Increment number of users */
  d->numItems++;
  return 0;
}
/*-------------------------------------------------------------------*/
/* unregister a customer with 'id' */
int
UnregisterCustomerByID(DB_T d, const char *id)
{
  /* fill out this function */
  /* d and id should not be NULL */
  if (!d || !id) return -1;
  /* Find the user with id */
  struct UserInfo *p;
  struct UserInfo *nextp;
  int found;
  int hashId = hash_function(id, d->curBuckSize);
  if (d->IdToUser[hashId] == NULL) return -1;
  for (p = d->IdToUser[hashId]; p != NULL; p = nextp) {
    if (strcmp(p->id, id) == 0) {
      found = 1;
      break;
    }
    nextp = p->nextId;
  }

  if (!found) return -1;
  int hashName = hash_function(p->name, d->curBuckSize);

  removeNodeId(d, p, hashId);
  removeNodeName(d, p, hashName);

  /* Deallocate the user */
  free(p->name);
  free(p->id);
  free(p);
  d->numItems--;
  // printTable(d);
  return 0;
}

/*-------------------------------------------------------------------*/
/* unregister a customer with 'name' */
int
UnregisterCustomerByName(DB_T d, const char *name)
{
  /* fill out this function */
  /* d and name should not be NULL */
  if (!d || !name) return -1;
  /* Find the user with name */
  struct UserInfo *p;
  struct UserInfo *nextp;
  int found;
  int hashName = hash_function(name, d->curBuckSize);
  if (d->NameToUser[hashName] == NULL) return -1;
  for (p = d->NameToUser[hashName]; p != NULL; p = nextp) {
    if (strcmp(p->name, name) == 0) {
      found = 1;
      break;
    }
    nextp = p->nextName;
  }

  if (!found) return -1;
  int hashId = hash_function(p->id, d->curBuckSize);
 
  removeNodeId(d, p, hashId);
  removeNodeName(d, p, hashName);
  
  /* Deallocate the user */
  free(p->name);
  free(p->id);
  free(p);
  d->numItems--;
  // printTable(d);
  return 0;
  }
/*-------------------------------------------------------------------*/
/* get the purchase amount of a user whose ID is 'id' */
int
GetPurchaseByID(DB_T d, const char* id)
{
  /* fill out this function */
  /* d and id should not be NULL */
  if (!d || !id) return -1;

  /* Find user by id */
  struct UserInfo *p;
  struct UserInfo *nextp;
  int found;
  int hashId = hash_function(id, d->curBuckSize);
  if (d->IdToUser[hashId] == NULL) return -1;

  for (p = d->IdToUser[hashId]; p != NULL; p = nextp) {
    if (strcmp(p->id, id) == 0) {
      found = 1;
      break;
    }
    nextp = p->nextId;
  }
  if (found) assert(p);

  /* Return purchase amount */
  if (found) return p->purchase;
  else return -1;
}
/*-------------------------------------------------------------------*/
/* get the purchase amount of a user whose name is 'name' */
int
GetPurchaseByName(DB_T d, const char* name)
{
  /* fill out this function */
  /* d and name should not be NULL */
  if (!d || !name) return -1;

  /* Find user by name */
  struct UserInfo *p;
  struct UserInfo *nextp;
  int found;

  int hashName = hash_function(name, d->curBuckSize);
  if (d->NameToUser[hashName] == NULL) return -1;

  for (p = d->NameToUser[hashName]; p != NULL; p = nextp) {
    if (strcmp(p->name, name) == 0) {
      found = 1;
      break;
    }
    nextp = p->nextName;
  }
  if (found) assert(p);
  /* Return purchase amount */
  if (found) return p->purchase;
  else return -1;
}
/*-------------------------------------------------------------------*/
/* iterate all valid user items once, evaluate fp for each valid user
   and return the sum of all fp function calls */
int
GetSumCustomerPurchase(DB_T d, FUNCPTR_T fp)
{
  /* fill out this function */
  /* d and fp should not be NULL */
  if (!d || !fp) return -1;

  /* Find user by id */
  int sum = 0;
  struct UserInfo *p;
  struct UserInfo *nextp;
  for (int b = 0; b < d->curBuckSize; b++){
    for (p = d->IdToUser[b]; p != NULL; p = nextp) {
      nextp = p->nextId;
      sum += (*fp)(p->id, p->name, p->purchase);
    }
  }

  /* Return sum amount */
  return sum;
}

/* Print out the table for debugging */
void printTable(DB_T d)
{
  /* fill out this function */
  /* d and fp should not be NULL */
  assert(!d);
  /* Find user by id */
  struct UserInfo *p;
  struct UserInfo *nextp;
  for (int b = 0; b < d->curBuckSize; b++){
    if (!d->IdToUser[b]) continue;
    for (p = d->IdToUser[b]; p != NULL; p = nextp) {
      nextp = p->nextId;
      printf("%d \t %s \t %s \t %d \t %s \t %s\n", 
        b, p->id, p->name, p->purchase, 
        p->prevId ? p->prevId->id : "No prev",
        p->nextId ? p->nextId->id : "No next" );    }
  }
  printf("\n");
  for (int b = 0; b < d->curBuckSize; b++){
    if (!d->NameToUser[b]) continue;
    for (p = d->NameToUser[b]; p != NULL; p = nextp) {
      nextp = p->nextName;
      printf("%d \t %s \t %s \t %d \t %s  \t %s\n", 
        b, p->id, p->name, p->purchase, 
        p->prevName ? p->prevName->id : "No prev",
        p->nextName ? p->nextName->id : "No next" );
    }
  }
}


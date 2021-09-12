/* Author: Mai Tung Duong
   Assignment 3
   File: customer_manager1.c */
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "customer_manager.h"

/*--------------------------------------------------------------------
Uncomment and use the following code if you want
*/
#define UNIT_ARRAY_SIZE 1024

struct UserInfo {
  char *name;                // customer name
  char *id;                  // customer id
  int purchase;              // purchase amount (> 0)
};

struct DB {
  struct UserInfo *pArray;   // pointer to the array
  int curArrSize;            // current array size (max # of elements)
  int numItems;              // # of stored items, needed to determine
			     // # whether the array should be expanded
			     // # or not
};

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
  d->curArrSize = UNIT_ARRAY_SIZE; // start with 1024 elements
  d->pArray = (struct UserInfo *)calloc(d->curArrSize,
               sizeof(struct UserInfo));
  if (d->pArray == NULL) {
    fprintf(stderr, "Can't allocate a memory for array of size %d\n",
	    d->curArrSize);   
    free(d);
    return NULL;
  }
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
    struct UserInfo *p0 = d->pArray;
    for (p = p0; p - p0 < d->curArrSize; p++) {
      if (p->id){
        free(p->name);
        free(p->id);
      }
    }
    free(d->pArray);
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

  /* Check if we already register id or name */
  struct UserInfo *p;
  struct UserInfo *p0 = d->pArray;
  for (p = p0; p - p0 < d->curArrSize; p++) {
    if (p->id == NULL) continue;
    if (strcmp(p->id, id) == 0 || strcmp(p->name, name) == 0)
      return -1;
  }

  /* Expand the array if it reaches max size*/
  if (d->numItems == d->curArrSize){
    int oldSize = d->curArrSize;
    d->curArrSize += UNIT_ARRAY_SIZE;
    struct UserInfo* temp = 
      realloc(d->pArray, (d->curArrSize)*sizeof(struct UserInfo));
    if (temp == NULL) {
      d->curArrSize = oldSize;
      return -1;
    }
    else
      d->pArray = temp;
    p0 = d->pArray;
    memset(p0+oldSize, 0, UNIT_ARRAY_SIZE*sizeof(struct UserInfo));
  }

  /* Find the first "hole" to add the user*/
  for (p = p0; p - p0 < d->curArrSize; p++) {
    if (p->id == NULL)
      break;
  }

  /* Add new user */
  struct UserInfo *new_user = p;
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
  struct UserInfo *p0 = d->pArray;
  int found = 0;
  for (p = p0; p - p0 < d->curArrSize; p++) {
    if (p->id == NULL) continue;
    if (strcmp(p -> id, id) == 0) {
      found = 1;
      break;
    }
  }
  if (!found) return -1;
  /* Deallocate the user */
  free(p->name);
  free(p->id);
  memset(p, 0, sizeof(struct UserInfo));
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
  struct UserInfo *p0 = d->pArray;
  int found = 0;
  for (p = p0; p - p0 < d->curArrSize; p++) {
    if (p->id == NULL) continue;
    if (strcmp(p -> name, name) == 0) {
      found = 1;
      break;
    }
  }
  if (!found) return -1;

  /* Deallocate the user */
  free(p->name);
  free(p->id);
  memset(p, 0, sizeof(struct UserInfo));
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
  int found = 0;
  struct UserInfo *p;
  struct UserInfo *p0 = d->pArray;
  for (p = p0; p - p0 < d->curArrSize; p++) {
    if (p->id == NULL) continue;
    if (strcmp(p->id, id) == 0){
      found = 1;
      break;
    }
  }
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
  int found = 0;
  struct UserInfo *p;
  struct UserInfo *p0 = d->pArray;
  for (p = p0; p - p0 < d->curArrSize; p++) {
    if (p->id == NULL) continue;
    if (strcmp(p->name, name) == 0){
      found = 1;
      break;
    }
  }
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
  struct UserInfo *p0 = d->pArray;
  for (p = p0; p - p0 < d->curArrSize; p++) {
      if (p->id == NULL) continue;
      sum += (*fp)(p->id, p->name, p->purchase);
  }
  /* Return sum amount */
  return sum;
}

/* Print out the table for debugging */
void printTable(DB_T d){
  struct UserInfo *p;
  struct UserInfo *p0 = d->pArray;
  for (p = p0; p - p0 < d->curArrSize; p++) {
      if (p->id == NULL) continue;
      printf("%ld \t", p - p0);
      printf("%s \t %s \t %d \n", p->id, p->name, p->purchase);
  }
}
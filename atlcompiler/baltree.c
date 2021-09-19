/* baltree.c: Balanced Binary Tree routines insert_id and find_id.  */

#include "baltree.h"
#include <string.h>

#define FALSE 0
#define TRUE  1

/*  find_id returns a pointer to node in TREE that has the correct
    ID.  If there is no node in TREE with ID, NULL is returned. */

id_rec *
  find_id (tree, id)
id_rec *tree;
char   *id;
{
  int cmp_result;
  
  /* Check for an empty tree. */
  if (tree == NULL)
    return NULL;
  
  /* Recursively search the tree. */
  cmp_result = strcmp (id, tree->id);
  if (cmp_result == 0)
    return tree;  /* This is the item. */
  else if (cmp_result < 0)
    return find_id (tree->left, id);
  else
    return find_id (tree->right, id);  
}


/* insert_id inserts a NEW_ID rec into the tree whose ROOT is
   provided.  insert_id returns TRUE if the tree height from
   ROOT down is increased otherwise it returns FALSE.  This is a
   recursive balanced binary tree insertion algorithm. */

int insert_id (root, new_id)
     id_rec **root;
     id_rec *new_id;
{
  id_rec *A, *B;
  
  /* If root is NULL, this where it is to be inserted. */
  if (*root == NULL)
    {
      *root = new_id;
      new_id->left = NULL;
      new_id->right = NULL;
      new_id->balance = 0;
      return (TRUE);
    }
  
  /* We need to search for a leaf. */
  if (strcmp (new_id->id, (*root)->id) < 0)
    {
      /* Insert it on the left. */
      if (insert_id (&((*root)->left), new_id))
	{
	  /* The height increased. */
	  (*root)->balance --;
	  
	  switch ((*root)->balance)
	    {
	    case  0:  /* no height increase. */
	      return (FALSE);
	    case -1:  /* height increase. */
	      return (FALSE);
	    case -2:  /* we need to do a rebalancing act. */
	      A = *root;
	      B = (*root)->left;
	      if (B->balance <= 0)
		{
		  /* Single Rotate. */
		  A->left = B->right;
		  B->right = A;
		  *root = B;
		  A->balance = 0;
		  B->balance = 0;
		}
	      else
		{
		  /* Double Rotate. */
		  *root = B->right;
		  B->right = (*root)->left;
		  A->left = (*root)->right;
		  (*root)->left = B;
		  (*root)->right = A;
		  switch ((*root)->balance)
		    {
		    case -1:
		      A->balance = 1;
		      B->balance = 0;
		      break;
		    case  0:
		      A->balance = 0;
		      B->balance = 0;
		      break;
		    case  1:
		      A->balance = 0;
		      B->balance = -1;
		      break;
		    }
		  (*root)->balance = 0;
		}
	    }     
	} 
    }
  else
    {
      /* Insert it on the right. */
      if (insert_id (&((*root)->right), new_id))
	{
	  /* The height increased. */
	  (*root)->balance ++;
	  switch ((*root)->balance)
	    {
	    case 0:  /* no height increase. */
	      return (FALSE);
	    case 1:  /* height increase. */
	      return (FALSE);
	    case 2:  /* we need to do a rebalancing act. */
	      A = *root;
	      B = (*root)->right;
	      if (B->balance >= 0)
		{
		  /* Single Rotate. */
		  A->right = B->left;
		  B->left = A;
		  *root = B;
		  A->balance = 0;
		  B->balance = 0;
		}
	      else
		{
		  /* Double Rotate. */
		  *root = B->left;
		  B->left = (*root)->right;
		  A->right = (*root)->left;
		  (*root)->left = A;
		  (*root)->right = B;
		  switch ((*root)->balance)
		    {
		    case -1:
		      A->balance = 0;
		      B->balance = 1;
		      break;
		    case  0:
		      A->balance = 0;
		      B->balance = 0;
		      break;
		    case  1:
		      A->balance = -1;
		      B->balance = 0;
		      break;
		    }
		  (*root)->balance = 0;
		}
	    }     
	} 
    }
  
  /* If we fall through to here, the tree did not grow in height. */
  return (FALSE);
}

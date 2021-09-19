/* baltree.h:  The record definition for the balanced tree. */

/* Basic balanced binary tree. */

typedef struct id_rec {
	/* The balanced binary tree fields. */
	char  *id;      /* The name. */
	short balance;  /* For the balanced tree. */
	struct id_rec *left, *right; /* Tree pointers. */
  
	/* Other information fields. */
} id_rec;

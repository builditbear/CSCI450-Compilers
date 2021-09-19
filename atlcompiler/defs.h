/* defs.h:  Common definitions and includes for ATL/0.  */

/* The includes .... */
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include "scan.h"
#include "baltree.h"

/* Some defines to use. */
#define FALSE 0
#define TRUE  1

/* Global function prototypes. */

/* From scan.c - - */
token scanner (void);

/* From parse.c - - */
void parse (void);

/* From util.c - - */
void generate (char *fmt,...);
void label (char *lab,...);
void error (char *mesg,...);

/* From baltree.c - - */
id_rec *find_id (id_rec *tree, char *id);
int insert_id (id_rec **root, id_rec *new_id);



/* global.h:  The global variables!   For all files except main, these
	should extern variables. */

/*
   Set up the macros EXTERN and INIT.
   The main program (main.c) defines MAIN before including this file.
   All other files do not define MAIN.  These definitions cause
   "global" variables to be "extern" in all files except main.  Also,
   in main.c, the variable is initialized to the value in the INIT
   macro.  In other files, the INIT value is removed by the macro.
*/

#ifdef MAIN
  #define EXTERN
  #define INIT(val) = val
#else
  #define EXTERN extern
  #define INIT(val)
#endif


/* The name of the source file. */
EXTERN char *src_name;

/* The input and output streams. */
EXTERN FILE *src_in;
EXTERN FILE *asm_out;

/* Option switches. */
EXTERN int debug  INIT(FALSE);	 /* Are we debugging? */
EXTERN int list_src INIT(FALSE); /* Do we want a source listing? */

/* Set to true if an error was found. */
EXTERN int had_errors INIT(FALSE);

/* Variables for the Scanner.  (You may want more.) */
EXTERN int  line_no INIT(0);
// For storing the last token seen and returned by Scanner.
EXTERN char mostRecentToken[50];

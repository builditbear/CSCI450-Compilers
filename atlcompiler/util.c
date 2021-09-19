/* util.c: Utility routines for use with ATL/0. */

/* Get the common definitions and includes. */
#include "defs.h"

/* Variables declared elsewhere.... */
#include "global.h"


/* generate:  A function that writes assembly instructions to the 
   assembly file.  The parameters are very similar to printf.

     Examples:
	generate ("add");	(generates the add instruction.)
        generate ("load %d,%d", level, offset)
				(generates a load with level and offset
				 as integer data.)

*/

/* Has the current line been labeled? */
static int has_label    = FALSE;

void generate (char *opcode_fmt ,...)
{
  va_list args;

  va_start (args, opcode_fmt);
  fprintf (asm_out, "\t");
  vfprintf (asm_out, opcode_fmt, args);
  fprintf (asm_out, "\n");
  va_end (args);
  has_label = FALSE;
}

/* label() defines a label at the beginning of an assembly line. */
void label (char *lab,...)
{
  va_list args;
  char chars[128];

  if (has_label)
    fprintf (asm_out, "\n");
  va_start (args, lab);
  vsprintf (chars, lab, args);
  fprintf (asm_out, "%s: ", chars);
  va_end (args);
  has_label = TRUE;
}


/* Generate an error message.  Parameters like in printf.
   No \n is needed in the format string.  */

void error (char *mesg,...)
{
  va_list args;

  fprintf (stderr, "%s:%d:", src_name, line_no);
  va_start (args, mesg);
  vfprintf (stderr, mesg, args);
  va_end (args);
  fprintf  (stderr, "\n");
  had_errors = TRUE;
}


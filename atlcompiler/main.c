/* main.c:  This is the main program for ATL/0.

   CS 450 at WWU.  Fall 1992.
 */

/* Get the standard definitions and includes. */
#include "defs.h"

/* Variable Definitions ... */
#define MAIN
#include "global.h"


/* The main program.  */

int
main (int argc, char **argv)
{

  char ch;
  char *out_name = NULL;
  char *asm_name;
  char asm_chars[30];
  int  bad_opt   = FALSE;
  int  asm_only  = FALSE;	/* Just produce an assembly file. */

  char asm_cmd[128];

  /* Process the args. */

  ch = getopt (argc, argv, "dlo:s");
  while (ch != EOF)
    {
      switch (ch)
	{
	case 'd':  /* debug flag */
	  debug = TRUE;
	  break;
	
	case 'l':  /* list source flag */
	  list_src = TRUE;
	  break;
	
	case 'o':  /* output file name */
	  out_name = optarg;
	  break;

	case 's':  /* Just generate an assembly file. */
	  asm_only = TRUE;
	  break;

	default :
	  bad_opt = TRUE;
	}
      ch = getopt (argc, argv, "dlo:s");
    }

  /* Get the file name. */
  if (optind != argc-1 || bad_opt)
    {
      fprintf (stderr, "usage: %s [-d] [-l] [-s] [-o file] file\n", argv[0]);
      exit(1);
    }

  /* Source file name. */
  src_name = argv[optind];

  if (debug)
    fprintf (stderr, "Source file = %s\n", src_name);

  /* The assembly file name. */
  if (asm_only)
    {
      if (out_name != NULL)
	asm_name = out_name;
      else
	asm_name = "atl.hcs";
    }
  else
    {
      sprintf (asm_chars, ".atl.%d.hcs", (int) getpid());
      asm_name = asm_chars;
    }

  if (debug)
    fprintf (stderr, "Assembly file = %s\n", asm_name);

  
  /* hc executable file name, if not given in the -o file option. */
  if (out_name == NULL)
    out_name = "atl.out";	

  if (debug)
    fprintf (stderr, "Executable file = %s\n", out_name);

  /* Open the files. */
  src_in = fopen (src_name, "r");
  if (src_in == NULL)
    {
      fprintf (stderr, "%s: Could not open %s.\n", argv[0], src_name);
      exit (1);
    }
  
  asm_out = fopen (asm_name, "w");
  if (asm_out == NULL)
    {
      fprintf (stderr, "%s: Could not open %s.\n", argv[0], asm_name);
      fclose (src_in);
      exit (1);
    }
  
  /* Everything is now set-up ready to go.... Start the parse. */
  parse ();

  /* Close files. */
  fclose (src_in);
  fclose (asm_out);

  /* Call the assembler.  */
  if (!had_errors && !asm_only)
    {
      sprintf (asm_cmd, "hcas -a -x -o %s %s", out_name, asm_name);
      if (system (asm_cmd) != 0)
        {
	  fprintf (stderr, "%s: Assembler problems.\n", argv[0]);
	  had_errors = TRUE;
        }
    }

  if (!debug && !asm_only)
    unlink (asm_name);  

  /* Exit cleanly. */
  if (had_errors)
    exit (1);
  else
    exit (0);

}

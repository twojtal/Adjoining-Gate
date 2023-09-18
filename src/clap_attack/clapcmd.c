#include "base/main/main.h"
#include "clap.h"

ABC_NAMESPACE_IMPL_START

// Declarations
static int ClapAttack_ScanLeakage(Abc_Frame_t * pAbc, int argc, int **argv);

// Function Definitions
void ClapAttack_Init(Abc_Frame_t * pAbc) {
  Cmd_CommandAdd(pAbc, "Various", "scan", ClapAttack_ScanLeakage, 0);
  Cmd_CommandAdd(pAbc, "Various", "add", ClapAttack_ScanLeakage, 0);
  Cmd_CommandAdd(pAbc, "Various", "rem", ClapAttack_ScanLeakage, 0);
  Cmd_CommandAdd(pAbc, "Various", "mov", ClapAttack_ScanLeakage, 0);
}

int ClapAttack_ScanLeakage(Abc_Frame_t * pAbc, int argc, int ** argv) {
  int fVerbose;
  int c, result, alg=0, keysConsideredCutoff=7;
  char * pKey = "0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111", * pOutFile = NULL; //Hard-code the key in
  float keyElimCutoff = 0.006125;
  
  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "mclokvh")) != EOF) {
    switch (c) {
    case 'k':
      if ( globalUtilOptind >= argc )
	{
	  Abc_Print( -1, "Command line switch \"-k\" must be followed by a key string.\n" );
	  goto usage;
	}
      pKey = argv[globalUtilOptind];
      globalUtilOptind++;
      break;
    case 'o':
      if ( globalUtilOptind >= argc )
	{
	  Abc_Print( -1, "Command line switch \"-o\" must be followed by a filename string.\n" );
	  goto usage;
	}
      pOutFile = argv[globalUtilOptind];
      globalUtilOptind++;
      break;
    case 'm':
      alg ^= 1;
      break;
    case 'c':
      if ( globalUtilOptind >= argc )
	{
	  Abc_Print( -1, "Command line switch \"-c\" should be followed by an integer.\n" );
	  goto usage;
	}
      keysConsideredCutoff = atoi(argv[globalUtilOptind]);
      globalUtilOptind++;
      break;
    case 'l':
      if ( globalUtilOptind >= argc )
	{
	  Abc_Print( -1, "Command line switch \"-l\" should be followed by a float.\n" );
	  goto usage;
	}
      keyElimCutoff = atof(argv[globalUtilOptind]);
      printf("%f\n", keyElimCutoff);
      globalUtilOptind++;
      break;      
    case 'v':
      fVerbose ^= 1;
      break;
    case 'h':
      goto usage;
    default:
      goto usage;
    }
  }

  // Check if there is currently a network. If not, exit.
  if ( pAbc->pNtkCur == NULL )
    {
      fprintf( pAbc->Out, "Empty network.\n" );
      return 0;
    }
  
  // Error catch for no specified correct oracle key
  if(pKey == NULL) {
    Abc_Print( -1, "No key was specified with the -k flag. This is required. The CLAP attack cannot simulate probing the EOFM oracle without this.\n" );
    goto usage;
  }
  
  // If no output file was specified, go ahead and use a default.
  if (pOutFile == NULL) {
    pOutFile = malloc(sizeof(char) * 25);
    sprintf(pOutFile, "physical_clap_out.bench");
  }
  
  // call the main function
  result = ClapAttack_ClapAttackAbc(pAbc, pKey, pOutFile, alg, keysConsideredCutoff, keyElimCutoff);

  // print verbose information if the verbose mode is on
  if (fVerbose) {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else Abc_Print(1, "The command execution has failed.\n");
  }

  exit(0);

  return 0;

 usage:
  Abc_Print(-2, "usage: clap [-mclovh] -k <key> \n");
  Abc_Print(-2, "\t           The physical portion of the CLAP attack in ABC.\n");
  Abc_Print(-2, "\t-k <key>   : input the correct oracle key value for EOFM probing simulation \n");
  Abc_Print(-2, "\t-m         : use multi-node probing algorithm (alg. 2) for CLAP attack, omitting this command uses fixed EOFM probe algorithm (alg. 1)\n");  
  Abc_Print(-2, "\t-c <int>   : maximum number of key inputs for a node to be considered for EOFM probing [default = 7]\n");
  Abc_Print(-2, "\t-l <float> : minimum portion of keyspace that must be eliminated for a multi-node probe to be run [default = 0.006125]\n");
  Abc_Print(-2, "\t-o <str>   : set name of SAT solver output file from physical portion of CLAP attack [default = \"physical_clap_out.bench\"]\n");
  Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}

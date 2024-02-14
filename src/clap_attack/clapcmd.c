#include "base/main/main.h"
#include "clap.h"

ABC_NAMESPACE_IMPL_START

// Declarations
static int AdjoiningGate_ScanLeakage_CMD( Abc_Frame_t *pAbc, int argc, int **argv );
static int AdjoiningGate_ListNetwork_CMD( Abc_Frame_t *pAbc, int argc, int **argv );
static int AdjoiningGate_BFS_CMD( Abc_Frame_t *pAbc, int argc, int **argv );
static int AdjoiningGate_AddNode_CMD( Abc_Frame_t *pAbc, int argc, int **argv );
static int AdjoiningGate_Run_CMD( Abc_Frame_t *pAbc, int argc, int **argv );

// Function Definitions
void ClapAttack_Init(Abc_Frame_t * pAbc) {
  Cmd_CommandAdd(pAbc, "Various", "scan", AdjoiningGate_ScanLeakage_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "list", AdjoiningGate_ListNetwork_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "bfs", AdjoiningGate_BFS_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "add", AdjoiningGate_AddNode_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "run", AdjoiningGate_Run_CMD, 0);
}

int AdjoiningGate_ScanLeakage_CMD(Abc_Frame_t *pAbc, int argc, int **argv)
{
  int fVerbose, grouped = 0, listAdjOrder = 0;
  int c, result, alg = 0, keysConsideredCutoff = 7, probeResolutionSize = 1;
  char *pKey = "0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111", *pOutFile = NULL; // Hard-code the key in
  float keyElimCutoff = 0.006125;

  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "agclovrh")) != EOF)
  {
    switch (c)
    {
    case 'a':
      listAdjOrder = 1;
      break;
    case 'g':
      grouped = 1;
      break;
    case 'o':
      if (globalUtilOptind >= argc)
      {
        Abc_Print(-1, "Command line switch \"-o\" must be followed by a filename string.\n");
        goto usage;
      }
      pOutFile = argv[globalUtilOptind];
      globalUtilOptind++;
      break;
    case 'c':
      if (globalUtilOptind >= argc)
      {
        Abc_Print(-1, "Command line switch \"-c\" should be followed by an integer.\n");
        goto usage;
      }
      keysConsideredCutoff = atoi(argv[globalUtilOptind]);
      globalUtilOptind++;
      break;
    case 'l':
      if (globalUtilOptind >= argc)
      {
        Abc_Print(-1, "Command line switch \"-l\" should be followed by a float.\n");
        goto usage;
      }
      keyElimCutoff = atof(argv[globalUtilOptind]);
      printf("%f\n", keyElimCutoff);
      globalUtilOptind++;
      break;
    case 'r':
      if (globalUtilOptind >= argc)
      {
        Abc_Print(-1, "Command line switch \"-r\" should be followed by an integer.\n");
        goto usage;
      }
      probeResolutionSize = atoi(argv[globalUtilOptind]);
      if (probeResolutionSize < 1)
      {
        Abc_Print(-1,
                  "integer for switch \"-r\" must be greater "
                  "than 0.\n");
        goto usage;
      }
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
  if (pAbc->pNtkCur == NULL)
  {
    fprintf(pAbc->Out, "Empty network.\n");
    return 0;
  }

  // Error catch for no specified correct oracle key
  if (pKey == NULL)
  {
    Abc_Print(-1, "No key was specified with the -k flag. This is required. The CLAP attack cannot simulate probing the EOFM oracle without this.\n");
    goto usage;
  }

  // If no output file was specified, go ahead and use a default.
  if (pOutFile == NULL)
  {
    pOutFile = malloc(sizeof(char) * 25);
    sprintf(pOutFile, "physical_clap_out.bench");
  }

  // call the main function
  result = ClapAttack_ClapAttackAbc(pAbc, pKey, pOutFile, alg, keysConsideredCutoff, keyElimCutoff, probeResolutionSize, grouped, listAdjOrder);

  // print verbose information if the verbose mode is on
  if (fVerbose)
  {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else
      Abc_Print(1, "The command execution has failed.\n");
  }

  // exit(0);

  return 0;

  usage:
    Abc_Print(-2, "usage: scan [-clovrh] -k <key> \n");
    Abc_Print(-2, "\t           The physical portion of the CLAP attack in ABC.\n");
    Abc_Print(-2, "\t-a         : list nodes in adjacency tag order \n");
    Abc_Print(-2, "\t-g         : scan the groups based on adjacency tag (network must first be BFS grouped)\n");
    Abc_Print(-2, "\t-k <key>   : input the correct oracle key value for EOFM probing simulation \n");
    Abc_Print(-2, "\t-m         : use multi-node probing algorithm (alg. 2) for CLAP attack, omitting this command uses fixed EOFM probe algorithm (alg. 1)\n");
    Abc_Print(-2, "\t-c <int>   : maximum number of key inputs for a node to be considered for EOFM probing [default = 7]\n");
    Abc_Print(-2, "\t-l <float> : minimum portion of keyspace that must be eliminated for a multi-node probe to be run [default = 0.006125]\n");
    Abc_Print(-2, "\t-o <str>   : set name of SAT solver output file from physical portion of CLAP attack [default = \"physical_clap_out.bench\"]\n");
    Abc_Print(-2, "\t-r <int>   : set the probe resolution size [default = 1]\n");
    Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
    Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}

static int AdjoiningGate_ListNetwork_CMD( Abc_Frame_t * pAbc, int argc, int **argv )
{
  int fVerbose, aGrouping = 0;
  int c, result, keysConsideredCutoff = 7;
  
  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "avh")) != EOF) {
    switch (c) {
    case 'a':
      aGrouping = 1;
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
  
  // call the main function
  result = AdjoiningGate_ListNetwork(pAbc, aGrouping);

  // print verbose information if the verbose mode is on
  if (fVerbose)
  {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else Abc_Print(1, "The command execution has failed.\n");
  }

  //exit(0);

  return 0;

  usage:
    Abc_Print(-2, "usage: list [-vh]\n");
    Abc_Print(-2, "\t           List all the nodes in the network.\n");
    Abc_Print(-2, "\t-a         : list nodes in adjacency tag order \n");
    Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
    Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}

static int AdjoiningGate_BFS_CMD( Abc_Frame_t * pAbc, int argc, int **argv )
{
  int fVerbose;
  int c, gSize, result;
  
  // set defaults
  fVerbose = 0;
  gSize = 2;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "vhg")) != EOF)
  {
    switch (c) {
    case 'g':
      gSize = atoi(argv[globalUtilOptind]);
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

  if (gSize<1 || gSize>50)
  {
    fprintf( pAbc->Out, "Invalid group size.\n" );
    return 0;
    gSize = 2;
  }
  
  // call the main function
  result = AdjoiningGate_BFS(pAbc, gSize);

  // print verbose information if the verbose mode is on
  if (fVerbose)
  {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else Abc_Print(1, "The command execution has failed.\n");
  }

  return 0;

 usage:
  Abc_Print(-2, "usage: bfs [-vh]\n");
  Abc_Print(-2, "\t           Breadth First Search to update adjacency tags.\n");
  Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}

int AdjoiningGate_AddNode_CMD(Abc_Frame_t * pAbc, int argc, int ** argv)
{
  int fVerbose;
  int c, result, gateType;
  char * addNode = NULL;
  char * gate = NULL;

  // set defaults
  fVerbose = 0;
  gateType = 2; // Default = 2 (AND)

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "vhng")) != EOF) {
    switch (c) {
    case 'n':
      if ( globalUtilOptind >= argc )
      {
        Abc_Print( -1, "Command line switch \"-n\" must be followed by a node string.\n" );
        goto usage;
      }
      addNode = argv[globalUtilOptind];
      globalUtilOptind++;
      break;
    case 'g':
      if ( globalUtilOptind >= argc )
      {
        Abc_Print( -1, "Command line switch \"-g\" must be followed by a gate string.\n" );
        goto usage;
      }
      gate = argv[globalUtilOptind];
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

  // If no node was specified, show error
  if (addNode == NULL) {
    fprintf( pAbc->Out, "No target node specified.\n" );
    return 0;
  }

  if (gate == NULL)
  {
    fprintf( pAbc->Out, "No gate type specified. Proceeding with default (NAND).\n" );
  }
  else
  {
    if(strcmp(gate,"or") == 0)
    {
      fprintf( pAbc->Out, "Adding gate type: NOR.\n" );
      gateType = 1;
    }
    else if(strcmp(gate,"and") == 0)
    {
      fprintf( pAbc->Out, "Adding gate type: NAND.\n" );
      gateType = 2;
    }
    else if(strcmp(gate,"xor") == 0)
    {
      fprintf( pAbc->Out, "Adding gate type: NXOR.\n" );
      gateType = 3;
    }
    else
    {
      fprintf( pAbc->Out, "Not a valid gate type.\n" );
      goto usage;
    }
  }
  
  // call the main function
  result = AdjoiningGate_AddNode( pAbc, addNode , gateType);

  // print verbose information if the verbose mode is on
  if (fVerbose) {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else Abc_Print(1, "The command execution has failed.\n");
  }

  //exit(0);

  return 0;

 usage:
  Abc_Print(-2, "usage: add [-vh] -n <node> \n");
  Abc_Print(-2, "\t           Add a node to the network.\n");
  Abc_Print(-2, "\t-n <node>  : the node to be targeted \n");
  Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h         : print the command usage \n");
  Abc_Print(-2, "\t-g         : specify the gate type (or, and, xor) \n");
  return 1;
}

int AdjoiningGate_Run_CMD(Abc_Frame_t *pAbc, int argc, int **argv)
{
  int fVerbose;
  int c, result;
  int gateType = 2; //Default (NAND)
  char *gate = NULL;

  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "fh")) != EOF)
  {
    switch (c)
    {
    case 'f':
      if ( globalUtilOptind >= argc )
      {
        Abc_Print( -1, "Command line switch \"-f\" must be followed by a gate type.\n" );
        goto usage;
      }
      gate = argv[globalUtilOptind];
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
  if (pAbc->pNtkCur == NULL)
  {
    fprintf(pAbc->Out, "Empty network.\n");
    return 0;
  }

  if (gate == NULL)
  {
    fprintf( pAbc->Out, "No gate type specified. Proceeding with default (NAND).\n" );
  }
  else
  {
    if(strcmp(gate,"or") == 0)
    {
      fprintf( pAbc->Out, "Adding gate type: NOR.\n" );
      gateType = 1;
    }
    else if(strcmp(gate,"and") == 0)
    {
      fprintf( pAbc->Out, "Adding gate type: NAND.\n" );
      gateType = 2;
    }
    else if(strcmp(gate,"xor") == 0)
    {
      fprintf( pAbc->Out, "Adding gate type: NXOR.\n" );
      gateType = 3;
    }
    else
    {
      fprintf( pAbc->Out, "Not a valid gate type.\n" );
      goto usage;
    }
  }

  // call the main function
  result = AdjoiningGate_Run(pAbc, gateType);
  // print verbose information if the verbose mode is on
  if (fVerbose)
  {
    Abc_Print(1, "\nVerbose mode is on.\n");
    if (result)
      Abc_Print(1, "The command finished successfully.\n");
    else
      Abc_Print(1, "The command execution has failed.\n");
  }

  // exit(0);

  return 0;

  usage:
    Abc_Print(-2, "usage: scan [-fvh]\n");
    Abc_Print(-2, "\t           The primary adjoining gate replancement program.\n");
    Abc_Print(-2, "\t-f <int>   : force a replacement of all leaky nodes with a specified gate\n");
    Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
    Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}
#include "base/main/main.h"
#include "clap.h"

ABC_NAMESPACE_IMPL_START

// Declarations
static int AdjoiningGate_ScanLeakage_CMD(Abc_Frame_t * pAbc, int argc, int **argv);
static int AdjoiningGate_AddNode_CMD(Abc_Frame_t * pAbc, int argc, int **argv);
static int AdjoiningGate_RemoveNode_CMD(Abc_Frame_t * pAbc, int argc, int **argv);
static int AdjoiningGate_ReplaceNode_CMD(Abc_Frame_t * pAbc, int argc, int **argv);

// Function Definitions
void ClapAttack_Init(Abc_Frame_t * pAbc) {
  Cmd_CommandAdd(pAbc, "Various", "scan", AdjoiningGate_ScanLeakage_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "add", AdjoiningGate_AddNode_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "rem", AdjoiningGate_RemoveNode_CMD, 0);
  Cmd_CommandAdd(pAbc, "Various", "rep", AdjoiningGate_ReplaceNode_CMD, 0);
}

int AdjoiningGate_ScanLeakage_CMD(Abc_Frame_t * pAbc, int argc, int ** argv) {
  int fVerbose;
  int c, result, alg=0, keysConsideredCutoff=7;
  char * pKey = "0000010010001101111101010111101000000010010011110010010110111110011110000010100110110111", * pOutFile = NULL; //Hard-code the key in
  float keyElimCutoff = 0.006125;
  
  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "mclovh")) != EOF) {
    switch (c) {
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

  //exit(0);

  return 0;

 usage:
  Abc_Print(-2, "usage: scan [-mclovh] -k <key> \n");
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

//Abc_NtkCreateNode( Abc_Ntk_t * pNtk )
//Abc_NtkCreateObj( Abc_Ntk_t * pNtk, Abc_ObjType_t Type );
//Abc_NtkCreateNodeInv( Abc_Ntk_t * pNtk, Abc_Obj_t * pFanin );
//Abc_NtkCreateNodeBuf( Abc_Ntk_t * pNtk, Abc_Obj_t * pFanin );
//Abc_NtkCreateNodeAnd( Abc_Ntk_t * pNtk, Vec_Ptr_t * vFanins );
//Abc_NtkCreateNodeOr( Abc_Ntk_t * pNtk, Vec_Ptr_t * vFanins );
//Abc_NtkCreateNodeExor( Abc_Ntk_t * pNtk, Vec_Ptr_t * vFanins );
//Abc_NtkCreateNodeMux( Abc_Ntk_t * pNtk, Abc_Obj_t * pNodeC, Abc_Obj_t * pNode1, Abc_Obj_t * pNode0 );
int AdjoiningGate_AddNode_CMD(Abc_Frame_t * pAbc, int argc, int ** argv)
{
  int fVerbose;
  int c, result;
  char * addNode = NULL;

  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "vhn")) != EOF) {
    switch (c) {
    case 'n':
      if ( globalUtilOptind >= argc )
      {
        Abc_Print( -1, "Command line switch \"-n\" must be followed by a node string.\n" );
        goto usage;
      }
      addNode = argv[globalUtilOptind];
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
  result = AdjoiningGate_AddNode( pAbc );

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
  Abc_Print(-2, "\t-n <node>  : input the node to be deleted \n");
  Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}

int AdjoiningGate_RemoveNode_CMD(Abc_Frame_t * pAbc, int argc, int ** argv)
{
  int fVerbose;
  int c, result;
  char * delNode = NULL;
  
  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "vhn")) != EOF) {
    switch (c) {
    case 'n':
      if ( globalUtilOptind >= argc )
      {
        Abc_Print( -1, "Command line switch \"-n\" must be followed by a node string.\n" );
        goto usage;
      }
      delNode = argv[globalUtilOptind];
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
  if (delNode == NULL) {
    fprintf( pAbc->Out, "No node specified for deletion.\n" );
    return 0;
  }
  
  // call the main function
  result = AdjoiningGate_RemoveNode(pAbc, delNode);

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
  Abc_Print(-2, "usage: del [-vh] -n <node> \n");
  Abc_Print(-2, "\t           Delete a node in the network.\n");
  Abc_Print(-2, "\t-n <node>  : input the node to be deleted \n");
  Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}

//Abc_ObjReplace( Abc_Obj_t * pObjOld, Abc_Obj_t * pObjNew );
int AdjoiningGate_ReplaceNode_CMD(Abc_Frame_t * pAbc, int argc, int ** argv)
{
  int fVerbose;
  int c, result;
  char * repNode = NULL;
  
  // set defaults
  fVerbose = 0;

  // get arguments
  Extra_UtilGetoptReset();
  while ((c = Extra_UtilGetopt(argc, argv, "vhn")) != EOF) {
    switch (c) {
    case 'n':
      if ( globalUtilOptind >= argc )
      {
        Abc_Print( -1, "Command line switch \"-n\" must be followed by a node string.\n" );
        goto usage;
      }
      repNode = argv[globalUtilOptind];
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
  if (repNode == NULL) {
    fprintf( pAbc->Out, "No node specified for replacement.\n" );
    return 0;
  }
  
  // call the main function
  result = AdjoiningGate_ReplaceNode( pAbc, repNode );

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
  Abc_Print(-2, "usage: rep [-vh] -n <node> \n");
  Abc_Print(-2, "\t           Replace a node in the network.\n");
  Abc_Print(-2, "\t-n <node>  : input the node to be replaced \n");
  Abc_Print(-2, "\t-v         : toggle printing verbose information [default = %s]\n", fVerbose ? "yes" : "no");
  Abc_Print(-2, "\t-h         : print the command usage \n");
  return 1;
}
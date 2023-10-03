#ifndef CLAPATTACK_h
#define CLAPATTACK_h

// Includes
#include "base/main/main.h"
#include "base/abc/abc.h"
#include "base/main/mainInt.h"

// Parameters
ABC_NAMESPACE_HEADER_START

// Basic Types

// Function Declarations
extern int ClapAttack_ClapAttackAbc(Abc_Frame_t *pAbc, char *pKey, char *pOutFile, int alg, int keysConsideredCutoff, float keyElimCutoff, int probeResolutionSize, int grouped, int listAdjOrder);
int AdjoiningGate_ListNetwork( Abc_Frame_t *pAbc , int adjGrouping);
int AdjoiningGate_BFS( Abc_Frame_t *pAbc, int group_size);
int AdjoiningGate_AddNode( Abc_Frame_t *pAbc, char *targetNode );
int AdjoiningGate_RemoveNode( Abc_Frame_t *pAbc, char *delNode );
int AdjoiningGate_ReplaceNode( Abc_Frame_t *pAbc, char *repNode );

#endif

ABC_NAMESPACE_HEADER_END

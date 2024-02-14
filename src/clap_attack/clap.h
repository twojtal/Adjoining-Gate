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
int AdjoiningGate_ListNetwork(Abc_Frame_t *pAbc, int aGrouping);
int AdjoiningGate_BFS(Abc_Frame_t *pAbc, int group_size);
int AdjoiningGate_AddNode(Abc_Frame_t *pAbc, char *targetNode, int gateType);
int AdjoiningGate_Run(Abc_Frame_t *pAbc, int gateType);

#endif

ABC_NAMESPACE_HEADER_END

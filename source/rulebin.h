   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/16/14            */
   /*                                                     */
   /*           DEFRULE BSAVE/BLOAD HEADER FILE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for the  */
/*    defrule construct.                                     */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Removed CONFLICT_RESOLUTION_STRATEGIES,        */
/*            DYNAMIC_SALIENCE, and LOGICAL_DEPENDENCIES     */
/*            compilation flags.                             */
/*                                                           */
/*      6.30: Changed integer type/precision.                */
/*                                                           */
/*            Added support for alpha memories.              */
/*                                                           */
/*            Added salience groups to improve performance   */
/*            with large numbers of activations of different */
/*            saliences.                                     */
/*                                                           */
/*************************************************************/

#if (! RUN_TIME)
#ifndef _H_rulebin

#define _H_rulebin

#include "modulbin.h"
#include "cstrcbin.h"
#ifndef _H_network
#include "network.h"
#endif

#if FUZZY_DEFTEMPLATES
/* These structs are added at the end of rule structs
   that have patterns that have FUZZY slot references.
   They record for each fuzzy slot the pattern number
   in the rule and the slot number within the pattern
   as well as the ptr to that fuzzy values hash node.
*/
struct bsaveFzSlotLocator
  {
    unsigned int patternNum;
    unsigned int slotNum : 13;
    long     fvhnPtr;
  };
#endif

struct bsaveDefrule
  {
   struct bsaveConstructHeader header;
   int salience;
   int localVarCnt;
   unsigned int complexity      : 12;
   unsigned int autoFocus       :  1;
   long dynamicSalience;
   long actions;
   long logicalJoin;
   long lastJoin;
   long disjunct;
#if CERTAINTY_FACTORS
   double CF;
   long  dynamicCF;
#endif
#if FUZZY_DEFTEMPLATES
   double       min_of_maxmins;
   unsigned int lhsRuleType;
   unsigned int numberOfFuzzySlots;
   long         pattern_fv_arrayPtr;
#endif
  };

struct bsavePatternNodeHeader
  {
   long entryJoin;
   long rightHash;
   unsigned int singlefieldNode : 1;
   unsigned int multifieldNode : 1;
   unsigned int stopNode : 1;
   unsigned int blocked : 1;
   unsigned int initialize : 1;
   unsigned int marked : 1;
   unsigned int beginSlot : 1;
   unsigned int endSlot : 1;
   unsigned int selector : 1;
  };

struct bsaveDefruleModule
  {
   struct bsaveDefmoduleItemHeader header;
  };

struct bsaveJoinLink
  {
   char enterDirection;
   long join;
   long next;
  };
  
struct bsaveJoinNode
  {
   unsigned int firstJoin : 1;
   unsigned int logicalJoin : 1;
   unsigned int joinFromTheRight : 1;
   unsigned int patternIsNegated : 1;
   unsigned int patternIsExists : 1;
   unsigned int rhsType : 3;
   unsigned int depth : 7;
   long networkTest;
   long secondaryNetworkTest;
   long leftHash;
   long rightHash;
   long rightSideEntryStructure;
   long nextLinks;
   long lastLevel;
   long rightMatchNode;
   long ruleToActivate;
  };
  
#define RULEBIN_DATA 20

struct defruleBinaryData
  { 
   long NumberOfDefruleModules;
   long NumberOfDefrules;
   long NumberOfJoins;
   long NumberOfLinks;
   long RightPrimeIndex;
   long LeftPrimeIndex; 
   struct defruleModule *ModuleArray;
   struct defrule *DefruleArray;
   struct joinNode *JoinArray;
   struct joinLink *LinkArray;
#if FUZZY_DEFTEMPLATES 
   long NumberOfPatternFuzzyValues;
   struct fzSlotLocator *PatternFuzzyValueArray;
#endif
  };

#define DefruleBinaryData(theEnv) ((struct defruleBinaryData *) GetEnvironmentData(theEnv,RULEBIN_DATA))

#define BloadDefrulePointer(x,i) ((struct defrule *) ((i == -1L) ? NULL : &x[i]))
#define BsaveJoinIndex(joinPtr) ((joinPtr == NULL) ? -1L :  ((struct joinNode *) joinPtr)->bsaveID)
#define BloadJoinPointer(i) ((struct joinNode *) ((i == -1L) ? NULL : &DefruleBinaryData(theEnv)->JoinArray[i]))
#define BsaveJoinLinkIndex(linkPtr) ((linkPtr == NULL) ? -1L :  ((struct joinLink *) linkPtr)->bsaveID)
#define BloadJoinLinkPointer(i) ((struct joinLink *) ((i == -1L) ? NULL : &DefruleBinaryData(theEnv)->LinkArray[i]))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _RULEBIN_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           DefruleBinarySetup(void *);
   LOCALE void                           UpdatePatternNodeHeader(void *,struct patternNodeHeader *,
                                                                 struct bsavePatternNodeHeader *);
   LOCALE void                           AssignBsavePatternHeaderValues(void *,struct bsavePatternNodeHeader *,
                                                                        struct patternNodeHeader *);
   LOCALE void                          *BloadDefruleModuleReference(void *,int);

#endif /* _H_rulebin */ 

#endif /* (! RUN_TIME) */





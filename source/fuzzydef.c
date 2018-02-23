   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             FUZZY REASONING MODULE                  */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Donnell                                     */
/*      Bob Orchard (NRCC - Nat'l Research Council of Canada)*/
/*                  (Fuzzy reasoning extensions)             */
/*                  (certainty factors for facts and rules)  */
/*                  (extensions to run command)              */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/


/******************************************************************
    Fuzzy Logic Module

    This file contains the following categories of functions:
    
    initialization of fuzzy reasoning functions, structures, etc.
        adds function parsers and fuzzy commands
    parsing of deffuzzy statement
    maintaining and accessing linguistic variables

 ******************************************************************/
 
#define _FUZZYDEF_SOURCE_


#include "setup.h"


#if FUZZY_DEFTEMPLATES
 
#include "fuzzydef.h"
#include "fuzzycom.h"
#include "scanner.h"
#include "fuzzyval.h"
#include "fuzzylv.h"
#include "fuzzypsr.h"
#include "fuzzymod.h"

#include "memalloc.h"

#include <stdio.h>
#define _CLIPS_STDIO_




/******************************************************************
    Global Internal Function Declarations
    
    Defined in fuzzydef.h
 ******************************************************************/



/******************************************************************
    Local Internal Function Declarations
 ******************************************************************/
 static void DeallocateFuzzyData(void *theEnv);


/******************************************************************
    Local Internal Variable Declarations
 ******************************************************************/

   
  
/******************************************************************
    Global External Variable Declarations
 ******************************************************************/



/******************************************************************
    Global Internal Variable Declarations
 ******************************************************************/


/******************************************************************
    FUNCTIONS THAT INITIALIZE FUZZY LOGIC CONSTRUCT 
******************************************************************/


globle void InitializeFuzzy(
  void *theEnv)
{
  AllocateEnvironmentData(theEnv,FUZZY_DATA,sizeof(struct fuzzyData),DeallocateFuzzyData);
  FuzzyData(theEnv)->FuzzyInferenceType = MAXMIN;  /* inference type 'max-min' by default */
  FuzzyData(theEnv)->FuzzyFloatPrecision = 4;      /* default precision for printing fuzzy set values */
  FuzzyData(theEnv)->FuzzyAlphaValue = 0.0;        /* default alpha cut for pattern matching */
  FuzzyData(theEnv)->is_last_defuzzify_valid = TRUE;
  DeffuzzyCommands(theEnv);       /* in fuzzycom.c */
  Init_S_Z_PI_yvalues(theEnv);    /* in fuzzypsr.c */
  initFuzzyModifierList(theEnv);  /* in fuzzymod.c */

}

/************************************************/
/* DeallocateFuzzyData: Deallocates environment */
/*    data for symbols.                         */
/************************************************/
static void DeallocateFuzzyData(
  void *theEnv)
  {
   struct modifierListItem *tmpPtr, *nextPtr;
   
   tmpPtr = FuzzyData(theEnv)->ListOfModifierFunctions;
   while (tmpPtr != NULL)
     {
      nextPtr = tmpPtr->next;
      rm(theEnv,tmpPtr->name,strlen(tmpPtr->name) + 1);
      rtn_struct(theEnv,modifierListItem,tmpPtr);
      tmpPtr = nextPtr;
     }
  }
  
#endif /* FUZZY_DEFTEMPLATES */

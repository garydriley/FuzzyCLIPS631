   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             FUZZY REASONING HEADER FILE             */
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



#ifndef _H_fuzzydef
#define _H_fuzzydef

#ifndef _H_fuzzymod
#include "fuzzymod.h"
#endif

#define FUZZY_DATA USER_ENVIRONMENT_DATA + 1

/* must be an odd number -- best not to be too big or too small
   Keep in range 5 to 13 -- 9 has been used most of the time
*/
#define ArraySIZE 9

struct fuzzyData
  { 
   int FuzzyInferenceType;
   int FuzzyFloatPrecision;
   double FuzzyAlphaValue;
   /* ALL defuzzify functions must set this variable before exiting. 
      They must set it to TRUE if the defuzzify is valid or
      to FALSE if it is returning a default value when the
      defuzzificztion is undefined (e.g. moment-defuzzify being done
      for a fuzzy set that has zero area)
   */
   intBool is_last_defuzzify_valid;
   /* saveFactsInProgress is TRUE temporarily during save-facts command */
   int saveFactsInProgress;

   struct modifierListItem *ListOfModifierFunctions;
   double S_array[ArraySIZE];
   double Z_array[ArraySIZE];
   double PI_array[ArraySIZE];

  };
  
#define FuzzyData(theEnv) ((struct fuzzyData *) GetEnvironmentData(theEnv,FUZZY_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FUZZYDEF_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializeFuzzy(void *theEnv);


#ifndef _FUZZYDEF_SOURCE_
    extern int            FuzzyInferenceType;
    extern int            FuzzyFloatPrecision;
    extern double         FuzzyAlphaValue;
#endif



#endif

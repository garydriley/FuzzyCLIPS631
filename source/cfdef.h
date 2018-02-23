   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             CERTAINTY FACTOR HEADER FILE            */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*      Brian L. Donnell                                     */
/*      Bob Orchard (NRCC - Nat'l Research Council of Canada)*/
/*	                (Fuzzy reasoning extensions)             */
/*	                (certainty factors for facts and rules)  */
/*	                (extensions to run command)              */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*************************************************************/


#ifndef _H_cfdef
#define _H_cfdef


#ifndef _H_factmngr
#include "factmngr.h"
#endif

#ifndef _H_fuzzyval
#include "fuzzyval.h"
#endif

#define CF_DATA USER_ENVIRONMENT_DATA

struct certaintyFactorData
  { 
   double Threshold_CF;
   int rule_cf_calculation;
  };
  
#define CertaintyFactorData(theEnv) ((struct certaintyFactorData *) GetEnvironmentData(theEnv,CF_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _CFDEF_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE double                possibility(void *theEnv, struct fuzzy_value *f1, struct fuzzy_value *f2 );
   LOCALE double                necessity(void *theEnv, struct fuzzy_value *f1, struct fuzzy_value *f2 );
   LOCALE double                similarity(void *theEnv, struct fuzzy_value *f1, struct fuzzy_value *f2 );
   LOCALE void                  InitializeCF(void *theEnv);
   LOCALE struct expr           *ParseDeclareUncertainty(void *theEnv,char *readSource,
                                                        char *ruleName,
                                                        int *error,
                                                        double *cfVALUE);
   LOCALE double                computeStdConclCF(void *theEnv,double theRuleCF,
                                                  struct partialMatch *binds);
#if FUZZY_DEFTEMPLATES
   LOCALE double                computeFuzzyCrispConclCF(void *theEnv,struct defrule *theRule,
                                                  struct partialMatch *binds);
#endif
   LOCALE void                  changeCFofNewFact(void *theEnv,struct fact *newfact);
   LOCALE void                  changeCFofExistingFact(void *theEnv,struct fact *fact1,struct fact *fact2);
   LOCALE void                  changeCFofNewVsExistingFact(struct fact *fact1,struct fact *fact2);
   LOCALE void                  cfInformationError(void *,char *);
   LOCALE void                  cfRangeError(void *theEnv);
   LOCALE void                  cfNonNumberError(void *theEnv);
   LOCALE void                  printCF(void *theEnv,char *logicalName, double cf);
   LOCALE double                getcf(void *theEnv);
   LOCALE void                  set_threshold(void *theEnv);
   LOCALE double                get_threshold(void *theEnv);
   LOCALE void                  enable_rule_cf_calculation(void *theEnv);
   LOCALE void                  disable_rule_cf_calculation(void *theEnv);

#endif



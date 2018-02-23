   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             FUZZY UTILITY HEADER                    */
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



#ifndef _H_fuzzyutl
#define _H_fuzzyutl


#ifndef _H_factmngr
#include "factmngr.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FUZZYUTL_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                 fcompliment( struct fuzzy_value *fv );
   LOCALE struct fuzzy_value  *funion(void *theEnv, struct fuzzy_value *f1, struct fuzzy_value *f2 );
   LOCALE int                  nonintersectiontest( double *Ax, double *Ay, 
                                                    double *Bx, double *By,
                                                    int Asize, int Bsize );
   LOCALE void                 computeFuzzyConsequence(void *theEnv, struct fact *new_fact );
   LOCALE void                 changeValueOfFuzzySlots(void *theEnv, struct fact *fact1, struct fact *fact2 );
   LOCALE void                 PrintFuzzyTemplateFact(void *theEnv,char *logName, struct fuzzy_value *fv
#if CERTAINTY_FACTORS
                                                     ,double CF
#endif
                                                     );
   LOCALE void                 PrintFuzzySet(void *theEnv,char *logName, struct fuzzy_value *fv);
   LOCALE double               maxmin_intersect(void *theEnv,struct fuzzy_value *f1,
                                                struct fuzzy_value *f2, 
                                                int DoIntersect,
                                                struct fuzzy_value **intersectSet);
   LOCALE struct fuzzy_value  *fintersect(void *theEnv,struct fuzzy_value *f1, struct fuzzy_value *f2);
   LOCALE double               max_of_min(void *theEnv,struct fuzzy_value *f1, struct fuzzy_value *f2);
   LOCALE int                  FZ_EQUAL(double f1, double f2);


#endif


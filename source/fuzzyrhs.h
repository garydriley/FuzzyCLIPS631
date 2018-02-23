   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             FUZZY RHS PARSE HEADER FILE             */
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



#ifndef _H_fuzzyrhs
#define _H_fuzzyrhs


#ifndef _H_tmpltdef
#include "tmpltdef.h"
#endif

#ifndef _H_fuzzymod
#include "fuzzymod.h"
#endif

#ifndef _H_fuzzylv
#include "fuzzylv.h"
#endif


#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FUZZYRHS_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif


   LOCALE struct expr        *ParseAssertFuzzyFact(void *theEnv,char *readSource, struct token *tempToken,
                                              int *error, int endType, int constantsOnly,
                                              struct deftemplate *theDeftemplate,
                                              int variablesAllowed);
   LOCALE struct fuzzy_value *ParseLinguisticExpr(void *theEnv,char *readSource,
                                              struct token *tempToken,
                                              struct fuzzyLv *lvp,
                                              int *error);
   LOCALE struct fuzzy_value *CopyFuzzyValue(void *theEnv,struct fuzzy_value *fv);
   LOCALE void                CompactFuzzyValue(void *theEnv,struct fuzzy_value *fv);
   LOCALE struct fuzzy_value *getConstantFuzzyValue(void *theEnv,struct expr *top, int *error);
   LOCALE void                ModifyFuzzyValue(void *theEnv,struct modifierListItem *mptr,
                                               struct fuzzy_value *elem);
   LOCALE double             *FgetArray ( void *theEnv, int length );
   LOCALE void                FrtnArray ( void *theEnv, double *p, int length );
   LOCALE int                *IgetArray ( void *theEnv, int length );
   LOCALE void                IrtnArray ( void *theEnv, int *p, int length );
   LOCALE struct expr        *tokenToFloatExpression(void *theEnv,char *readSource,
                                                struct token *tempToken,
                                                int  *error,
                                                int constantsOnly);

#endif


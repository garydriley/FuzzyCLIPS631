   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             DEFFUZZY PARSER HEADER FILE             */
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

#ifndef _H_fuzzypsr
#define _H_fuzzypsr

#ifndef _H_tmpltdef
#include "tmpltdef.h"
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FUZZYPSR_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE struct fuzzyLv        *ParseFuzzyTemplate(void *theEnv,char *readSource, 
                                                   struct token *inputToken,
                                                   int *DeftemplateError);
   LOCALE void                  RtnFuzzyTemplate(struct fuzzyLv *lv);
   LOCALE void                  rtnFuzzyValue(void *theEnv,struct fuzzy_value *fv);
   LOCALE void                  InstallFuzzyValue(void *fv);
   LOCALE void                  DeinstallFuzzyValue(void *theEnv,void *fv);
   LOCALE void                  InstallFuzzyTemplate(struct deftemplate *theDeftemplate);
   LOCALE void                  DeinstallFuzzyTemplate(void *theEnv,struct fuzzyLv *fzTemplate);
   LOCALE double                sFunction(double x, double alfa, double beta, double gamma);
   LOCALE void                  Init_S_Z_PI_yvalues(void *theEnv);
   LOCALE struct fuzzy_value    *Get_S_Z_or_PI_FuzzyValue(void *theEnv,double alfa, double beta, 
                                                          double gamma, int function_type);

#endif


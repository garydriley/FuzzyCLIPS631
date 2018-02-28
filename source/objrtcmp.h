   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*               CLIPS Version 6.31  02/03/18          */
   /*                                                     */
   /*                                                     */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Converted INSTANCE_PATTERN_MATCHING to         */
/*            DEFRULE_CONSTRUCT.                             */
/*                                                           */
/*            Added environment parameter to GenClose.       */
/*                                                           */
/*      6.30: Added support for path name argument to        */
/*            constructs-to-c.                               */
/*                                                           */
/*            Added support for hashed comparisons to        */
/*            constants.                                     */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*      6.31: Optimization for marking relevant alpha nodes  */
/*            in the object pattern network.                 */
/*                                                           */
/*************************************************************/

#ifndef _H_objrtcmp
#define _H_objrtcmp

#if DEFRULE_CONSTRUCT && OBJECT_SYSTEM && (! RUN_TIME) && CONSTRUCT_COMPILER

#ifndef _STDIO_INCLUDED_
#include <stdio.h>
#define _STDIO_INCLUDED_
#endif

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _OBJRTCMP_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                    ObjectPatternsCompilerSetup(void *);
   LOCALE void                    ObjectPatternNodeReference(void *,void *,FILE *,int,int);
   LOCALE void                    ClassAlphaLinkReference(void *,void *,FILE *,int,int);
   
#endif /* DEFRULE_CONSTRUCT && OBJECT_SYSTEM && (! RUN_TIME) && CONSTRUCT_COMPILER */

#endif /* _H_objrtcmp */




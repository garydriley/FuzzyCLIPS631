   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.31  01/29/18            */
   /*                                                     */
   /*              PRINT UTILITY HEADER FILE              */
   /*******************************************************/

/*************************************************************/
/* Purpose: Utility routines for printing various items      */
/*   and messages.                                           */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Link error occurs for the SlotExistError       */
/*            function when OBJECT_SYSTEM is set to 0 in     */
/*            setup.h. DR0865                                */
/*                                                           */
/*            Added DataObjectToString function.             */
/*                                                           */
/*            Added SlotExistError function.                 */
/*                                                           */
/*      6.30: Support for long long integers.                */
/*                                                           */
/*            Support for DATA_OBJECT_ARRAY primitive.       */
/*                                                           */
/*            Support for typed EXTERNAL_ADDRESS.            */
/*                                                           */
/*            Used gensprintf and genstrcat instead of       */
/*            sprintf and strcat.                            */
/*                                                           */
/*            Changed integer type/precision.                */
/*                                                           */
/*            Added code for capturing errors/warnings.      */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*            Fixed linkage issue when BLOAD_ONLY compiler   */
/*            flag is set to 1.                              */
/*                                                           */
/*      6.31: Added additional error messages for retracted  */
/*            facts, deleted instances, and invalid slots.   */
/*                                                           */
/*            Added under/overflow error message.            */
/*                                                           */
/*************************************************************/

#ifndef _H_prntutil
#define _H_prntutil

#ifndef _H_moduldef
#include "moduldef.h"
#endif

#ifndef _STDIO_INCLUDED_
#define _STDIO_INCLUDED_
#include <stdio.h>
#endif

#define PRINT_UTILITY_DATA 53

struct printUtilityData
  { 
   intBool PreserveEscapedCharacters;
   intBool AddressesToStrings;
   intBool InstanceAddressesToNames;
  };

#define PrintUtilityData(theEnv) ((struct printUtilityData *) GetEnvironmentData(theEnv,PRINT_UTILITY_DATA))

#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _PRNTUTIL_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

   LOCALE void                           InitializePrintUtilityData(void *);
   LOCALE void                           PrintInChunks(void *,const char *,const char *);
   LOCALE void                           PrintFloat(void *,const char *,double);
   LOCALE void                           PrintLongInteger(void *,const char *,long long);
   LOCALE void                           PrintAtom(void *,const char *,int,void *);
   LOCALE void                           PrintTally(void *,const char *,long long,const char *,const char *);
   LOCALE const char                    *FloatToString(void *,double);
   LOCALE const char                    *LongIntegerToString(void *,long long);
   LOCALE const char                    *DataObjectToString(void *,DATA_OBJECT *);
   LOCALE void                           SyntaxErrorMessage(void *,const char *);
   LOCALE void                           SystemError(void *,const char *,int);
   LOCALE void                           PrintErrorID(void *,const char *,int,int);
   LOCALE void                           PrintWarningID(void *,const char *,int,int);
   LOCALE void                           CantFindItemErrorMessage(void *,const char *,const char *);
   LOCALE void                           CantDeleteItemErrorMessage(void *,const char *,const char *);
   LOCALE void                           AlreadyParsedErrorMessage(void *,const char *,const char *);
   LOCALE void                           LocalVariableErrorMessage(void *,const char *);
   LOCALE void                           DivideByZeroErrorMessage(void *,const char *);
   LOCALE void                           SalienceInformationError(void *,const char *,const char *);
   LOCALE void                           SalienceRangeError(void *,int,int);
   LOCALE void                           SalienceNonIntegerError(void *);
   LOCALE void                           CantFindItemInFunctionErrorMessage(void *,const char *,const char *,const char *);
   LOCALE void                           SlotExistError(void *,const char *,const char *);
   LOCALE void                           FactRetractedErrorMessage(void *,void *);
   LOCALE void                           FactVarSlotErrorMessage1(void *,void *,const char *);
   LOCALE void                           FactVarSlotErrorMessage2(void *,void *,const char *);
   LOCALE void                           InvalidVarSlotErrorMessage(void *,const char *);
   LOCALE void                           InstanceVarSlotErrorMessage1(void *,void *,const char *);
   LOCALE void                           InstanceVarSlotErrorMessage2(void *,void *,const char *);
   LOCALE void                           ArgumentOverUnderflowErrorMessage(void *,const char *);

#endif /* _H_prntutil */







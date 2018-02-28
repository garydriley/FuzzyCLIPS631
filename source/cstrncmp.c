   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/16/14            */
   /*                                                     */
   /*          CONSTRAINT CONSTRUCTS-TO-C MODULE          */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the constructs-to-c feature for       */
/*    constraint records.                                    */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.24: Added allowed-classes slot facet.              */
/*                                                           */
/*            Added environment parameter to GenClose.       */
/*                                                           */
/*      6.30: Added support for path name argument to        */
/*            constructs-to-c.                               */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#define _CSTRNCMP_SOURCE_

#include "setup.h"

#if CONSTRUCT_COMPILER && (! RUN_TIME)

#include "constant.h"

#include "conscomp.h"
#include "envrnmnt.h"
#include "memalloc.h"
#include "router.h"
#include "sysdep.h"

#include "cstrncmp.h"

/***********************************************/
/* ConstraintsToCode: Produces the constraint  */
/*   record code for a run-time module created */
/*   using the constructs-to-c function.       */
/***********************************************/
globle int ConstraintsToCode(
  void *theEnv,
  const char *fileName,
  const char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices)
  {
   int i, j, count;
   int newHeader = TRUE;
   FILE *fp;
   int version = 1;
   int arrayVersion = 1;
   unsigned short numberOfConstraints = 0;
   CONSTRAINT_RECORD *tmpPtr;
#if FUZZY_DEFTEMPLATES
   unsigned long int numberOfFuzzyValueConstraints = 0;
   int saveOnlyFuzzyValueConstraints = FALSE;
#endif

   /*===============================================*/
   /* Count the total number of constraint records. */
   /*===============================================*/

   for (i = 0 ; i < SIZE_CONSTRAINT_HASH; i++)
     {
      for (tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[i];
           tmpPtr != NULL;
           tmpPtr = tmpPtr->next)
        { 
         tmpPtr->bsaveIndex = numberOfConstraints++;
#if FUZZY_DEFTEMPLATES
         if (tmpPtr->fuzzyValuesAllowed)
            numberOfFuzzyValueConstraints++;
#endif

        }
     }

   /*=====================================================*/
   /* If dynamic constraint checking is disabled, then    */
   /* contraints won't be saved. If there are constraints */
   /* which could be saved, then issue a warning message. */
   /*=====================================================*/

   if ((! EnvGetDynamicConstraintChecking(theEnv)) && (numberOfConstraints != 0))
     {
#if FUZZY_DEFTEMPLATES
      int theIndex = 0;
      /* Fuzzy Value constraints MUST always be kept!! */
      numberOfConstraints = numberOfFuzzyValueConstraints;
      saveOnlyFuzzyValueConstraints = TRUE;
      for (i = 0 ; i < SIZE_CONSTRAINT_HASH; i++)
        {
         tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[i];
         while (tmpPtr != NULL)
           {
            if (tmpPtr->fuzzyValuesAllowed)
               tmpPtr->bsaveIndex = theIndex++;
            else
               tmpPtr->bsaveIndex = -1L;
            tmpPtr = tmpPtr->next;
           }
        }
#else
      numberOfConstraints = 0;
#endif
      PrintWarningID(theEnv,"CSTRNCMP",1,FALSE);
      EnvPrintRouter(theEnv,WWARNING,"Constraints are not saved with a constructs-to-c image\n");
      EnvPrintRouter(theEnv,WWARNING,"  when dynamic constraint checking is disabled.\n");
#if FUZZY_DEFTEMPLATES
      EnvPrintRouter(theEnv,WWARNING,"  (except Fuzzy Value constraints are always saved)\n");
#endif
     }

   if (numberOfConstraints == 0) return(-1);

   /*=================================================*/
   /* Print the extern definition in the header file. */
   /*=================================================*/

   for (i = 1; i <= (numberOfConstraints / maxIndices) + 1 ; i++)
     { fprintf(headerFP,"extern CONSTRAINT_RECORD C%d_%d[];\n",imageID,i); }

   /*==================*/
   /* Create the file. */
   /*==================*/

   if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,fileID,version,FALSE)) == NULL) return(-1);

   /*===================*/
   /* List the entries. */
   /*===================*/

   j = 0;
   count = 0;

   for (i = 0; i < SIZE_CONSTRAINT_HASH; i++)
     {
      for (tmpPtr = ConstraintData(theEnv)->ConstraintHashtable[i];
           tmpPtr != NULL;
           tmpPtr = tmpPtr->next)
        {
#if FUZZY_DEFTEMPLATES
         if (!saveOnlyFuzzyValueConstraints ||
             (saveOnlyFuzzyValueConstraints && tmpPtr->fuzzyValuesAllowed)
            )
           {
#endif
         if (newHeader)
           {
            fprintf(fp,"CONSTRAINT_RECORD C%d_%d[] = {\n",imageID,arrayVersion);
            newHeader = FALSE;
           }

#if FUZZY_DEFTEMPLATES
         fprintf(fp,"{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,",
                 tmpPtr->anyAllowed,
                 tmpPtr->symbolsAllowed,
                 tmpPtr->stringsAllowed,
                 tmpPtr->floatsAllowed,
                 tmpPtr->integersAllowed,
                 tmpPtr->instanceNamesAllowed,
                 tmpPtr->instanceAddressesAllowed,
                 tmpPtr->externalAddressesAllowed,
                 tmpPtr->factAddressesAllowed,
                 tmpPtr->fuzzyValuesAllowed,
                 tmpPtr->fuzzyValueRestriction,
                 0, /* void allowed */
                 tmpPtr->anyRestriction,
                 tmpPtr->symbolRestriction,
                 tmpPtr->stringRestriction,
                 tmpPtr->floatRestriction,
                 tmpPtr->integerRestriction,
                 tmpPtr->classRestriction,
                 tmpPtr->instanceNameRestriction);

/* Due to some restriction in the MicroSoft C compiler, it was
   necessary to use 2 fprintf statements. */

         fprintf(fp,"%d,%d",
                 tmpPtr->multifieldsAllowed,
                 tmpPtr->singlefieldsAllowed);
#else

         fprintf(fp,"{%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                 tmpPtr->anyAllowed,
                 tmpPtr->symbolsAllowed,
                 tmpPtr->stringsAllowed,
                 tmpPtr->floatsAllowed,
                 tmpPtr->integersAllowed,
                 tmpPtr->instanceNamesAllowed,
                 tmpPtr->instanceAddressesAllowed,
                 tmpPtr->externalAddressesAllowed,
                 tmpPtr->factAddressesAllowed,
                 0, /* void allowed */
                 tmpPtr->anyRestriction,
                 tmpPtr->symbolRestriction,
                 tmpPtr->stringRestriction,
                 tmpPtr->floatRestriction,
                 tmpPtr->integerRestriction,
                 tmpPtr->classRestriction,
                 tmpPtr->instanceNameRestriction,
                 tmpPtr->multifieldsAllowed,
                 tmpPtr->singlefieldsAllowed);
#endif

         fprintf(fp,",0,"); /* bsaveIndex */

         PrintHashedExpressionReference(theEnv,fp,tmpPtr->classList,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->restrictionList,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->minValue,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->maxValue,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->minFields,imageID,maxIndices);
         fprintf(fp,",");
         PrintHashedExpressionReference(theEnv,fp,tmpPtr->maxFields,imageID,maxIndices);

         /* multifield slot */

         fprintf(fp,",NULL");

         /* next slot */

         if (tmpPtr->next == NULL)
           { fprintf(fp,",NULL,"); }
         else
           {
            if ((j + 1) >= maxIndices)
              { fprintf(fp,",&C%d_%d[%d],",imageID,arrayVersion + 1,0); }
            else
              { fprintf(fp,",&C%d_%d[%d],",imageID,arrayVersion,j + 1); }
           }

         fprintf(fp,"%d,%d",tmpPtr->bucket,tmpPtr->count + 1);

         count++;
         j++;

         if ((count == numberOfConstraints) || (j >= maxIndices))
           {
            fprintf(fp,"}};\n");
            GenClose(theEnv,fp);
            j = 0;
            version++;
            arrayVersion++;
            if (count < numberOfConstraints)
              {
               if ((fp = NewCFile(theEnv,fileName,pathName,fileNameBuffer,1,version,FALSE)) == NULL) return(0);
               newHeader = TRUE;
              }
           }
         else
           { fprintf(fp,"},\n"); }
           
#if FUZZY_DEFTEMPLATES
           }
#endif

        }
     }

   return(version);
  }

/**********************************************************/
/* PrintConstraintReference: Prints C code representation */
/*   of a constraint record data structure reference.     */
/**********************************************************/
globle void PrintConstraintReference(
  void *theEnv,
  FILE *fp,
  CONSTRAINT_RECORD *cPtr,
  int imageID,
  int maxIndices)
  {
#if FUZZY_DEFTEMPLATES
   if ((cPtr == NULL) || (cPtr->bsaveIndex == -1L))
#else
   if ((cPtr == NULL) || (! EnvGetDynamicConstraintChecking(theEnv)))
#endif
     { fprintf(fp,"NULL"); }
   else fprintf(fp,"&C%d_%d[%d]",imageID,
                                 (int) (cPtr->bsaveIndex / maxIndices) + 1,
                                 (int) cPtr->bsaveIndex % maxIndices);
  }

#endif /* CONSTRUCT_COMPILER && (! RUN_TIME) */




   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/16/14            */
   /*                                                     */
   /*          DEFTEMPLATE CONSTRUCTS-TO-C MODULE         */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the constructs-to-c feature for the   */
/*    deftemplate construct.                                 */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
/*                                                           */
/* Contributing Programmer(s):                               */
/*      Brian L. Dantes                                      */
/*      Bob Orchard (NRCC - Nat'l Research Council of Canada)*/
/*                  (Fuzzy reasoning extensions)             */
/*                  (certainty factors for facts and rules)  */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Added support for templates maintaining their  */
/*            own list of facts.                             */
/*                                                           */
/*      6.30: Added support for path name argument to        */
/*            constructs-to-c.                               */
/*                                                           */
/*            Removed conditional code for unsupported       */
/*            compilers/operating systems (IBM_MCW,          */
/*            MAC_MCW, and IBM_TBC).                         */
/*                                                           */
/*            Support for deftemplate slot facets.           */
/*                                                           */
/*            Added code for deftemplate run time            */
/*            initialization of hashed comparisons to        */
/*            constants.                                     */
/*                                                           */
/*            Added const qualifiers to remove C++           */
/*            deprecation warnings.                          */
/*                                                           */
/*************************************************************/

#define _TMPLTCMP_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME)

#define SlotPrefix() ArbitraryPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem,2)

#if FUZZY_DEFTEMPLATES  
/* add 2 more files to store the Fuzzy template definitions */
#define LvUniversePrefix()  ArbitraryPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem,3)
#define PrimaryTermPrefix() ArbitraryPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem,4)
#endif

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "conscomp.h"
#include "factcmp.h"
#include "cstrncmp.h"
#include "tmpltdef.h"
#include "envrnmnt.h"

#if FUZZY_DEFTEMPLATES   
#include "fuzzyval.h"
#include "fuzzylv.h"
#include "prntutil.h"
#endif

#include "tmpltcmp.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

   static int                     ConstructToCode(void *,const char *,const char *,char *,int,FILE *,int,int);
   static void                    SlotToCode(void *,FILE *,struct templateSlot *,int,int,int);
   static void                    DeftemplateModuleToCode(void *,FILE *,struct defmodule *,int,int,int);
#if FUZZY_DEFTEMPLATES   
   static void                    DeftemplateToCode(void *,FILE *,struct deftemplate *,
                                                 int,int,int,int,int,int);
   static void                    CloseDeftemplateFiles(void *,FILE *,FILE *,FILE *,FILE *,FILE *,int);
#else
   static void                    DeftemplateToCode(void *,FILE *,struct deftemplate *,
                                                 int,int,int,int);
   static void                    CloseDeftemplateFiles(void *,FILE *,FILE *,FILE *,int);
#endif
   static void                    InitDeftemplateCode(void *,FILE *,int,int);
#if FUZZY_DEFTEMPLATES    
   static void                    LvUniverseToCode(void *,FILE *,struct fuzzyLv *,
                                                 int,int,int,int);
   static void                    primaryTermToCode(void *,FILE *,struct primary_term *,
                                                 int,int,int *, int);
#endif

/*********************************************************/
/* DeftemplateCompilerSetup: Initializes the deftemplate */
/*   construct for use with the constructs-to-c command. */
/*********************************************************/
globle void DeftemplateCompilerSetup(
  void *theEnv)
  {
#if FUZZY_DEFTEMPLATES
   DeftemplateData(theEnv)->DeftemplateCodeItem = AddCodeGeneratorItem(theEnv,"deftemplate",0,NULL,InitDeftemplateCode,ConstructToCode,5);
#else
   DeftemplateData(theEnv)->DeftemplateCodeItem = AddCodeGeneratorItem(theEnv,"deftemplate",0,NULL,InitDeftemplateCode,ConstructToCode,3);
#endif
  }

/*************************************************************/
/* ConstructToCode: Produces deftemplate code for a run-time */
/*   module created using the constructs-to-c function.      */
/*************************************************************/
static int ConstructToCode(
  void *theEnv,
  const char *fileName,
  const char *pathName,
  char *fileNameBuffer,
  int fileID,
  FILE *headerFP,
  int imageID,
  int maxIndices)
  {
   int fileCount = 1;
   struct defmodule *theModule;
   struct deftemplate *theTemplate;
   struct templateSlot *slotPtr;
   int slotCount = 0, slotArrayCount = 0, slotArrayVersion = 1;
   int moduleCount = 0, moduleArrayCount = 0, moduleArrayVersion = 1;
   int templateArrayCount = 0, templateArrayVersion = 1;
   FILE *slotFile = NULL, *moduleFile = NULL, *templateFile = NULL;
#if FUZZY_DEFTEMPLATES   
   int lvUniverseArrayCount = 0, lvUniverseArrayVersion = 1;
   int primaryTermArrayCount = 0, primaryTermArrayVersion = 1;
   FILE *lvUniverseFile = NULL, *primaryTermFile = NULL;
   struct fuzzyLv *lvPtr;
   struct primary_term *primaryTermPtr;
#endif

   /*==================================================*/
   /* Include the appropriate deftemplate header file. */
   /*==================================================*/

   fprintf(headerFP,"#include \"tmpltdef.h\"\n");

   /*=============================================================*/
   /* Loop through all the modules, all the deftemplates, and all */
   /* the deftemplate slots writing their C code representation   */
   /* to the file as they are traversed.                          */
   /*=============================================================*/

   theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);

   while (theModule != NULL)
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      moduleFile = OpenFileIfNeeded(theEnv,moduleFile,fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                                    moduleArrayVersion,headerFP,
                                    "struct deftemplateModule",ModulePrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                                    FALSE,NULL);

      if (moduleFile == NULL)
        {
#if FUZZY_DEFTEMPLATES   
         CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,
                               lvUniverseFile,primaryTermFile,maxIndices);
#else
         CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
#endif
         return(0);
        }

      DeftemplateModuleToCode(theEnv,moduleFile,theModule,imageID,maxIndices,moduleCount);
      moduleFile = CloseFileIfNeeded(theEnv,moduleFile,&moduleArrayCount,&moduleArrayVersion,
                                     maxIndices,NULL,NULL);

      /*=======================================================*/
      /* Loop through each of the deftemplates in this module. */
      /*=======================================================*/

      theTemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);

      while (theTemplate != NULL)
        {
         templateFile = OpenFileIfNeeded(theEnv,templateFile,fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                                         templateArrayVersion,headerFP,
                                         "struct deftemplate",ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                                         FALSE,NULL);
         if (templateFile == NULL)
           {
#if FUZZY_DEFTEMPLATES   
            CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,
                                  lvUniverseFile,primaryTermFile,maxIndices);
#else
            CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
#endif
            return(0);
           }

#if FUZZY_DEFTEMPLATES   
         DeftemplateToCode(theEnv,templateFile,theTemplate,imageID,maxIndices,
                           moduleCount,slotCount,
                           lvUniverseArrayCount, lvUniverseArrayVersion);
#else
         DeftemplateToCode(theEnv,templateFile,theTemplate,imageID,maxIndices,
                        moduleCount,slotCount);
#endif
         templateArrayCount++;
         templateFile = CloseFileIfNeeded(theEnv,templateFile,&templateArrayCount,&templateArrayVersion,
                                          maxIndices,NULL,NULL);

#if FUZZY_DEFTEMPLATES    
         /* write out the fuzzyLv with universe */
         lvPtr = theTemplate->fuzzyTemplate;
         if (lvPtr != NULL)
           {
             lvUniverseFile = OpenFileIfNeeded(theEnv,lvUniverseFile,fileName,pathName,fileNameBuffer,fileID,
                                               imageID,&fileCount,
                                               lvUniverseArrayVersion,headerFP,
                                               "struct fuzzyLv",
                                               LvUniversePrefix(),FALSE,NULL);
             if (lvUniverseFile == NULL)
                {
                 CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,
                                       lvUniverseFile,primaryTermFile,maxIndices);
                 return(0);
                }
              LvUniverseToCode(theEnv,lvUniverseFile,lvPtr,imageID,maxIndices,
                               primaryTermArrayCount,primaryTermArrayVersion);
              lvUniverseArrayCount++;
              lvUniverseFile = CloseFileIfNeeded(theEnv,lvUniverseFile,&lvUniverseArrayCount,
                                                &lvUniverseArrayVersion,
                                                maxIndices,NULL,NULL);

              /* now write out the primaryTerm list*/
              primaryTermPtr = lvPtr->primary_term_list;
              primaryTermFile = OpenFileIfNeeded(theEnv,primaryTermFile,fileName,pathName,fileNameBuffer,fileID,
                                                 imageID,&fileCount,
                                                 primaryTermArrayVersion,headerFP,
                                                 "struct primary_term",
                                                 PrimaryTermPrefix(),FALSE,NULL);
              if (primaryTermFile == NULL)
                {
                 CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,
                                       lvUniverseFile,primaryTermFile,maxIndices);
                 return(0);
                }

              primaryTermToCode(theEnv,primaryTermFile,primaryTermPtr,imageID,maxIndices,
                                &primaryTermArrayCount,primaryTermArrayVersion);
              primaryTermFile = CloseFileIfNeeded(theEnv,primaryTermFile,&primaryTermArrayCount,
                                               &primaryTermArrayVersion,maxIndices,NULL,NULL);




            } /* end of if (lvPtr != NULL) */
#endif

         /*======================================================*/
         /* Loop through each of the slots for this deftemplate. */
         /*======================================================*/

         slotPtr = theTemplate->slotList;
         while (slotPtr != NULL)
           {
            slotFile = OpenFileIfNeeded(theEnv,slotFile,fileName,pathName,fileNameBuffer,fileID,imageID,&fileCount,
                                        slotArrayVersion,headerFP,
                                       "struct templateSlot",SlotPrefix(),FALSE,NULL);
            if (slotFile == NULL)
              {
#if FUZZY_DEFTEMPLATES   
               CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,
                                     lvUniverseFile,primaryTermFile,maxIndices);
#else
               CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
#endif
               return(0);
              }

            SlotToCode(theEnv,slotFile,slotPtr,imageID,maxIndices,slotCount);
            slotCount++;
            slotArrayCount++;
            slotFile = CloseFileIfNeeded(theEnv,slotFile,&slotArrayCount,&slotArrayVersion,
                                         maxIndices,NULL,NULL);
            slotPtr = slotPtr->next;
           }

         theTemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theTemplate);
        }

      theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
      moduleCount++;
      moduleArrayCount++;

     }

#if FUZZY_DEFTEMPLATES    
   CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,
                         lvUniverseFile,primaryTermFile,maxIndices);
#else 
   CloseDeftemplateFiles(theEnv,moduleFile,templateFile,slotFile,maxIndices);
#endif

   return(1);
  }

/************************************************************/
/* CloseDeftemplateFiles: Closes all of the C files created */
/*   for deftemplates. Called when an error occurs or when  */
/*   the deftemplates have all been written to the files.   */
/************************************************************/
static void CloseDeftemplateFiles(
  void *theEnv,
  FILE *moduleFile,
  FILE *templateFile,
  FILE *slotFile,
#if FUZZY_DEFTEMPLATES
  FILE *lvUniverseFile,
  FILE *primaryTermFile,
#endif
  int maxIndices)
  {
   int count = maxIndices;
   int arrayVersion = 0;

   if (slotFile != NULL)
     {
      count = maxIndices;
      CloseFileIfNeeded(theEnv,slotFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }

   if (templateFile != NULL)
     {
      count = maxIndices;
      CloseFileIfNeeded(theEnv,templateFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }

   if (moduleFile != NULL)
     {
      count = maxIndices;
      CloseFileIfNeeded(theEnv,moduleFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }
     
#if FUZZY_DEFTEMPLATES   
   if (lvUniverseFile != NULL)
     {
      count = maxIndices;
      lvUniverseFile = CloseFileIfNeeded(theEnv,lvUniverseFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }

   if (primaryTermFile != NULL)
     {
      count = maxIndices;
      primaryTermFile = CloseFileIfNeeded(theEnv,primaryTermFile,&count,&arrayVersion,maxIndices,NULL,NULL);
     }
#endif
  }
  
#if FUZZY_DEFTEMPLATES   

/* generate code for the fuzzy deftemplate definitions */

/************************************************************/
/* LvUniverseToCode:                                        */
/************************************************************/
#if IBM_TBC
#pragma argsused
#endif
static void LvUniverseToCode(
  void *theEnv,
  FILE *lvUniverseFile,
  struct fuzzyLv *lvPtr,
  int imageID,
  int maxIndices,
  int primaryTermArrayCount,
  int primaryTermArrayVersion)
  {
#if MAC_MPW
#pragma unused(maxIndices)
#endif
    fprintf(lvUniverseFile, "{%s, %s, ",
            FloatToString(theEnv,lvPtr->from), FloatToString(theEnv,lvPtr->to));
    PrintSymbolReference(theEnv,lvUniverseFile,lvPtr->units);
    fprintf(lvUniverseFile, ", &%s%d_%d[%d] }",
            PrimaryTermPrefix(), imageID, primaryTermArrayVersion, primaryTermArrayCount);
  }

/************************************************************/
/* primaryTermToCode:                                       */
/************************************************************/
static void primaryTermToCode(
  void *theEnv,
  FILE *primaryTermFile,
  struct primary_term *primaryTermPtr,
  int imageID,
  int maxIndices,
  int *primaryTermArrayCount,
  int primaryTermArrayVersion)
  {
    int count, arrayVersion;
    struct primary_term *nextPtr;

    while (primaryTermPtr != NULL)
       {
         nextPtr = primaryTermPtr->next;
         if ((nextPtr == NULL) && (*primaryTermArrayCount >= maxIndices))
           {
             count = 0;
             arrayVersion = primaryTermArrayVersion+1;
           }
         else
           {
             count = *primaryTermArrayCount+1;
             arrayVersion = primaryTermArrayVersion;
           }

         fprintf(primaryTermFile,"{");
         PrintFuzzyValueReference(theEnv, primaryTermFile, primaryTermPtr->fuzzy_value_description);
         if (nextPtr != NULL)
            fprintf(primaryTermFile,",&%s%d_%d[%d]}",
                    PrimaryTermPrefix(), imageID, arrayVersion, count);

         else
            fprintf(primaryTermFile, ",NULL}");

         *primaryTermArrayCount += 1;
         primaryTermPtr = nextPtr;

         if (primaryTermPtr != NULL)
         fprintf(primaryTermFile,",");
       }
  }

#endif

/*************************************************************/
/* DeftemplateModuleToCode: Writes the C code representation */
/*   of a single deftemplate module to the specified file.   */
/*************************************************************/
static void DeftemplateModuleToCode(
  void *theEnv,
  FILE *theFile,
  struct defmodule *theModule,
  int imageID,
  int maxIndices,
  int moduleCount)
  {
#if MAC_XCD
#pragma unused(moduleCount)
#endif

   fprintf(theFile,"{");

   ConstructModuleToCode(theEnv,theFile,theModule,imageID,maxIndices,
                         DeftemplateData(theEnv)->DeftemplateModuleIndex,ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem));

   fprintf(theFile,"}");
  }

/************************************************************/
/* DeftemplateToCode: Writes the C code representation of a */
/*   single deftemplate construct to the specified file.    */
/************************************************************/
static void DeftemplateToCode(
  void *theEnv,
  FILE *theFile,
  struct deftemplate *theTemplate,
  int imageID,
  int maxIndices,
  int moduleCount,
#if ! FUZZY_DEFTEMPLATES
  int slotCount)
#else 
  int slotCount,
  int lvUniverseArrayCount,
  int lvUniverseArrayVersion)
#endif
  {
   /*====================*/
   /* Deftemplate Header */
   /*====================*/

   fprintf(theFile,"{");

   ConstructHeaderToCode(theEnv,theFile,&theTemplate->header,imageID,maxIndices,
                                  moduleCount,ModulePrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                                  ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem));
   fprintf(theFile,",");

   /*===========*/
   /* Slot List */
   /*===========*/

   if (theTemplate->slotList == NULL)
     { fprintf(theFile,"NULL,"); }
   else
     {
      fprintf(theFile,"&%s%d_%d[%d],",SlotPrefix(),
                 imageID,
                 (slotCount / maxIndices) + 1,
                 slotCount % maxIndices);
     }

   /*==========================================*/
   /* Implied Flag, Watch Flag, In Scope Flag, */
   /* Number of Slots, and Busy Count.         */
   /*==========================================*/

   fprintf(theFile,"%d,0,0,%d,%ld,",theTemplate->implied,theTemplate->numberOfSlots,theTemplate->busyCount);

   /*=================*/
   /* Pattern Network */
   /*=================*/

   if (theTemplate->patternNetwork == NULL)
     { fprintf(theFile,"NULL"); }
   else
     { FactPatternNodeReference(theEnv,theTemplate->patternNetwork,theFile,imageID,maxIndices); }

#if FUZZY_DEFTEMPLATES  

   /*==========================================*/
   /* hasFuzzySlots field and fuzzyTemplate    */
   /*==========================================*/

   if (theTemplate->fuzzyTemplate == NULL)
     { fprintf(theFile,",%d,NULL", theTemplate->hasFuzzySlots); }
   else
     { fprintf(theFile, ",%d,(struct fuzzyLv *)&%s%d_%d[%d]",
                        theTemplate->hasFuzzySlots,
                        LvUniversePrefix(),imageID,
                        lvUniverseArrayVersion,lvUniverseArrayCount);
     }

#endif /* FUZZY_DEFTEMPLATES */

   /*============================================*/
   /* Print the factList and lastFact references */
   /* and close the structure.                   */
   /*============================================*/
   
   fprintf(theFile,",NULL,NULL}");
  }

/*****************************************************/
/* SlotToCode: Writes the C code representation of a */
/*   single deftemplate slot to the specified file.  */
/*****************************************************/
static void SlotToCode(
  void *theEnv,
  FILE *theFile,
  struct templateSlot *theSlot,
  int imageID,
  int maxIndices,
  int slotCount)
  {
   /*===========*/
   /* Slot Name */
   /*===========*/

   fprintf(theFile,"{");
   PrintSymbolReference(theEnv,theFile,theSlot->slotName);

   /*=============================*/
   /* Multislot and Default Flags */
   /*=============================*/

   fprintf(theFile,",%d,%d,%d,%d,",theSlot->multislot,theSlot->noDefault,
                                   theSlot->defaultPresent,theSlot->defaultDynamic);

   /*=============*/
   /* Constraints */
   /*=============*/

   PrintConstraintReference(theEnv,theFile,theSlot->constraints,imageID,maxIndices);

   /*===============*/
   /* Default Value */
   /*===============*/

   fprintf(theFile,",");
   PrintHashedExpressionReference(theEnv,theFile,theSlot->defaultList,imageID,maxIndices);
   
   /*============*/
   /* Facet List */
   /*============*/
   
   fprintf(theFile,",");
   PrintHashedExpressionReference(theEnv,theFile,theSlot->facetList,imageID,maxIndices);
   fprintf(theFile,",");

   /*===========*/
   /* Next Slot */
   /*===========*/

   if (theSlot->next == NULL)
     { fprintf(theFile,"NULL}"); }
   else
     {
      fprintf(theFile,"&%s%d_%d[%d]}",SlotPrefix(),imageID,
                               ((slotCount+1) / maxIndices) + 1,
                                (slotCount+1) % maxIndices);
     }
  }

/*****************************************************************/
/* DeftemplateCModuleReference: Writes the C code representation */
/*   of a reference to a deftemplate module data structure.      */
/*****************************************************************/
globle void DeftemplateCModuleReference(
  void *theEnv,
  FILE *theFile,
  int count,
  int imageID,
  int maxIndices)
  {
   fprintf(theFile,"MIHS &%s%d_%d[%d]",ModulePrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                      imageID,
                      (count / maxIndices) + 1,
                      (count % maxIndices));
  }

/********************************************************************/
/* DeftemplateCConstructReference: Writes the C code representation */
/*   of a reference to a deftemplate data structure.                */
/********************************************************************/
globle void DeftemplateCConstructReference(
  void *theEnv,
  FILE *theFile,
  void *vTheTemplate,
  int imageID,
  int maxIndices)
  {
   struct deftemplate *theTemplate = (struct deftemplate *) vTheTemplate;

   if (theTemplate == NULL)
     { fprintf(theFile,"NULL"); }
   else
     {
      fprintf(theFile,"&%s%d_%ld[%ld]",ConstructPrefix(DeftemplateData(theEnv)->DeftemplateCodeItem),
                      imageID,
                      (theTemplate->header.bsaveID / maxIndices) + 1,
                      theTemplate->header.bsaveID % maxIndices);
     }

  }
  
/*******************************************/
/* InitDeftemplateCode: Writes out runtime */
/*   initialization code for deftemplates. */
/*******************************************/
static void InitDeftemplateCode(
  void *theEnv,
  FILE *initFP,
  int imageID,
  int maxIndices)
  {
#if MAC_XCD
#pragma unused(theEnv)
#pragma unused(imageID)
#pragma unused(maxIndices)
#endif

   fprintf(initFP,"   DeftemplateRunTimeInitialize(theEnv);\n");
  }

#endif /* DEFTEMPLATE_CONSTRUCT && CONSTRUCT_COMPILER && (! RUN_TIME) */


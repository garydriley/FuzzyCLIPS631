   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*             CLIPS Version 6.30  08/16/14            */
   /*                                                     */
   /*            DEFTEMPLATE BSAVE/BLOAD MODULE           */
   /*******************************************************/

/*************************************************************/
/* Purpose: Implements the binary save/load feature for the  */
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
/*                  (extensions to run command)              */
/*                                                           */
/* Revision History:                                         */
/*                                                           */
/*      6.23: Added support for templates maintaining their  */
/*            own list of facts.                             */
/*                                                           */
/*      6.30: Changed integer type/precision.                */
/*                                                           */
/*            Support for deftemplate slot facets.           */
/*                                                           */
/*************************************************************/

#define  _TMPLTBIN_SOURCE_

#include "setup.h"

#if DEFTEMPLATE_CONSTRUCT && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME)

#include <stdio.h>
#define _STDIO_INCLUDED_

#include "memalloc.h"
#include "bload.h"
#include "bsave.h"
#include "factbin.h"
#include "cstrnbin.h"
#include "factmngr.h"
#include "tmpltpsr.h"
#include "tmpltdef.h"
#include "tmpltutl.h"
#include "envrnmnt.h"

#if FUZZY_DEFTEMPLATES 
#include "fuzzylv.h"
#include "fuzzyval.h"
#include "dffnxbin.h"
#endif

#include "tmpltbin.h"

/***************************************/
/* LOCAL INTERNAL FUNCTION DEFINITIONS */
/***************************************/

#if BLOAD_AND_BSAVE
   static void                    BsaveFind(void *);
   static void                    BsaveStorage(void *,FILE *);
   static void                    BsaveBinaryItem(void *,FILE *);
#endif
   static void                    BloadStorage(void *);
   static void                    BloadBinaryItem(void *);
   static void                    UpdateDeftemplateModule(void *,void *,long);
   static void                    UpdateDeftemplate(void *,void *,long);
   static void                    UpdateDeftemplateSlot(void *,void *,long);
#if FUZZY_DEFTEMPLATES   
   static void                    UpdateLvPlusUniverse(void *,void *,long);
   static void                    UpdateFuzzyPrimaryTerms(void *,void *,long);
#endif
   static void                    ClearBload(void *);
   static void                    DeallocateDeftemplateBloadData(void *);

/***********************************************/
/* DeftemplateBinarySetup: Installs the binary */
/*   save/load feature for deftemplates.       */
/***********************************************/
globle void DeftemplateBinarySetup(
  void *theEnv)
  {
   AllocateEnvironmentData(theEnv,TMPLTBIN_DATA,sizeof(struct deftemplateBinaryData),DeallocateDeftemplateBloadData);
#if BLOAD_AND_BSAVE
   AddBinaryItem(theEnv,"deftemplate",0,BsaveFind,NULL,
                             BsaveStorage,BsaveBinaryItem,
                             BloadStorage,BloadBinaryItem,
                             ClearBload);
#endif
#if (BLOAD || BLOAD_ONLY)
   AddBinaryItem(theEnv,"deftemplate",0,NULL,NULL,NULL,NULL,
                             BloadStorage,BloadBinaryItem,
                             ClearBload);
#endif
  }
  
/***********************************************************/
/* DeallocateDeftemplateBloadData: Deallocates environment */
/*    data for the deftemplate bsave functionality.        */
/***********************************************************/
static void DeallocateDeftemplateBloadData(
  void *theEnv)
  {
   size_t space;

   space =  DeftemplateBinaryData(theEnv)->NumberOfTemplateModules * sizeof(struct deftemplateModule);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->ModuleArray,space);
   
   space = DeftemplateBinaryData(theEnv)->NumberOfDeftemplates * sizeof(struct deftemplate);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->DeftemplateArray,space);

   space =  DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots * sizeof(struct templateSlot);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->SlotArray,space);

#if FUZZY_DEFTEMPLATES   
   space =  DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms * sizeof(struct primary_term);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms,space);

   space =  DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates * sizeof(struct fuzzyLv);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->LvPlusUniverseArray,space);
#endif
  }

#if BLOAD_AND_BSAVE

/**************************************************************/
/* BsaveFind: Counts the number of data structures which must */
/*   be saved in the binary image for the deftemplates in the */
/*   current environment.                                     */
/**************************************************************/
static void BsaveFind(
  void *theEnv)
  {
   struct deftemplate *theDeftemplate;
   struct templateSlot *theSlot;
   struct defmodule *theModule;

   /*=======================================================*/
   /* If a binary image is already loaded, then temporarily */
   /* save the count values since these will be overwritten */
   /* in the process of saving the binary image.            */
   /*=======================================================*/

   SaveBloadCount(theEnv,DeftemplateBinaryData(theEnv)->NumberOfDeftemplates);
   SaveBloadCount(theEnv,DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots);
   SaveBloadCount(theEnv,DeftemplateBinaryData(theEnv)->NumberOfTemplateModules);
#if FUZZY_DEFTEMPLATES 
   SaveBloadCount(theEnv,DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates);
   SaveBloadCount(theEnv,DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms);
#endif

   /*==================================================*/
   /* Set the count of deftemplates, deftemplate slots */
   /* and deftemplate module data structures to zero.  */
   /*==================================================*/

   DeftemplateBinaryData(theEnv)->NumberOfDeftemplates = 0;
   DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots = 0;
   DeftemplateBinaryData(theEnv)->NumberOfTemplateModules = 0;
#if FUZZY_DEFTEMPLATES   
   DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates = 0;
   DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms = 0;
#endif

   /*===========================*/
   /* Loop through each module. */
   /*===========================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      /*============================================*/
      /* Set the current module to the module being */
      /* examined and increment the number of       */
      /* deftemplate modules encountered.           */
      /*============================================*/

      EnvSetCurrentModule(theEnv,(void *) theModule);
      DeftemplateBinaryData(theEnv)->NumberOfTemplateModules++;

      /*======================================================*/
      /* Loop through each deftemplate in the current module. */
      /*======================================================*/

      for (theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);
           theDeftemplate != NULL;
           theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theDeftemplate))
        {
         /*======================================================*/
         /* Initialize the construct header for the binary save. */
         /*======================================================*/

         MarkConstructHeaderNeededItems(&theDeftemplate->header,
                                        DeftemplateBinaryData(theEnv)->NumberOfDeftemplates++);

#if FUZZY_DEFTEMPLATES  
         if (theDeftemplate->fuzzyTemplate != NULL)
           {
             struct primary_term *ptptr;

             /* fuzzy template -- deal with the fuzzyLv (linguistic value)
                which contains the universe of discourse, and the list of
                primary terms
                Count the number of fuzzy lv's and the number of primary terms
                ... also mark the fuzzy values and symbols (fv names, unit names)
                as 'needed' for bsave
             */
             DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates++;

             if (theDeftemplate->fuzzyTemplate->units != NULL)
                theDeftemplate->fuzzyTemplate->units->neededSymbol = TRUE;

             ptptr = theDeftemplate->fuzzyTemplate->primary_term_list;
             while (ptptr != NULL)
                {
                  DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms++;
                  ptptr->fuzzy_value_description->neededFuzzyValue = TRUE;
                  ptptr = ptptr->next;
                }
            }
#endif

         /*=============================================================*/
         /* Loop through each slot in the deftemplate, incrementing the */
         /* slot count and marking the slot names as needed symbols.    */
         /*=============================================================*/

         for (theSlot = theDeftemplate->slotList;
              theSlot != NULL;
              theSlot = theSlot->next)
           {
            DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots++;
            theSlot->slotName->neededSymbol = TRUE;
           }
        }

     }
  }

/*********************************************************/
/* BsaveStorage: Writes out the storage requirements for */
/*    all deftemplate structures to the binary file.     */
/*********************************************************/
static void BsaveStorage(
  void *theEnv,
  FILE *fp)
  {
   size_t space;

   /*========================================================================*/
   /* Three data structures are saved as part of a deftemplate binary image: */
   /* the deftemplate data structure, the deftemplateModule data structure,  */
   /* and the templateSlot data structure. The data structures associated    */
   /* with default values and constraints are not save with the deftemplate  */
   /* portion of the binary image.                                           */
   /*========================================================================*/

#if ! FUZZY_DEFTEMPLATES
   space = sizeof(long) * 3;
#else
   space = sizeof(long) * 5;
#endif
   GenWrite(&space,sizeof(size_t),fp);
   GenWrite(&DeftemplateBinaryData(theEnv)->NumberOfDeftemplates,sizeof(long int),fp);
   GenWrite(&DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots,sizeof(long int),fp);
   GenWrite(&DeftemplateBinaryData(theEnv)->NumberOfTemplateModules,sizeof(long int),fp);
#if FUZZY_DEFTEMPLATES   
   GenWrite(&DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates,(unsigned long) sizeof(long int),fp);
   GenWrite(&DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms,(unsigned long) sizeof(long int),fp);
#endif
  }

/***********************************************/
/* BsaveBinaryItem: Writes out all deftemplate */
/*   structures to the binary file.            */
/***********************************************/
static void BsaveBinaryItem(
  void *theEnv,
  FILE *fp)
  {
   size_t space;
   struct deftemplate *theDeftemplate;
   struct bsaveDeftemplate tempDeftemplate;
   struct templateSlot *theSlot;
   struct bsaveTemplateSlot tempTemplateSlot;
   struct bsaveDeftemplateModule tempTemplateModule;
   struct defmodule *theModule;
   struct deftemplateModule *theModuleItem;
#if FUZZY_DEFTEMPLATES  
   long lastNumberOfFuzzyPrimaryTerms;
#endif

   /*============================================================*/
   /* Write out the amount of space taken up by the deftemplate, */
   /* deftemplateModule, and templateSlot data structures in the */
   /* binary image.                                              */
   /*============================================================*/

   space = (DeftemplateBinaryData(theEnv)->NumberOfDeftemplates * sizeof(struct bsaveDeftemplate)) +
           (DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots * sizeof(struct bsaveTemplateSlot)) +
#if FUZZY_DEFTEMPLATES  
           (DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates * sizeof(struct bsaveLvPlusUniverse)) +
           (DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms * sizeof(struct bsaveFuzzyPrimaryTerm)) +
#endif
           (DeftemplateBinaryData(theEnv)->NumberOfTemplateModules * sizeof(struct bsaveDeftemplateModule));
   GenWrite(&space,sizeof(size_t),fp);

   /*===================================================*/
   /* Write out each deftemplate module data structure. */
   /*===================================================*/

   DeftemplateBinaryData(theEnv)->NumberOfDeftemplates = 0;
#if FUZZY_DEFTEMPLATES   
   DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates = 0;
   DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms = 0;
   lastNumberOfFuzzyPrimaryTerms = 0;
#endif
   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      theModuleItem = (struct deftemplateModule *)
                      GetModuleItem(theEnv,NULL,FindModuleItem(theEnv,"deftemplate")->moduleIndex);
      AssignBsaveDefmdlItemHdrVals(&tempTemplateModule.header,
                                           &theModuleItem->header);
      GenWrite(&tempTemplateModule,sizeof(struct bsaveDeftemplateModule),fp);
     }

   /*============================================*/
   /* Write out each deftemplate data structure. */
   /*============================================*/

   DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots = 0;
   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      for (theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);
           theDeftemplate != NULL;
           theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theDeftemplate))
        {
         AssignBsaveConstructHeaderVals(&tempDeftemplate.header,
                                          &theDeftemplate->header);
         tempDeftemplate.implied = theDeftemplate->implied;
         tempDeftemplate.numberOfSlots = theDeftemplate->numberOfSlots;
         tempDeftemplate.patternNetwork = BsaveFactPatternIndex(theDeftemplate->patternNetwork);
         
#if FUZZY_DEFTEMPLATES   
         /* a -1 in fuzzyTemplateList slot of tempDeftemplate indicates
                    NOT a fuzzy deftemplate -- other values give index into the
                        LvPlusUniverseArray for resolving ptr when read back in
             */
         if (theDeftemplate->fuzzyTemplate == NULL)
             tempDeftemplate.fuzzyTemplateList = -1L;
         else
                     tempDeftemplate.fuzzyTemplateList = DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates;
         tempDeftemplate.hasFuzzySlots = theDeftemplate->hasFuzzySlots;
#endif

         if (theDeftemplate->slotList != NULL)
           { tempDeftemplate.slotList = DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots; }
         else tempDeftemplate.slotList = -1L;

         GenWrite(&tempDeftemplate,sizeof(struct bsaveDeftemplate),fp);

#if FUZZY_DEFTEMPLATES    
         if (theDeftemplate->fuzzyTemplate != NULL)
                    DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates++;
#endif

         DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots += theDeftemplate->numberOfSlots;
        }
     }

#if FUZZY_DEFTEMPLATES   
   /* At this point the bsave templates are written out .. now append
      to that for any fuzzy deftemplates

             1. write out the Lv with universe of discourse
             2. write out the list of primary terms

          all fuzzyLv structs written, then all primary terms
   */

   if (DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates > 0)
    {
     theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
     while (theModule != NULL)
       {
        EnvSetCurrentModule(theEnv,(void *) theModule);

        theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);
        while (theDeftemplate != NULL)
          {
           if (theDeftemplate->fuzzyTemplate != NULL)
             {
               struct bsaveLvPlusUniverse bLVFU;
               struct fuzzyLv *flvptr = theDeftemplate->fuzzyTemplate;
               struct primary_term *ptptr;

               ptptr = flvptr->primary_term_list;
               while (ptptr != NULL)
                  { /* count the primary terms */
                    DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms++;
                    ptptr = ptptr->next;
                  }

               /* -1 means no primary terms or no modifiers */
               bLVFU.ptPtr = (DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms == lastNumberOfFuzzyPrimaryTerms) ?
                              -1L : lastNumberOfFuzzyPrimaryTerms;
               bLVFU.from = flvptr->from;
               bLVFU.to = flvptr->to;
               if (flvptr->units == NULL) /* may not be a unit specifier */
                  bLVFU.unitsName = -1L;
               else
                  bLVFU.unitsName = (long)flvptr->units->bucket;
               GenWrite(&bLVFU,(unsigned long) sizeof(struct bsaveLvPlusUniverse),fp);

               lastNumberOfFuzzyPrimaryTerms = DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms;
             }

           theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theDeftemplate);
          }

        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
       }

     theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
     while (theModule != NULL)
       {
        EnvSetCurrentModule(theEnv,(void *) theModule);

        theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);
        while (theDeftemplate != NULL)
          {
           if (theDeftemplate->fuzzyTemplate != NULL)
             {
               struct fuzzyLv *flvptr = theDeftemplate->fuzzyTemplate;
               struct primary_term *ptptr;
               struct bsaveFuzzyPrimaryTerm bFPT;

               ptptr = flvptr->primary_term_list;
               while (ptptr != NULL)
                 { /* write out the primary terms */
                   bFPT.fuzzyValue = (long)ptptr->fuzzy_value_description->bucket;
                   if (ptptr->next != NULL)
                      bFPT.next = 0L;
                   else
                      bFPT.next = -1L;

                   GenWrite(&bFPT,(unsigned long) sizeof(struct bsaveFuzzyPrimaryTerm),fp);
                                   ptptr = ptptr->next;
                  }
              }

           theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theDeftemplate);
          }

        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule);
       }

     }
#endif

   /*=============================================*/
   /* Write out each templateSlot data structure. */
   /*=============================================*/

   for (theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,NULL);
        theModule != NULL;
        theModule = (struct defmodule *) EnvGetNextDefmodule(theEnv,theModule))
     {
      EnvSetCurrentModule(theEnv,(void *) theModule);

      for (theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,NULL);
           theDeftemplate != NULL;
           theDeftemplate = (struct deftemplate *) EnvGetNextDeftemplate(theEnv,theDeftemplate))
        {
         for (theSlot = theDeftemplate->slotList;
              theSlot != NULL;
              theSlot = theSlot->next)
           {
            tempTemplateSlot.constraints = ConstraintIndex(theSlot->constraints);
            tempTemplateSlot.slotName = theSlot->slotName->bucket;
            tempTemplateSlot.multislot = theSlot->multislot;
            tempTemplateSlot.noDefault = theSlot->noDefault;
            tempTemplateSlot.defaultPresent = theSlot->defaultPresent;
            tempTemplateSlot.defaultDynamic = theSlot->defaultDynamic;
            tempTemplateSlot.defaultList = HashedExpressionIndex(theEnv,theSlot->defaultList);
            tempTemplateSlot.facetList = HashedExpressionIndex(theEnv,theSlot->facetList);

            if (theSlot->next != NULL) tempTemplateSlot.next = 0L;
            else tempTemplateSlot.next = -1L;

            GenWrite(&tempTemplateSlot,sizeof(struct bsaveTemplateSlot),fp);
           }
        }
     }

   /*=============================================================*/
   /* If a binary image was already loaded when the bsave command */
   /* was issued, then restore the counts indicating the number   */
   /* of deftemplates, deftemplate modules, and deftemplate slots */
   /* in the binary image (these were overwritten by the binary   */
   /* save).                                                      */
   /*=============================================================*/

   RestoreBloadCount(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfDeftemplates);
   RestoreBloadCount(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots);
   RestoreBloadCount(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfTemplateModules);
#if FUZZY_DEFTEMPLATES    
   RestoreBloadCount(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates);
   RestoreBloadCount(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms);
#endif
  }

#endif /* BLOAD_AND_BSAVE */

/****************************************************/
/* BloadStorage: Allocates storage requirements for */
/*   the deftemplates used by this binary image.    */
/****************************************************/
static void BloadStorage(
  void *theEnv)
  {
   size_t space;

   /*=========================================================*/
   /* Determine the number of deftemplate, deftemplateModule, */
   /* and templateSlot data structures to be read.            */
   /*=========================================================*/

   GenReadBinary(theEnv,&space,sizeof(size_t));
   GenReadBinary(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfDeftemplates,sizeof(long int));
   GenReadBinary(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots,sizeof(long int));
   GenReadBinary(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfTemplateModules,sizeof(long int));
#if FUZZY_DEFTEMPLATES   
   GenReadBinary(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates,(unsigned long) sizeof(long int));
   GenReadBinary(theEnv,&DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms,(unsigned long) sizeof(long int));
#endif

   /*====================================*/
   /* Allocate the space needed for the  */
   /* deftemplateModule data structures. */
   /*====================================*/

   if (DeftemplateBinaryData(theEnv)->NumberOfTemplateModules == 0)
     {
      DeftemplateBinaryData(theEnv)->DeftemplateArray = NULL;
      DeftemplateBinaryData(theEnv)->SlotArray = NULL;
      DeftemplateBinaryData(theEnv)->ModuleArray = NULL;
#if FUZZY_DEFTEMPLATES
      DeftemplateBinaryData(theEnv)->LvPlusUniverseArray = NULL;
      DeftemplateBinaryData(theEnv)->PrimaryTermArray = NULL;
#endif
      return;
     }

   space = DeftemplateBinaryData(theEnv)->NumberOfTemplateModules * sizeof(struct deftemplateModule);
   DeftemplateBinaryData(theEnv)->ModuleArray = (struct deftemplateModule *) genalloc(theEnv,space);

   /*===================================*/
   /* Allocate the space needed for the */
   /* deftemplate data structures.      */
   /*===================================*/

   if (DeftemplateBinaryData(theEnv)->NumberOfDeftemplates == 0)
     {
      DeftemplateBinaryData(theEnv)->DeftemplateArray = NULL;
      DeftemplateBinaryData(theEnv)->SlotArray = NULL;
#if FUZZY_DEFTEMPLATES   
      DeftemplateBinaryData(theEnv)->LvPlusUniverseArray = NULL;
      DeftemplateBinaryData(theEnv)->PrimaryTermArray = NULL;
#endif
      return;
     }

   space = DeftemplateBinaryData(theEnv)->NumberOfDeftemplates * sizeof(struct deftemplate);
   DeftemplateBinaryData(theEnv)->DeftemplateArray = (struct deftemplate *) genalloc(theEnv,space);

#if FUZZY_DEFTEMPLATES   
   /*==============================================*/
   /* Get space for the LvPlusUniverse structures. */
   /*==============================================*/

   if (DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates == 0)
     {
      DeftemplateBinaryData(theEnv)->LvPlusUniverseArray = NULL;
     }
   else
     {
      space = DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates * sizeof(struct fuzzyLv);

      DeftemplateBinaryData(theEnv)->LvPlusUniverseArray = (struct fuzzyLv *) genalloc(theEnv,space);
     }

   /*============================================*/
   /* Get space for the primary term structures. */
   /*============================================*/

   if (DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms == 0)
     {
      DeftemplateBinaryData(theEnv)->PrimaryTermArray = NULL;
     }
   else
     {
      space = DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms * sizeof(struct primary_term);

      DeftemplateBinaryData(theEnv)->PrimaryTermArray = (struct primary_term *) genalloc(theEnv,space);
     }

#endif  /* FUZZY_DEFTEMPLATES */

   /*===================================*/
   /* Allocate the space needed for the */
   /* templateSlot data structures.     */
   /*===================================*/

   if (DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots == 0)
     {
      DeftemplateBinaryData(theEnv)->SlotArray = NULL;
      return;
     }

   space =  DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots * sizeof(struct templateSlot);
   DeftemplateBinaryData(theEnv)->SlotArray = (struct templateSlot *) genalloc(theEnv,space);
  }

/********************************************************/
/* BloadBinaryItem: Loads and refreshes the deftemplate */
/*   constructs used by this binary image.              */
/********************************************************/
static void BloadBinaryItem(
  void *theEnv)
  {
   size_t space;

   /*======================================================*/
   /* Read in the amount of space used by the binary image */
   /* (this is used to skip the construct in the event it  */
   /* is not available in the version being run).          */
   /*======================================================*/

   GenReadBinary(theEnv,&space,sizeof(size_t));

   /*===============================================*/
   /* Read in the deftemplateModule data structures */
   /* and refresh the pointers.                     */
   /*===============================================*/

   BloadandRefresh(theEnv,DeftemplateBinaryData(theEnv)->NumberOfTemplateModules,sizeof(struct bsaveDeftemplateModule),
                   UpdateDeftemplateModule);

   /*===============================================*/
   /* Read in the deftemplateModule data structures */
   /* and refresh the pointers.                     */
   /*===============================================*/

   BloadandRefresh(theEnv,DeftemplateBinaryData(theEnv)->NumberOfDeftemplates,sizeof(struct bsaveDeftemplate),
                   UpdateDeftemplate);
                   
#if FUZZY_DEFTEMPLATES   
   BloadandRefresh(theEnv,DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates,(unsigned) sizeof(struct bsaveLvPlusUniverse),
                   UpdateLvPlusUniverse);
   BloadandRefresh(theEnv,DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms,(unsigned) sizeof(struct bsaveFuzzyPrimaryTerm),
                   UpdateFuzzyPrimaryTerms);
#endif  /* FUZZY_DEFTEMPLATES */


   /*==========================================*/
   /* Read in the templateSlot data structures */
   /* and refresh the pointers.                */
   /*==========================================*/

   BloadandRefresh(theEnv,DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots,sizeof(struct bsaveTemplateSlot),
                   UpdateDeftemplateSlot);
  }

/**************************************************/
/* UpdateDeftemplateModule: Bload refresh routine */
/*   for deftemplateModule data structures.       */
/**************************************************/
static void UpdateDeftemplateModule(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct bsaveDeftemplateModule *bdmPtr;

   bdmPtr = (struct bsaveDeftemplateModule *) buf;
   UpdateDefmoduleItemHeader(theEnv,&bdmPtr->header,&DeftemplateBinaryData(theEnv)->ModuleArray[obji].header,
                             (int) sizeof(struct deftemplate),
                             (void *) DeftemplateBinaryData(theEnv)->DeftemplateArray);
  }

/********************************************/
/* UpdateDeftemplate: Bload refresh routine */
/*   for deftemplate data structures.       */
/********************************************/
static void UpdateDeftemplate(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct deftemplate *theDeftemplate;
   struct bsaveDeftemplate *bdtPtr;

   bdtPtr = (struct bsaveDeftemplate *) buf;
   theDeftemplate = (struct deftemplate *) &DeftemplateBinaryData(theEnv)->DeftemplateArray[obji];

   UpdateConstructHeader(theEnv,&bdtPtr->header,&theDeftemplate->header,
                         (int) sizeof(struct deftemplateModule),(void *) DeftemplateBinaryData(theEnv)->ModuleArray,
                         (int) sizeof(struct deftemplate),(void *) DeftemplateBinaryData(theEnv)->DeftemplateArray);

   if (bdtPtr->slotList != -1L)
     { theDeftemplate->slotList = (struct templateSlot *) &DeftemplateBinaryData(theEnv)->SlotArray[bdtPtr->slotList]; }
   else
     { theDeftemplate->slotList = NULL; }
     
#if FUZZY_DEFTEMPLATES   
   if (bdtPtr->fuzzyTemplateList != -1L)
     { theDeftemplate->fuzzyTemplate = (struct fuzzyLv *) &DeftemplateBinaryData(theEnv)->LvPlusUniverseArray[bdtPtr->fuzzyTemplateList]; }
   else
     { theDeftemplate->fuzzyTemplate = NULL; }

   theDeftemplate->hasFuzzySlots = bdtPtr->hasFuzzySlots;
#endif

   if (bdtPtr->patternNetwork != -1L)
     { theDeftemplate->patternNetwork = (struct factPatternNode *) BloadFactPatternPointer(bdtPtr->patternNetwork); }
   else
     { theDeftemplate->patternNetwork = NULL; }

   theDeftemplate->implied = bdtPtr->implied;
#if DEBUGGING_FUNCTIONS
   theDeftemplate->watch = FactData(theEnv)->WatchFacts;
#endif
   theDeftemplate->inScope = FALSE;
   theDeftemplate->numberOfSlots = (unsigned short) bdtPtr->numberOfSlots;
   theDeftemplate->factList = NULL;
   theDeftemplate->lastFact = NULL;
  }
  
#if FUZZY_DEFTEMPLATES   
/******************************************************/
/* UpdateLvPlusUniverse: Updates pointers in bloaded  */
/*   LvPlusUniverse slot structures.                  */
/******************************************************/
static void UpdateLvPlusUniverse(
  void *theEnv,
  void *buf,
  long obji)
  {
    struct fuzzyLv *lvpuPtr;
    struct bsaveLvPlusUniverse *blvpuPtr;

    blvpuPtr = (struct bsaveLvPlusUniverse *) buf;
    lvpuPtr = (struct fuzzyLv *) &DeftemplateBinaryData(theEnv)->LvPlusUniverseArray[obji];

    if (blvpuPtr->ptPtr != -1L)
       lvpuPtr->primary_term_list = (struct primary_term *) &DeftemplateBinaryData(theEnv)->PrimaryTermArray[blvpuPtr->ptPtr];
    else
       lvpuPtr->primary_term_list = NULL;

    lvpuPtr->from = blvpuPtr->from;
    lvpuPtr->to = blvpuPtr->to;
    if (blvpuPtr->unitsName == -1L)
       lvpuPtr->units = NULL;
    else
       {
         lvpuPtr->units = SymbolData(theEnv)->SymbolArray[blvpuPtr->unitsName];
         IncrementSymbolCount(lvpuPtr->units);
       }
  }

/*********************************************************/
/* UpdateFuzzyPrimaryTerms: Updates pointers in bloaded  */
/*   primary term data structures.                       */
/*********************************************************/
static void UpdateFuzzyPrimaryTerms(
  void *theEnv,
  void *buf,
  long obji)
  {
    struct primary_term *ptPtr;
    struct bsaveFuzzyPrimaryTerm *bptPtr;

        bptPtr = (struct bsaveFuzzyPrimaryTerm *) buf;
        ptPtr = (struct primary_term *) &DeftemplateBinaryData(theEnv)->PrimaryTermArray[obji];

    ptPtr->fuzzy_value_description = (FUZZY_VALUE_HN *) SymbolData(theEnv)->FuzzyValueArray[bptPtr->fuzzyValue];
        IncrementFuzzyValueCount(ptPtr->fuzzy_value_description);

        if (bptPtr->next == -1L)
          ptPtr->next = NULL;
        else
          ptPtr->next = (struct primary_term *) &DeftemplateBinaryData(theEnv)->PrimaryTermArray[obji+1];
  }

#endif /* FUZZY_DEFTEMPLATES */

/************************************************/
/* UpdateDeftemplateSlot: Bload refresh routine */
/*   for templateSlot data structures.          */
/************************************************/
static void UpdateDeftemplateSlot(
  void *theEnv,
  void *buf,
  long obji)
  {
   struct templateSlot *theSlot;
   struct bsaveTemplateSlot *btsPtr;

   btsPtr = (struct bsaveTemplateSlot *) buf;
   theSlot = (struct templateSlot *) &DeftemplateBinaryData(theEnv)->SlotArray[obji];

   theSlot->slotName = SymbolPointer(btsPtr->slotName);
   IncrementSymbolCount(theSlot->slotName);
   theSlot->defaultList = HashedExpressionPointer(btsPtr->defaultList);
   theSlot->facetList = HashedExpressionPointer(btsPtr->facetList);
   theSlot->constraints = ConstraintPointer(btsPtr->constraints);

   theSlot->multislot = btsPtr->multislot;
   theSlot->noDefault = btsPtr->noDefault;
   theSlot->defaultPresent = btsPtr->defaultPresent;
   theSlot->defaultDynamic = btsPtr->defaultDynamic;

   if (btsPtr->next != -1L)
     { theSlot->next = (struct templateSlot *) &DeftemplateBinaryData(theEnv)->SlotArray[obji + 1]; }
   else
     { theSlot->next = NULL; }
  }

/*****************************************/
/* ClearBload: Deftemplate clear routine */
/*   when a binary load is in effect.    */
/*****************************************/
static void ClearBload(
  void *theEnv)
  {
   size_t space;
   int i;

   /*=============================================*/
   /* Decrement in use counters for atomic values */
   /* contained in the construct headers.         */
   /*=============================================*/

   for (i = 0; i < DeftemplateBinaryData(theEnv)->NumberOfDeftemplates; i++)
     { UnmarkConstructHeader(theEnv,&DeftemplateBinaryData(theEnv)->DeftemplateArray[i].header); }

   /*=======================================*/
   /* Decrement in use counters for symbols */
   /* used as slot names.                   */
   /*=======================================*/

   for (i = 0; i < DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots; i++)
     { DecrementSymbolCount(theEnv,DeftemplateBinaryData(theEnv)->SlotArray[i].slotName); }
     
#if FUZZY_DEFTEMPLATES   
   for (i = 0; i < DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates; i++)
     {
       if (DeftemplateBinaryData(theEnv)->LvPlusUniverseArray[i].units != NULL)
         DecrementSymbolCount(theEnv,DeftemplateBinaryData(theEnv)->LvPlusUniverseArray[i].units);
     }

   for (i = 0; i < DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms; i++)
     {
       DecrementFuzzyValueCount(theEnv,DeftemplateBinaryData(theEnv)->PrimaryTermArray[i].fuzzy_value_description);
     }
#endif

   /*======================================================================*/
   /* Deallocate the space used for the deftemplateModule data structures. */
   /*======================================================================*/

   space =  DeftemplateBinaryData(theEnv)->NumberOfTemplateModules * sizeof(struct deftemplateModule);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->ModuleArray,space);
   DeftemplateBinaryData(theEnv)->NumberOfTemplateModules = 0;
   
   /*================================================================*/
   /* Deallocate the space used for the deftemplate data structures. */
   /*================================================================*/

   space = DeftemplateBinaryData(theEnv)->NumberOfDeftemplates * sizeof(struct deftemplate);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->DeftemplateArray,space);
   DeftemplateBinaryData(theEnv)->NumberOfDeftemplates = 0;

#if FUZZY_DEFTEMPLATES  
   space = DeftemplateBinaryData(theEnv)->NumberOfFuzzyTemplates * sizeof(struct fuzzyLv);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->LvPlusUniverseArray,space);

   space = DeftemplateBinaryData(theEnv)->NumberOfFuzzyPrimaryTerms * sizeof(struct primary_term);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->PrimaryTermArray,space);
#endif

   /*=================================================================*/
   /* Deallocate the space used for the templateSlot data structures. */
   /*=================================================================*/

   space =  DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots * sizeof(struct templateSlot);
   if (space != 0) genfree(theEnv,(void *) DeftemplateBinaryData(theEnv)->SlotArray,space);
   DeftemplateBinaryData(theEnv)->NumberOfTemplateSlots = 0;
   
   /*======================================*/
   /* Create the initial-fact deftemplate. */
   /*======================================*/

#if (! BLOAD_ONLY)
   CreateImpliedDeftemplate(theEnv,(SYMBOL_HN *) EnvAddSymbol(theEnv,"initial-fact"),FALSE);
#endif
  }

/************************************************************/
/* BloadDeftemplateModuleReference: Returns the deftemplate */
/*   module pointer for use with the bload function.        */
/************************************************************/
globle void *BloadDeftemplateModuleReference(
  void *theEnv,
  int theIndex)
  {
   return ((void *) &DeftemplateBinaryData(theEnv)->ModuleArray[theIndex]);
  }

#endif /* DEFTEMPLATE_CONSTRUCT && (BLOAD || BLOAD_ONLY || BLOAD_AND_BSAVE) && (! RUN_TIME) */



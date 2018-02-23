  /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             CERTAINTY FACTORS MODULE                */
   /*******************************************************/

/*************************************************************/
/* Purpose:                                                  */
/*                                                           */
/* Principal Programmer(s):                                  */
/*      Gary D. Riley                                        */
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

#define _CFDEF_SOURCE_

#include "setup.h"

#if CERTAINTY_FACTORS

#include "symbol.h"
#include "cfdef.h"

#if FUZZY_DEFTEMPLATES
#include "fuzzyval.h"
#include "fuzzyutl.h"
#include "fuzzypsr.h"
#include "fuzzyrhs.h"
#endif  /* FUZZY_DEFTEMPLATES */

#include "prntutil.h"
#include "argacces.h"
#include "engine.h"
#include "router.h"
#include "extnfunc.h"
#include "exprnpsr.h"
#include "evaluatn.h"
#include "match.h"
#include "factmngr.h"
#include "constrct.h"

#include <math.h>
#include <stdio.h>
#define _CLIPS_STDIO_

/******************************************************************
    Local Internal Function Declarations
 ******************************************************************/
 
   static struct fact *getFactPtr(void *,struct expr *theArgument, char *functionName);

globle void InitializeCF(
  void *theEnv)
{
  AllocateEnvironmentData(theEnv,CF_DATA,sizeof(struct certaintyFactorData),NULL);
 /* Variables and functions for CF and threshold value of CF */
 CertaintyFactorData(theEnv)->Threshold_CF = 0.0;   /* by default set this to 0.0 */
 CertaintyFactorData(theEnv)->rule_cf_calculation = TRUE;

#if ! RUN_TIME  
 EnvDefineFunction2(theEnv,"get-cf", 'd', PTIEF getcf, "getcf", "11z");
 EnvDefineFunction2(theEnv,"threshold", 'v', PTIEF set_threshold, "set_threshold", "11n");
 EnvDefineFunction2(theEnv,"set-threshold", 'v', PTIEF set_threshold, "set_threshold", "11n");
 EnvDefineFunction2(theEnv,"get-threshold", 'd', PTIEF get_threshold, "get_threshold", "00");
 EnvDefineFunction2(theEnv,"enable-rule-cf-calculation", 'v', PTIEF enable_rule_cf_calculation, "enable_rule_cf_calculation", "00");
 EnvDefineFunction2(theEnv,"disable-rule-cf-calculation", 'v', PTIEF disable_rule_cf_calculation, "disable_rule_cf_calculation", "00");
#endif
}

/***********************************************************************
    FUNCTIONS FOR PARSING CERTAINTY FACTORS
 ***********************************************************************/

#if ! RUN_TIME

/****************************************************************/
/* ParseDeclareUncertainty     :                                */
/* Parses uncertainty factor for Declare of Rule                */
/*                                                              */
/****************************************************************/
globle struct expr *ParseDeclareUncertainty(
  void *theEnv,
  char *readSource,
  char *ruleName,
  int *error,
  double *cfVALUE)
  {
   double cf;
   DATA_OBJECT cfValueDO;
   struct expr *cfExpression;

   /*======================================*/
   /* Get the certainty factor expression. */
   /*======================================*/

   SavePPBuffer(theEnv," ");

   cfExpression = ParseAtomOrExpression(theEnv,readSource,NULL);
   if (cfExpression == NULL)
     {
      *error = TRUE;
      *cfVALUE = 1.0;
      return(NULL);
     }

   /*=====================================================================*/
   /* Evaluate the expression and determine if it is an integer or float. */
   /*=====================================================================*/

   SetEvaluationError(theEnv,FALSE);
   if (EvaluateExpression(theEnv,cfExpression,&cfValueDO))
     {
      cfInformationError(theEnv,ruleName);
      *error = TRUE;
      *cfVALUE = 1.0;
      return(cfExpression);
     }

   if ( cfValueDO.type != INTEGER && cfValueDO.type != FLOAT)
     {
      cfNonNumberError(theEnv);
      *error = TRUE;
      *cfVALUE = 1.0;
      return(cfExpression);
     }
     
   /*==========================================*/
   /* The expression is Integer or Float,      */
   /* check range (0 to 1) and if              */
   /* OK then set the value in cfVALUE         */
   /*==========================================*/

   cf = (cfValueDO.type == INTEGER) ? 
                   (double) ValueToLong(cfValueDO.value) :
                                ValueToDouble(cfValueDO.value);
       
   if ((cf > 1.0) || (cf < 0.0))
     {
       cfRangeError(theEnv);
       *error = TRUE;
       *cfVALUE = 1.0;
     }
   else
       *cfVALUE = cf;
   
   return(cfExpression);
}


#endif /*  ! RUN_TIME  */


/*****************************************************************/
/* printCF: prints certainty factor                              */
/*****************************************************************/  
globle VOID printCF(
  void *theEnv,
  char *logicalName,
  double cf)
  {
   char printSpace[20];
   
   sprintf(printSpace," CF %.2f ",cf);
   EnvPrintRouter(theEnv,logicalName,printSpace);
   
  }
  
  

/***********************************************************************
    FUNCTIONS FOR COMPUTING CF'S OF RHS
 ***********************************************************************/

#if FUZZY_DEFTEMPLATES

/* Similarity calcs only needed if there are fuzzy facts allowed */

/******************************************************************/
/* POSSIBILITY: possibility measure of two fuzzy sets             */
/*                                                                */
/* p(f1,f2) = max(min(u  (x),u  (x))                              */ 
/*             x       f1     f2                                  */
/******************************************************************/
globle double possibility(
  void *theEnv,
  struct fuzzy_value *f1, 
  struct fuzzy_value *f2)
  {        
    return( max_of_min (theEnv, f1, f2 ) );
  }



/********************************************************************/
/* NECESSITY; necessity measure of two fuzzy sets                   */
/*                   _                                              */
/* n(f1,f2) = 1 - p(f1,f2)                                          */
/********************************************************************/
globle double necessity(
  void *theEnv,
  struct fuzzy_value *f1, 
  struct fuzzy_value *f2)
  {
   struct fuzzy_value *fc;
   double nc;
   
   fc = CopyFuzzyValue(theEnv,f1);
   fcompliment(fc);
   
   nc = 1.0 - possibility(theEnv,fc,f2);
   rtnFuzzyValue(theEnv,fc);
   
   return(nc);
  }

/*****************************************************************/
/* SIMILARITY: similarity measure of two fuzzy sets              */
/* m = if    n(f1,f2) > 0.5                                      */
/*     then  p(f1,f2)                                            */
/*     else  (n(f1,f2) + 0.5) * p(f1,f2)                         */       
/*****************************************************************/
globle double similarity(
  void *theEnv,
  struct fuzzy_value *f1,
  struct fuzzy_value *f2)
  {
   double nec, poss;

   nec = necessity(theEnv,f1,f2);
   poss = possibility(theEnv,f1,f2);
   
   if (nec > 0.5)
     return( poss );
   else
     return( (nec + 0.5) * poss );
     
  }

#endif  /* FUZZY_DEFTEMPLATES */


/********************************************************************
    enable_rule_cf_calculation()

    Enables the calculation of the CF of facts generated on RHS of rule.
 ********************************************************************/
globle void enable_rule_cf_calculation(
  void *theEnv)
{
    if (EnvArgCountCheck(theEnv,"get-threshold",EXACTLY,0) == -1)
      {     
        SetEvaluationError(theEnv,TRUE);
      }
      
    CertaintyFactorData(theEnv)->rule_cf_calculation = TRUE;  
    return;
}


/********************************************************************
    disable_rule_cf_calculation()

    Disables the calculation of the CF of facts generated on RHS of rule.
    The CF will be the CF provided in the assert statement if any (else 1.0)
    if the Rule CF calculation is disabled.
 ********************************************************************/
globle void disable_rule_cf_calculation(
  void *theEnv)
{
    if (EnvArgCountCheck(theEnv,"get-threshold",EXACTLY,0) == -1)
      {     
        SetEvaluationError(theEnv,TRUE);
      }
      
    CertaintyFactorData(theEnv)->rule_cf_calculation = FALSE;  
    return;
}


/****************************************************************
    computeStdConclCF: computes certainty factor for RHS for use
                       by thresholding and for use in calculating 
                       CF for fuzzy facts or crisp facts in 
                       CRISP_ rules (ie. no fuzzy patterns on LHS)            

    Given a rule of the form:

    If A1 and A2 and ... and An then C

    This function returns the certainty factor of the conclusion:
    CFconc = min (CFf1, CFf2, ..., CFfn) * CFrule
    
              - where CFfi are the CFs of the facts matching the
                patterns on the LHS

 ****************************************************************/
globle double computeStdConclCF(
  void *theEnv,
  double ruleCF,
  struct partialMatch *binds)
{
   double StdCF;
   struct genericMatch *antecedent_binds;
   double fact_cf;
   struct fact *tmpFact;
   struct joinNode *jNode;
   int i;

   /* initialize StdCF to the maximum value -- 1.0 */
   StdCF = 1.0; 

   /* for each pattern in the list of patterns that matched facts 
      find the minimum CF
   */
   
   antecedent_binds = &(binds->binds[0]); 
   jNode = EngineData(theEnv)->ExecutingRule->lastJoin;

   /* We will walk through each fact that matched in the rule
      in the reverse order so that we can more easily check to 
      see if the pattern was a NOT pattern
   */ 
	for (i=(unsigned int)((binds->bcount)-1); i>=0; i--, jNode = jNode->lastLevel)
     {
       /* find the fact that matched a pattern */
       tmpFact = (struct fact *)(antecedent_binds[i].gm.theMatch->matchingItem);

       if (jNode->patternIsNegated) /* check for NOT pattern */
          continue;                 /* just treat as 1.0 CF */

       /* only facts have CF's associated with them -- so if 
          anything else has been matched (e.g. Object Instance)
          then just treat as if it had a CF of 1.0)
          At some later time we may want to associate CFs with
          Object Instances and this code will change.
       */
       if (tmpFact == NULL || tmpFact->factHeader.theInfo->base.type != FACT_ADDRESS)
          continue;

       fact_cf = tmpFact->factCF;
                    
       if (fact_cf < StdCF) /* keep the minimum */
               StdCF = fact_cf;

     } /* end   for (i=0; i<binds->count; i++)  */
  
  return( StdCF * ruleCF );
}

 
/****************************************************************
    computeFuzzyCrispConclCF: computes certainty factors for RHS 
                              for use in calculating CF for crisp
                              facts in FUZZY_CRISP rules             

    Given a rule of the form:

    If A1 and A2 and ... and An then C  [C is crsip conclusion]

    This function returns the certainty factor of the conclusion:
    CFconc = min (CFf1, CFf2, ..., CFfn) * CFrule
    
    where - CFfi is the certainty of the fact matching pattern i
            if the fact is CRISP and CFfi is the certainty of the
            fact matching pattern i * the similarity of the fuzzy
            set of the pattern and the fuzzy set of the matching
            fact if the fact is FUZZY

 ****************************************************************/
#if FUZZY_DEFTEMPLATES

globle double computeFuzzyCrispConclCF(
  void *theEnv,
  struct defrule *theRule,
  struct partialMatch *binds)
{
   double FuzzyCrispCF;
   struct fuzzy_value *fact_fv, *antecedent_fv;
   struct genericMatch *antecedent_binds;
   double fact_cf, theFactCF, simFactCF;
   struct fact *tmpFact;
   struct joinNode *jNode;
   int i;
   
   /* initialize FuzzyCrispCF to the maximum value -- 1.0 */
   FuzzyCrispCF = 1.0;
 
   /* for each pattern in the list of patterns that matched facts 
      find the minimum CF -- if fact is a fuzzy fact and
      FuzzyCrispCF_needed is TRUE then must also calculate
      similarity and multiply by fact's CF
   */
   
   antecedent_binds = &(binds->binds[0]); 
   jNode = theRule->lastJoin;

   /* We will walk through each fact that matched in the rule
      in the reverse order so that we can more easily check to 
      see if the pattern was a NOT pattern
   */ 
   for (i=(unsigned int)((binds->bcount)-1); i>=0; i--, jNode = jNode->lastLevel)
     {
       int j;

       /* find the fact that matched a pattern */
       tmpFact = (struct fact *)(antecedent_binds[i].gm.theMatch->matchingItem);

       if (jNode->patternIsNegated) /* check for NOT pattern */
          continue;                 /* just treat as 1.0 CF */

       /* only facts have CF's associated with them -- so if 
          anything else has been matched (e.g. Object Instance)
          then just treat as if it had a CF of 1.0)
          At some later time we may want to associate CFs with
          Object Instances and this code will change.
       */
       if (tmpFact == NULL || tmpFact->factHeader.theInfo->base.type != FACT_ADDRESS)
          continue;

       theFactCF = tmpFact->factCF; /* assigned CF of the fact */
       fact_cf = theFactCF;         /* CF of the fact after considering fuzzy values */
                    
       /* theRule->pattern_fv_arrayPtr is ptr to array of fuzzy value hashNode 
          ptrs. Each of these ptrs points to a fuzzy value connected to a 
          fuzzy pattern and slot within that pattern in the rule's LHS.
          NOTE!! if there are no fuzzy patterns on the LHS of the rule then
          there are NO entries in this array and numberOfPatterns == 0
       */
           
       /* Since this is a FuzzyCrisp rule numberOfFuzzySlots should be > 0!
          This function and computeStdConclCF could possibly be combined??
       */
       if (theRule->numberOfFuzzySlots > 0)
         {
          /* search for any fuzzy slots in this pattern and do a similarity
             calculation for each one against the matching fact slot -- if 
             less than the current FuzzyCrispCF use that similarity value
          */
          for (j=0; j<theRule->numberOfFuzzySlots; j++)
             {
               int slotNum, patternNum;
               struct fzSlotLocator *patFvArrayPtrj = theRule->pattern_fv_arrayPtr + j;

               patternNum = patFvArrayPtrj->patternNum;

               if (patternNum > i) break; /* past FV's for this pattern */

               if (patternNum == i) 
                  {
                    /* this pattern has a fuzzy slot */
                    slotNum = patFvArrayPtrj->slotNum;
                    /* get FV for pattern and the matching FV in the fact */
                    antecedent_fv = (struct fuzzy_value *) 
                            ValueToFuzzyValue(patFvArrayPtrj->fvhnPtr); 
                    
                    fact_fv = (struct fuzzy_value *)
                        ValueToFuzzyValue((tmpFact->theProposition.theFields[slotNum].value));
                   
                    simFactCF = similarity(theEnv,antecedent_fv,fact_fv) * theFactCF;

                    /* keep the minimum of CFs over all fuzzy values in the fact */
                    if (simFactCF < fact_cf) 
                       fact_cf = simFactCF;
                  }
          
             } /* end of   for (j=0; ...)  */
         }
    
       if (fact_cf < FuzzyCrispCF) /* keep the minimum CF of all facts */
          FuzzyCrispCF = fact_cf;

     } /* end   for (i=0; i<binds->count; i++)  */

  return( FuzzyCrispCF * theRule->CF );
}



#endif /* FUZZY_DEFTEMPLATES */
 

/********************************************************************
    changeCFofNewFact()

    If the new fact is being asserted from the command line
    or the rule CF calculation is disabled,
    then the cf is not altered (no rule is executing). Also if
    during a reset the cf is not altered.

    If the new fact is being asserted from the RHS of a rule, then
    the cf of the new fact is multiplied by the cf calculated for
    the whole rule -- depends on whether the asserted fact is Crisp
    or fuzzy -- if crisp and LHS had Fuzzy patterns then use 
    FuzzyCrispConcludingCF of Executing Rule else use
    StdConcludingCF of Executing Rule (both values are stored in the 
    current rule activation - if their value is -1.0 then they have
    not yet been calculated and should now be calculated )
 ********************************************************************/
 
globle VOID changeCFofNewFact(
  void *theEnv,
  struct fact *newFact)
  {
    double ConcludingCFofExecutingRule;

    if ((CertaintyFactorData(theEnv)->rule_cf_calculation) &&
        (EngineData(theEnv)->ExecutingRule != NULL) &&
        (EngineData(theEnv)->ExecutingRule->executing) &&
        (! ConstructData(theEnv)->ResetInProgress)
       ) 
      {
        /* Fuzzy facts always use StdConcludingCF -- Crisp Facts do too
           when NO fuzzy facts in rule
        */
#if FUZZY_DEFTEMPLATES
        if (newFact->whichDeftemplate->hasFuzzySlots)
           {
#endif
             ConcludingCFofExecutingRule = EngineData(theEnv)->TheCurrentActivation->StdConcludingCF;
             if (ConcludingCFofExecutingRule < 0) /* not yet calculated if -1.0 */
                EngineData(theEnv)->TheCurrentActivation->StdConcludingCF
                    = ConcludingCFofExecutingRule
                    = computeStdConclCF(theEnv,EngineData(theEnv)->TheCurrentActivation->CF,EngineData(theEnv)->TheCurrentActivation->basis);
#if FUZZY_DEFTEMPLATES
           }
        else
           { 
             /* Crisp facts use StdConcludingCF if LHS of Rule is Crisp and
                use FuzzyCrispConcludingCF when fuzzy patterns on LHS of rule
             */
             if (EngineData(theEnv)->ExecutingRule->lhsRuleType == FUZZY_LHS)
                {
                  ConcludingCFofExecutingRule = EngineData(theEnv)->TheCurrentActivation->FuzzyCrispConcludingCF;
                  if (ConcludingCFofExecutingRule < 0) /* not yet calculated if -1.0 */
                     EngineData(theEnv)->TheCurrentActivation->FuzzyCrispConcludingCF
                         = ConcludingCFofExecutingRule
                         = computeFuzzyCrispConclCF(theEnv,EngineData(theEnv)->ExecutingRule, EngineData(theEnv)->TheCurrentActivation->basis);
                }
             else
                {
                  ConcludingCFofExecutingRule = EngineData(theEnv)->TheCurrentActivation->StdConcludingCF;
                  if (ConcludingCFofExecutingRule < 0) /* not yet calculated if -1.0 */
                     EngineData(theEnv)->TheCurrentActivation->StdConcludingCF
                         = ConcludingCFofExecutingRule
                         = computeStdConclCF(theEnv,EngineData(theEnv)->TheCurrentActivation->CF, EngineData(theEnv)->TheCurrentActivation->basis);
                }
           }
#endif
        newFact->factCF = newFact->factCF * ConcludingCFofExecutingRule;
      }

  }

/******************************************************************
    Functions for accessing cf of a fact
 ******************************************************************/


/************************************************************
    getFactPtr():                                            

    given a ptr to an argument of a function that is expected
    to be a fact address or a fact index get a ptr to the fact
    
    returns a ptr to a fact or NULL if error occurred
************************************************************/
static struct fact *getFactPtr(
  void *theEnv,
  struct expr *theArgument,
  char *functionName)
{
   long int factIndex;
   int found_fact;
   DATA_OBJECT  theResult;
   struct fact *factPtr;

	EvaluateExpression(theEnv,theArgument,&theResult);

	if ((theResult.type == INTEGER) || (theResult.type == FACT_ADDRESS))
     {
       if (theResult.type == INTEGER)
         { 
			  factIndex = ValueToLong(theResult.value);
			  if (factIndex < 0)
             {            
               ExpectedTypeError1(theEnv,functionName,1,"fact-index must be positive");
               return(NULL);
             }
           found_fact = FALSE;
           factPtr = (struct fact *) EnvGetNextFact(theEnv,NULL);
           while (factPtr != NULL)
             {
               if (factPtr->factIndex == factIndex)
                 {
                   found_fact = TRUE;
                   break;
                 }
               factPtr = factPtr->nextFact;
             }
           
           if (found_fact == FALSE)
             {
               char tempBuffer[20];
               sprintf(tempBuffer,"f-%ld",factIndex);
               CantFindItemErrorMessage(theEnv,"fact",tempBuffer);
               return(NULL);
             }
          }
        else
          { /* arg type is fact address */
            factPtr = (struct fact *) theResult.value; 
          }     
        return( factPtr );
     }
   
   ExpectedTypeError1(theEnv,functionName,1,"fact-index or fact-address");
   return( NULL );
}


/************************************************************
    getcf():                                            

    returns the certainty factor of a single fact in
    NUMBER format; if the certainty factor is a TFN,
    the peak value is returned.
 ************************************************************/
globle double getcf(
  void *theEnv)
  {
    struct fact *factPtr;
    struct expr *theArgument;

	 if (EnvArgCountCheck(theEnv,"get-cf",EXACTLY,1) != -1)
      {     
		  theArgument = GetFirstArgument();
        if (theArgument != NULL)
          {
            factPtr = getFactPtr(theEnv,theArgument, "get-cf");
       
            if (factPtr != NULL)
              {
                return(factPtr->factCF);
              }
          }
		}
        
    SetEvaluationError(theEnv,TRUE);
	 return(0.0);
  }



/*******************************************************************
    Functions for setting and accessing the threshold cf value
 *******************************************************************/

/*******************************************************************
    set_threshold()
    
    Sets the threshold cf to desired CRISP value and changes
    threshold_on to TRUE.
 *******************************************************************/
globle VOID set_threshold(
  void *theEnv)
  {
    DATA_OBJECT theArgument;
    double theThreshold;

    if (EnvArgCountCheck(theEnv,"set-threshold",EXACTLY,1) != -1)
      {     
        if (EnvArgTypeCheck(theEnv,"set-threshold",1,INTEGER_OR_FLOAT,&theArgument) != 0)
          {
            if (GetType(theArgument) == INTEGER)
              {
                theThreshold = (double)DOToLong(theArgument);
              }
            else
              {
                theThreshold = DOToDouble(theArgument);
              }
            if (theThreshold < 0.0 || theThreshold > 1.0)
              {
                cfRangeError(theEnv);
              }
            else
              {
                CertaintyFactorData(theEnv)->Threshold_CF = theThreshold;
                return;
              }
          }
      }
        
    SetEvaluationError(theEnv,TRUE);
  }


/********************************************************************
    get_threshold()

    Returns the CRISP threshold value.
 ********************************************************************/
globle double get_threshold(
  void *theEnv)
{
    if (EnvArgCountCheck(theEnv,"get-threshold",EXACTLY,0) == -1)
      {     
        SetEvaluationError(theEnv,TRUE);
      }
        
    return( CertaintyFactorData(theEnv)->Threshold_CF );
}




/*******************************************************************
    FUNCTIONS FOR CHANGING CF'S DUE TO
    GLOBAL CONTRIBUTION
 *******************************************************************/

/*******************************************************************
    changeCFofExistingFact(newFact, oldFact)

    where: oldFact is the existing fact on the fact list
           newFact is the new version of fact2 being asserted

    The purpose of this function is to update the certainty factor
    value of the existing fact when an identical fact is asserted
    (global contribution).  

    The new certainty factor is the maximum cf of the two facts.


 *******************************************************************/
globle VOID changeCFofExistingFact(
  void *theEnv,
  struct fact *newFact,
  struct fact *oldFact)
  {
    if (oldFact->factCF < newFact->factCF)
      {
        oldFact->factCF = newFact->factCF;
           
        /* fact has changed - set flag to say so */
        EnvSetFactListChanged(theEnv,TRUE);

#if DEBUGGING_FUNCTIONS
        if (oldFact->whichDeftemplate->watch)
          {
            EnvPrintRouter(theEnv,WTRACE,"~CF ");
            PrintFactWithIdentifier(theEnv,WTRACE,oldFact);
            EnvPrintRouter(theEnv,WTRACE,"\n");
          }
#endif
      }
  }



/*******************************************************************
    changeCFofNewVsExistingFact(newFact, oldFact)

    where: oldFact is the existing fact on the fact list
           newFact is the new version of fact being asserted

    The purpose of this function is to update the certainty factor
    value of the new fact when an identical fact already exists
    (global contribution).  

    The new certainty factor is the maximum cf of the two facts.


 *******************************************************************/
globle VOID changeCFofNewVsExistingFact(
  struct fact *newFact,
  struct fact *oldFact)
  {
    if (newFact->factCF < oldFact->factCF)
      {
        newFact->factCF = oldFact->factCF;
      }
  }






/*******************************************************************
    FUNCTIONS FOR Printing Error Messages for CFs
    
 *******************************************************************/

globle void cfInformationError(
  void *theEnv,
  char *name)
{
   PrintErrorID(theEnv,"Certainty Factors ",901,TRUE);
   EnvPrintRouter(theEnv,WERROR,"This error occurred while evaluating a Certainty Factor");
   if (name != NULL)
     {
      EnvPrintRouter(theEnv,WERROR," for rule ");
      EnvPrintRouter(theEnv,WERROR,name);
     }
   EnvPrintRouter(theEnv,WERROR,".\n");
}


globle VOID   cfRangeError(
  void *theEnv)

{
   PrintErrorID(theEnv,"Certainty Factors ",902,TRUE);
   EnvPrintRouter(theEnv,WERROR,"Expected Value in Range 0.0 to 1.0");
   EnvPrintRouter(theEnv,WERROR,".\n");
}


globle VOID   cfNonNumberError(
  void *theEnv)

{
   PrintErrorID(theEnv,"Certainty Factors ",903,TRUE);
   EnvPrintRouter(theEnv,WERROR,"Expected Integer or Float Value");
   EnvPrintRouter(theEnv,WERROR,".\n");
}






#endif  /* CERTAINTY_FACTORS */

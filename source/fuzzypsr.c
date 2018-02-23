   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             FUZZY REASONING PARSER MODULE           */
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


/******************************************************************
    Fuzzy Logic Module

    This file contains the following categories of functions:
    
    parsing of fuzzy value definition in deftemplate statement

    (has Get_S_Z_or_PI_FuzzyValue which is used in fuzzyrhs.c)

    General calling of major routines is:

       ParseFuzzyTemplate ---
                            |
                            |---> parseUniverse
                            |
                            |---> parsePrimaryTermList
                                       |
                                       |
                                      \ /
                                  parseTemplateFuzzyValue
                                       |
                                       |
                                       |---> parseStandardFuzzyValue
                                       |
                                       |
                                       |---> parseSingletonFuzzyValue
                                       |
                                       |
                                       |---> ParseLinguisticExpr

 ******************************************************************/
 
#define _FUZZYPSR_SOURCE_

#include <string.h>


#include "setup.h"


#if FUZZY_DEFTEMPLATES

#include <stdio.h>
#define _CLIPS_STDIO_

#include "extnfunc.h"
#include "prntutil.h"
#include "scanner.h"
#include "constant.h"
#include "pprint.h"
#include "symbol.h"
#include "memalloc.h"
#include "dffnxfun.h"
#include "exprnops.h"
#include "modulutl.h"
#include "moduldef.h"
#include "evaluatn.h"

#include "fuzzydef.h"
#include "fuzzypsr.h"
#include "fuzzyrhs.h"
#include "fuzzyval.h"
#include "fuzzylv.h"
#include "fuzzyutl.h"

/******************************************************************
    Global Internal Function Declarations
    
    Defined in fuzzypsr.h
 ******************************************************************/



/******************************************************************
    Local Internal Function Declarations
 ******************************************************************/
 

#if (! RUN_TIME) && (! BLOAD_ONLY)
  static struct fuzzyLv       *parseUniverse(void *theEnv,char *read_source, struct token *inputToken, 
                                             int *DeftemplateError);
  static void                  parsePrimaryTermList(void *theEnv,char *in_file, struct token *inputToken, 
                                             int *DeftemplateError, struct fuzzyLv *u);
  static struct primary_term  *parsePrimaryTerm(void *theEnv,char *readSource, 
                                                struct token *inputToken, int *DeftemplateError,
                                                struct fuzzyLv *u);
  static struct fuzzy_value   *parseTemplateFuzzyValue(void *theEnv,char *readSource, struct token *inputToken, 
                                             int  *DeftemplateError, struct fuzzyLv *u);
  static struct fuzzy_value   *parseSingletonFuzzyValue(void *theEnv,char *readSource, 
                                                      struct token *inputToken,
                                                      int  *DeftemplateError,
                                                      struct fuzzyLv *u);
  static struct fuzzy_value   *parseStandardFuzzyValue(void *theEnv,char *readSource, 
                                                     struct token *inputToken,
                                                     int  *DeftemplateError,
                                                     struct fuzzyLv *u);
#endif
  static void                  rtnPrimaryTermList(void *theEnv,struct primary_term *pt);


/******************************************************************
    Local Internal Variable Declarations
 ******************************************************************/ 

   
   
/******************************************************************
    Global External Variable Declarations
 ******************************************************************/
  


/******************************************************************
    Global Internal Variable Declarations
 ******************************************************************/


/**********************************************************************
    FUNCTIONS to Initialize arrays for the S, Z and PI fuzzy functions
 **********************************************************************/


 
/*********************** !!!!!!!!!!!!!!!!!!!!!! **************

 NOTE: 
 
 PI function is s(x, gamma-beta, gamma-beta/2.0, gamma) if x < gamma
            and 1.0 - s(x, gamma, gamma+beta/2,0, gamma+beta) if x > gamma
            and 1.0 if x = gamma
           
 Z function is 1.0 - s(x, alpha, beta, gamma)

**************************************************************/
        


/***********************************/
/* S - Standard_function:          */
/***********************************/
globle double sFunction(
  double x,
  double alfa,
  double beta,
  double gamma)
  {
   register double gMinusa = gamma - alfa;
   register double gMinusaSqr = gMinusa * gMinusa;
   
   if (x <= alfa)
     { return(0.0); }
   else if (x <= beta)
     { return(2.0*(x-alfa)*(x-alfa)/(gMinusaSqr)); }
   else if (x < gamma)
     { return(1.0 - 2.0*(x-gamma)*(x-gamma)/(gMinusaSqr)); }
   else
     { return(1.0); }     
  }



/**********************************************************/
/* Init_S_Z_PI_yvalues:                                   */
/*                                                        */
/* We can do this in advance since the y values for any   */
/* S, Z, or PI function is the same due to the nature of  */
/* the functions. Just use a 0 to 1 range n calculations  */
/**********************************************************/

globle void Init_S_Z_PI_yvalues(
   void *theEnv)
   {
     int i;
     int ArraySzBy2 = ArraySIZE/2;
     int ArraySzMinus1 = ArraySIZE - 1;
     
     /* PI y values stored in PI_array */
     
     FuzzyData(theEnv)->PI_array[0] = 0.0;
     FuzzyData(theEnv)->PI_array[ArraySzBy2] = 1.0;
     FuzzyData(theEnv)->PI_array[ArraySzMinus1] = 0.0;
     
     for ( i=1; i < ArraySzBy2; i++)
        {
          FuzzyData(theEnv)->PI_array[i] = sFunction((double)i/(double)ArraySzBy2, 0.0, 0.5, 1.0);
        }
     for ( i = ArraySzBy2+1; i < ArraySzMinus1; i++)
        {
          FuzzyData(theEnv)->PI_array[i] = FuzzyData(theEnv)->PI_array[ArraySzMinus1-i]; /* symmetry let's us do this */
        }
        
     /* S y values stored in S_array */
     
     FuzzyData(theEnv)->S_array[0] = 0.0;
     FuzzyData(theEnv)->S_array[ArraySzMinus1] = 1.0;
     
     for ( i=1; i < ArraySzMinus1; i++)
        {
          FuzzyData(theEnv)->S_array[i] = sFunction((double)i/(double)(ArraySzMinus1), 0.0, 0.5, 1.0);
        }     
     
     /* Z y values stored in Z_array */

     for ( i=0; i < ArraySIZE; i++)
        {
          FuzzyData(theEnv)->Z_array[i] = FuzzyData(theEnv)->S_array[ArraySzMinus1-i]; /* symmetry of the functions */
        }     
   }



/**********************************************************/
/* Get_S_Z_or_PI_FuzzyValue:                              */
/*                                                        */
/* Depending on the type of function requested get the    */
/* x and y values for the S, PI or Z function             */
/**********************************************************/

globle struct fuzzy_value *Get_S_Z_or_PI_FuzzyValue(
  void *theEnv,
  double alfa,
  double beta,
  double gamma,
  int   function_type)
{
    struct fuzzy_value *fv;
    double h, deltah;
    double gPlusb;
    double gMinusb;
    int i;
    
    /* Construct the fuzzy value */

    fv = get_struct(theEnv,fuzzy_value);

    fv->whichDeftemplate = NULL;
    fv->name = (char *) gm2(theEnv,4);
    strcpy(fv->name, "???");

    /* note: for PI function beta may be 0 -- return just 3 points
             for S, Z functions alfa may equal gamma -- return 2 points
    */
    if (function_type == PI_FUNCTION && beta == 0.0)
       fv->maxn = 3;
    else if (function_type != PI_FUNCTION && alfa == gamma)
       fv->maxn = 2;
    else
       fv->maxn = ArraySIZE;

    fv->n = fv->maxn;
    
    fv->x = FgetArray( theEnv,fv->maxn );
    fv->y = FgetArray( theEnv,fv->maxn );
              
    if  (function_type == PI_FUNCTION)
      {
        gPlusb = gamma + beta;
        gMinusb = gamma - beta;
              
        if (beta == 0.0) /* (PI 0 gamma) ? */
          {
           fv->x[0] = fv->x[1] = fv->x[2] = gamma;
           fv->y[0] = fv->y[2] = 0.0;
           fv->y[1] = 1.0;
          }
        else
          {
           deltah = beta / (double)(ArraySIZE/2);
           fv->x[0] = gMinusb;
           fv->y[0] = 0.0;
           fv->x[ArraySIZE/2] = gamma;
           fv->y[ArraySIZE/2] = 1.0;
           fv->x[ArraySIZE-1] = gPlusb;
           fv->y[ArraySIZE-1] = 0.0;
                 
           for ( i=1, h=deltah; i < ArraySIZE/2; i++)
            {
             fv->x[i] = gMinusb + h;
             fv->y[i] = FuzzyData(theEnv)->PI_array[i];
             h = h + deltah;
            }
           for ( i = ArraySIZE/2+1, h = deltah; i < ArraySIZE-1; i++)
            {
             fv->x[i] = gamma + h;
             fv->y[i] = FuzzyData(theEnv)->PI_array[i];
             h = h + deltah;
            }
          }
      }
    else
      { /* s or z function */
        double h, deltah;
        
        if (alfa == gamma) /* (S x x) or (Z x x) ? */
          {
           fv->x[0] = fv->x[1] = alfa;
           if (function_type == S_FUNCTION)
             {
              fv->y[0] = 0.0; fv->y[1] = 1.0;
             }
           else
             {
              fv->y[0] = 1.0; fv->y[1] = 0.0;
             }
          }
        else
          {
           fv->x[0] = alfa;
           fv->x[8] = gamma;
           if (function_type == S_FUNCTION)
             { fv->y[0] = 0.0; fv->y[ArraySIZE-1] = 1.0; }
           else
             { fv->y[0] = 1.0; fv->y[ArraySIZE-1] = 0.0; }
        
           deltah = (gamma - alfa) / (double)(ArraySIZE-1);
           for (i=1, h = deltah; i < ArraySIZE-1; i++)
             {
              fv->x[i] = alfa + h;
              fv->y[i] = (function_type == S_FUNCTION) ? FuzzyData(theEnv)->S_array[i] :
                                                        FuzzyData(theEnv)->Z_array[i];
              h = h + deltah;
             }
          }
       }
    
    return (fv);

}


#if (! RUN_TIME) && (! BLOAD_ONLY)

/********************************************************************
    FUNCTIONS THAT PARSE A FUZZY DEFTEMPLATE 
 ********************************************************************/
 
/*****************************************************************/
/* ParseFuzzyTemplate:  The purpose of this function is to parse */
/*   the deftemplate statement for fuzzy variables.              */ 
/* The list of primary terms and a list of modifiers and the     */
/* universe of discourse are saved.                              */
/*****************************************************************/
globle struct fuzzyLv *ParseFuzzyTemplate(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  int *DeftemplateError)
  {
   struct fuzzyLv      *new_lv  = NULL;


   /*========================================*/
   /* Parse the universe of discourse        */
   /*========================================*/

   if (inputToken->type == FLOAT || inputToken->type == INTEGER)  
     {
      new_lv = parseUniverse(theEnv,readSource, inputToken, DeftemplateError);
      if (*DeftemplateError == TRUE) 
          { 
            return(NULL); 
          }
     }

   /*========================================*/
   /* Check that next token is a '('.        */
   /*========================================*/
   
   if (inputToken->type != LPAREN) 
     {
      SyntaxErrorMessage(theEnv,"Deftemplate (Expecting Fuzzy Term List)");
      *DeftemplateError = TRUE;
      /* need to return the fuzzyLv struct */
      rtn_struct(theEnv,fuzzyLv, new_lv);
      return(NULL);
     }
   else
     {
      parsePrimaryTermList(theEnv,readSource, inputToken, DeftemplateError, new_lv);
      if (*DeftemplateError == TRUE) 
           { 
             /* need to return the universe of discourse struct */
             rtn_struct(theEnv,fuzzyLv, new_lv);
             return(NULL); 
           }
     }

   GetToken(theEnv,readSource,inputToken);

   if (inputToken->type != RPAREN)
     {
      SyntaxErrorMessage(theEnv,"Deftemplate (Closing ')' for deftemplate expected)");
      *DeftemplateError = TRUE;
      /* must return universe of discourse and primary term structs */
      rtnPrimaryTermList(theEnv,new_lv->primary_term_list);
      rtn_struct(theEnv,fuzzyLv, new_lv);
      return(NULL);
     }

   PPBackup(theEnv);       
   SavePPBuffer(theEnv,"\n)\n");

   return(new_lv);       
  }


/*****************************************************/
/* parseUniverse:                                    */
/*                                                   */
/*  Must return a NULL ptr if error parsing universe */
/*  Must free any allocated structs if error         */
/*                                                   */
/*****************************************************/
static struct fuzzyLv *parseUniverse(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  int *DeftemplateError)
  {
   double f, t;       /* from and to values */
   struct fuzzyLv *u; /* universe of discourse is in fuzzyLv struct */
   
   SavePPBuffer(theEnv," ");
   
   /* inputToken has an integer or a float at this point */
   f = (inputToken->type == FLOAT) ? ValueToDouble(inputToken->value) : 
                                        (double)ValueToInteger(inputToken->value);
   GetToken(theEnv,readSource,inputToken);
   if (inputToken->type != FLOAT && inputToken->type != INTEGER) 
     {
      SyntaxErrorMessage(theEnv,"Deftemplate: Number Expected ('to' part of Universe)");
      *DeftemplateError = TRUE;
      return(NULL);
     }
   
   t = (inputToken->type == FLOAT) ? ValueToDouble(inputToken->value) : 
                                        (double)ValueToInteger(inputToken->value);
   if (f > t) 
     {
      SyntaxErrorMessage(theEnv,"Deftemplate: Invalid interval for Universe of Discourse");
      *DeftemplateError = TRUE;
      return(NULL);
     }
     
   u = get_struct(theEnv,fuzzyLv);
   u->from = f;
   u->to = t;
   u->units = NULL;
   u->primary_term_list = NULL;
   
   SavePPBuffer(theEnv," ");
   GetToken(theEnv,readSource,inputToken);
   if ((inputToken->type == STRING) || (inputToken->type ==  SYMBOL))
     {
       u->units = (SYMBOL_HN *) inputToken->value;
       PPCRAndIndent(theEnv);
       SavePPBuffer(theEnv," ");
       GetToken(theEnv,readSource,inputToken);
     }
   else
     {
       PPBackup(theEnv);
       PPCRAndIndent(theEnv);
       SavePPBuffer(theEnv," ");
       SavePPBuffer(theEnv,inputToken->printForm);
     }
   return(u);
  }


/*****************************************************/
/* parsePrimaryTermList:                             */
/*                                                   */
/*        PRIMARY TERMS                              */
/*  Must free any allocated structs if error parsing */
/*  Already have the opening '(' of the list         */
/*  Should eat up the closing ')' of list            */
/*****************************************************/
static void parsePrimaryTermList(
  void *theEnv,
  char *readSource,
  struct token *inputToken,  
  int  *DeftemplateError,
  struct fuzzyLv *new_lv)
  {
   struct primary_term *last_one, *next_one, *assert_list;

   last_one = assert_list = NULL;
   
   /* expect to see a set of terms, each enclosed in ()  */
    while ((next_one = parsePrimaryTerm(theEnv,readSource, inputToken,
                                   DeftemplateError, new_lv)) != NULL)
     {        
      /* make sure not defining same term a second time! */
      { char *thisName;
        struct primary_term *ptPtr = assert_list;

        thisName  = (ValueToFuzzyValue(next_one->fuzzy_value_description))->name;

        while (ptPtr != NULL)
          {
            if (strcmp(thisName,(ValueToFuzzyValue(ptPtr->fuzzy_value_description))->name) == 0)
              {
                *DeftemplateError = TRUE;
                SyntaxErrorMessage(theEnv,"Deftemplate (duplicate TERM being defined)");
                rtn_struct(theEnv,primary_term,next_one);
                rtnPrimaryTermList(theEnv,assert_list);                
                return;
              }
            ptPtr = ptPtr->next;
          }
       }

      if (last_one == NULL)
        { assert_list = next_one; 
          new_lv->primary_term_list = assert_list;
        }
      else
        { last_one->next = next_one; }
      last_one = next_one;     
     }
          
   if (*DeftemplateError || (assert_list == NULL))
     { /* need to free any allocated primary term structs */
       if (assert_list == NULL)
          {
            *DeftemplateError = TRUE;
            SyntaxErrorMessage(theEnv,"Deftemplate (At least one primary term must be defined)");
          }
       else
          { /* need to free any allocated primary term structs */
            rtnPrimaryTermList(theEnv,assert_list);
            new_lv->primary_term_list = NULL;
          }
     }

  }
  
/***************************************************************/
/* parsePrimaryTerm:                                           */
/* Returns NULL if ) is first token.                           */
/* Returns primary term.                                       */
/* DeftemplateError flag is set to true if an error occurs.    */
/***************************************************************/
static struct primary_term *parsePrimaryTerm(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  int *DeftemplateError,
  struct fuzzyLv *new_lv) 
{
   struct primary_term *ptr;
   char *pt_name;
   struct fuzzy_value *fuzzy_value_dsc;
   
   /*==========================================================*/
   /* Get the opening parenthesis of the primary term pattern. */
   /*==========================================================*/
   
   GetToken(theEnv,readSource,inputToken);
      
   if (inputToken->type == RPAREN) 
     {
       PPBackup(theEnv);
       PPBackup(theEnv);
       SavePPBuffer(theEnv," )");
       return(NULL);
     }
      
   if (inputToken->type != LPAREN)
     {
       SyntaxErrorMessage(theEnv,"Deftemplate (Expected primary term)");
       *DeftemplateError = TRUE;
       return(NULL);
     }
   
   /*================================================================*/
   /* Get the primary term name.                                     */
   /*================================================================*/
     
   GetToken(theEnv,readSource,inputToken);
   
   if (inputToken->type == SYMBOL)
     {
      pt_name = ((SYMBOL_HN *)inputToken->value)->contents;
     }
   else 
     {
      SyntaxErrorMessage(theEnv,"Deftemplate (Expected primary term name)");
      *DeftemplateError = TRUE;
      return(NULL);
     }
     
   /*====================================*/
   /* Get description of the fuzzy value */
   /*====================================*/
   
   SavePPBuffer(theEnv," ");
   fuzzy_value_dsc = parseTemplateFuzzyValue(theEnv,readSource,inputToken,DeftemplateError,new_lv);

   if (fuzzy_value_dsc != NULL && inputToken->type == RPAREN)
     {
      ptr = get_struct(theEnv,primary_term);
      if (fuzzy_value_dsc->name != NULL) rm(theEnv,fuzzy_value_dsc->name, strlen(fuzzy_value_dsc->name)+1);
      fuzzy_value_dsc->name = (char *) gm2(theEnv,strlen(pt_name)+1);
      strcpy(fuzzy_value_dsc->name, pt_name);
      ptr->fuzzy_value_description = (FUZZY_VALUE_HN *)AddFuzzyValue(theEnv,fuzzy_value_dsc);
      /* AddFuzzyValue makes a copy of the Fuzzy Value so we need to return this one */
      rtnFuzzyValue(theEnv,fuzzy_value_dsc);
      ptr->next = NULL;
      PPCRAndIndent(theEnv);
      SavePPBuffer(theEnv,"  ");
      return(ptr);
     }
   else
     {
      *DeftemplateError = TRUE;
      rtnFuzzyValue(theEnv,fuzzy_value_dsc);
      SyntaxErrorMessage(theEnv,"Deftemplate (expected ')' )");
      return(NULL);
     }
  }

/***********************************/    
/* parseTemplateFuzzyValue:        */
/***********************************/
static struct fuzzy_value *parseTemplateFuzzyValue(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  int  *DeftemplateError,
  struct fuzzyLv *new_lv)
{
   struct fuzzy_value *fv_ptr = NULL;

      GetToken(theEnv,readSource,inputToken);
      
      if (inputToken->type == LPAREN)
         {
           if (new_lv == NULL)
              {
                *DeftemplateError = TRUE;
                SyntaxErrorMessage(theEnv,"Deftemplate (Missing universe of discourse description)");
                return(NULL);
              }        
           GetToken(theEnv,readSource,inputToken);
           if (inputToken->type == FLOAT || inputToken->type == INTEGER)
              { fv_ptr = parseSingletonFuzzyValue(theEnv,readSource,inputToken,DeftemplateError,new_lv); }
           else
              { fv_ptr = parseStandardFuzzyValue(theEnv,readSource,inputToken,DeftemplateError,new_lv); }
         }  
      else
        {
          /* Could be a fuzzy linguistic expression made up of modifiers and
             any terms defined to this point in the deftemplate definition
          */
          if (new_lv == NULL || new_lv->primary_term_list == NULL)
            {
              *DeftemplateError = TRUE;
              SyntaxErrorMessage(theEnv,"Deftemplate (Expecting linguistic expression and no terms defined");
            }
          else
            {
              fv_ptr = ParseLinguisticExpr(theEnv,readSource,inputToken,new_lv,DeftemplateError);
              if (*DeftemplateError == TRUE)
                 SyntaxErrorMessage(theEnv,"Deftemplate (Fuzzy set description or linguistic expression expected)");
            }
        }
        
      if (*DeftemplateError == TRUE)
        {
          rtnFuzzyValue(theEnv,fv_ptr);
          return(NULL);
        }
      else     
        { return(fv_ptr); }
  }


/******************************************************************/    
/* parseSingletonFuzzyValue:                                      */
/*                                                                */
/* NOTE: this function is very similar to assertParseSingletonSet */
/*       routine found in fuzzyrhs.c -- if change this then check */
/*       that routine to see if similar changes are needed!!!     */
/******************************************************************/    

static struct fuzzy_value *parseSingletonFuzzyValue(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  int  *DeftemplateError,
  struct fuzzyLv *u)
  {
   
    struct fuzzy_value *fv;
    double  previous;
    int i, count, maxlength = 10, increment = 10;
    double newx, newy, *tempx, *tempy;
    double xtolerance;

    fv = get_struct(theEnv,fuzzy_value);
    fv->whichDeftemplate = NULL;
    fv->name = NULL;
    fv->x = FgetArray ( theEnv,maxlength );
    fv->y = FgetArray ( theEnv,maxlength );
    fv->maxn = maxlength;
      
    /* We want to allow specifications for Standard functions and singleton sets
       to NOT be thrown out with an error message due to a floating point
       representation problem so we will allow the points to be considered
       within the universe of discourse (range) if very close.

       FUZZY_TOLERANCE is defined in constant.h
    */
    xtolerance = ((u->to - u->from) >= 1.0) ? FUZZY_TOLERANCE : (u->to - u->from) * FUZZY_TOLERANCE;

    previous =  u->from - 1.0;
    count = 0;
    
    while (inputToken->type == FLOAT || inputToken->type == INTEGER)
         {
          /************************************************
           Token should be x coordinate.
          ************************************************/
          newx = (inputToken->type == FLOAT) ? (double)ValueToDouble(inputToken->value) : 
                                               (double)ValueToInteger(inputToken->value);
          if (newx < u->from)
            {
              if (u->from - newx > xtolerance)
                {
                 *DeftemplateError = TRUE;
                 SyntaxErrorMessage(theEnv,"Deftemplate (X value out of range (too small))");
                 rtnFuzzyValue(theEnv,fv);
                 return(NULL);
                }
              newx = u->from;
            }
          else if (newx > u->to)
            {
              if (newx - u->to > xtolerance)
                {
                 *DeftemplateError = TRUE;
                 SyntaxErrorMessage(theEnv,"Deftemplate (X value out of range (too large))");
                 rtnFuzzyValue(theEnv,fv);
                 return(NULL);
                }
              newx = u->to;
            }

          /* cannot have x value less than a previous one (if close we
             can consider it the same as previous -- rounding errors
             may have occurred)
          */
          if (newx < previous)
            {
             if (previous - newx > FUZZY_TOLERANCE)
               {
                *DeftemplateError = TRUE;
                SyntaxErrorMessage(theEnv,"Deftemplate (Singleton x values must be in increasing order)");
                rtnFuzzyValue(theEnv,fv);
                return(NULL);
               }
             newx = previous;
            }
          SavePPBuffer(theEnv," ");
          previous = newx;
      
          /*************************************************
           Get the next token, which should be y coordinate
          *************************************************/
          GetToken(theEnv,readSource,inputToken);
        
          if (inputToken->type == FLOAT || inputToken->type == INTEGER)
            { newy = (inputToken->type == FLOAT) ? (double)ValueToDouble(inputToken->value) : 
                                                   (double)ValueToInteger(inputToken->value);
            }
          else
            {
              *DeftemplateError = TRUE;
              SyntaxErrorMessage(theEnv,"Deftemplate (Number expected)");
              rtnFuzzyValue(theEnv,fv);
              return(NULL);
            }
          if (newy < 0.0)
            {
              if (newy < -FUZZY_TOLERANCE)
                {
                  *DeftemplateError = TRUE;
                  SyntaxErrorMessage(theEnv,"Deftemplate (Membership value must be >= 0.0)");
                  rtnFuzzyValue(theEnv,fv);
                  return(NULL);
                }
              newy = 0.0;
            }
          else if (newy > 1.0)
            {
              if (newy-1.0 > FUZZY_TOLERANCE)
                {
                  *DeftemplateError = TRUE;
                  SyntaxErrorMessage(theEnv,"Deftemplate (Membership must be <= 1.0)");
                  rtnFuzzyValue(theEnv,fv);
                  return(NULL);
                }
              newy = 1.0;
            }

          /*************************************************
           Get the next token, which should be closing bracket
          *************************************************/
          GetToken(theEnv,readSource,inputToken);
          if (inputToken->type == RPAREN)
           {
            /* if this point same as last don't store it */
            if (count == 0 || !FZ_EQUAL(newx, fv->x[count-1]) || !FZ_EQUAL(newy,fv->y[count-1]))
              {
               /* if last 2 pts have same y value as this new one then
                  replace the last one with this new one

                  OR

                  if only 1st point received and y value is same as
                  this 2nd one replace the 1st one with this one.
               */
               if ((count > 2  && newy == fv->y[count-1] && newy == fv->y[count-2]) ||
                   (count == 1 && newy == fv->y[0])
                  )
                 {
                   count--;
                   if (count == 0) previous = u->from - 1.0;
                 }
        

               /*************************************************
                Store new (x,y) in arrays re-allocating if necessary.
               *************************************************/
               if ( count == maxlength )
                 {
                   tempx = FgetArray ( theEnv,maxlength + increment );
                   tempy = FgetArray ( theEnv,maxlength + increment );
                   for (i=0; i<maxlength; i++ )
                    {
                      tempx[i] = fv->x[i];
                      tempy[i] = fv->y[i];
                    }
                   FrtnArray(theEnv,fv->x, maxlength);
                   FrtnArray(theEnv,fv->y, maxlength);
                   fv->x = tempx;
                   fv->y = tempy;
                   maxlength += increment;
                   fv->maxn = maxlength;
                 }
               fv->x[count] = newx;
               fv->y[count] = newy;
               count++;

               /* if last 3 points all have same x values and all y values
                  are in increasing order or all in decreasing order, then
                  replace 2nd one with the last one. If last 4 are the same
                  replace 2nd last one with last one.
               */
               if (count >2 && newx == fv->x[count-2] && newx == fv->x[count-3])
                 {
                  if ((newy > fv->y[count-2] && fv->y[count-2] > fv->y[count-3]) ||
                      (newy < fv->y[count-2] && fv->y[count-2] < fv->y[count-3]) ||
                      (count > 3 && newx == fv->x[count-4])
                     )
                    { count--; fv->y[count-1] = fv->y[count]; }
                 }
              }
           }     
          else
           {
             *DeftemplateError = TRUE;
             SyntaxErrorMessage(theEnv,"Deftemplate ( ')' expected)");
             rtnFuzzyValue(theEnv,fv);
             return(NULL);
           }
          SavePPBuffer(theEnv," ");
         
          /************************************************
           Get the next token, which should be either a closing bracket
           indicating the end of the set, or an opening bracket indicating
           the start of another (x,y) pair.
          ***********************************************/
          GetToken(theEnv,readSource, inputToken);
          if ((inputToken->type == RPAREN) || (inputToken->type == STOP))
            {
              fv->n = count;
              return(fv);
            }
          else if (inputToken->type != LPAREN)
            {
              *DeftemplateError = TRUE;
              SyntaxErrorMessage(theEnv,"Deftemplate ( '(' expected)");
              rtnFuzzyValue(theEnv,fv);
              return(NULL);
            }
          else    /* Get next token, which should be x coordinate */
           { 
              GetToken(theEnv,readSource, inputToken);
           }
         }
     *DeftemplateError = TRUE;
     SyntaxErrorMessage(theEnv,"Deftemplate (Number expected)");
     rtnFuzzyValue(theEnv,fv);
     return(NULL);
  }



/******************************************************************/
/* parseStandardFuzzyValue:                                       */    
/*                                                                */
/* NOTE: this function is very similar to convertStandardSet      */
/*       routine found in fuzzyrhs.c -- if change this then check */
/*       that routine to see if similar changes are needed!!!     */
/******************************************************************/

static struct fuzzy_value *parseStandardFuzzyValue(
  void *theEnv,
  char *readSource,
  struct token *inputToken,
  int  *DeftemplateError,
  struct fuzzyLv *u)
  {
    struct fuzzy_value *fv;
    double alfa, beta, gamma;
    char *name;
    int z_function=FALSE, s_function=FALSE, pi_function=FALSE;
    double xtolerance;
    
    /* get the name of the standard fuction */
    if ((inputToken->type != SYMBOL) && (inputToken->type != STRING))
      {
        *DeftemplateError = TRUE;
         SyntaxErrorMessage(theEnv,"Deftemplate (Fuzzy standard function expected)");
         return(NULL);
      }
       
    name = ValueToString(inputToken->value);
    /* classify the function */
    if ( (strcmp(name,"S")==0) || (strcmp(name,"s")==0) )
        s_function = TRUE;
    else if ( (strcmp(name,"Z")==0) || (strcmp(name,"z")==0) )
        z_function = TRUE;
    else if ( (strcmp(name, "PI") == 0) || (strcmp(name, "pi") == 0) )
        pi_function = TRUE;
        
    /* check for valid function */
    if ( !s_function && !z_function && !pi_function)
      {
         *DeftemplateError = TRUE;
         SyntaxErrorMessage(theEnv,"Deftemplate (Fuzzy standard function name expected)");
         return(NULL);
      }

    SavePPBuffer(theEnv," ");
              
    /* We want to allow specifications for Standard functions and singleton sets
       to NOT be thrown out with an error message due to a floating point
       representation problem so we will allow the points to be considered
       within the universe of discourse (range) if very close.

       FUZZY_TOLERANCE is defined in constant.h
    */
    xtolerance = ((u->to - u->from) >= 1.0) ? FUZZY_TOLERANCE 
                                            : (u->to - u->from) * FUZZY_TOLERANCE;

    /* get first parameter */  
    GetToken(theEnv,readSource, inputToken);
    if (inputToken->type != FLOAT && inputToken->type != INTEGER)
       {
         *DeftemplateError = TRUE;
         SyntaxErrorMessage(theEnv,"Deftemplate (Number expected)");
         return(NULL);
       }
    SavePPBuffer(theEnv," ");
    alfa = (inputToken->type == FLOAT) ? (double)ValueToDouble(inputToken->value) : 
                                         (double)ValueToInteger(inputToken->value);
    if  (pi_function)
      {
        if (alfa < 0.0)
           {
              *DeftemplateError = TRUE;
              SyntaxErrorMessage(theEnv,"Deftemplate (PI function 1st parameter must be >= 0)");
               return(NULL);
           }
        else { beta = alfa; }
      }
    else if (alfa < u->from)  /* S or Z function */
      {
        if (u->from - alfa > xtolerance)
          {
           *DeftemplateError = TRUE;
            SyntaxErrorMessage(theEnv,"Deftemplate (s or z function 1st parameter out of range (too small))");
            return(NULL);
          }
        alfa = u->from;
      }
    else if (alfa > u->to)
      {
        if (alfa - u->to > xtolerance)
          {
           *DeftemplateError = TRUE;
            SyntaxErrorMessage(theEnv,"Deftemplate (s or z function 1st parameter out of range (too large))");
            return(NULL);
          }
        alfa = u->to;
      }

    /* get 2nd parameter */   
    GetToken(theEnv,readSource, inputToken);
    if (inputToken->type != FLOAT && inputToken->type != INTEGER)
       {
         *DeftemplateError = TRUE;
         SyntaxErrorMessage(theEnv,"Deftemplate (Number expected for standard function parameter)");
         return(NULL);
       }
    SavePPBuffer(theEnv," ");
    gamma = (inputToken->type == FLOAT) ? (double)ValueToDouble(inputToken->value) : 
                                            (double)ValueToInteger(inputToken->value);
    if (pi_function)
       { 
         if ((gamma > u->to) || (gamma < u->from))
           {
             *DeftemplateError = TRUE;
             SyntaxErrorMessage(theEnv,"Deftemplate (pi function produces x values out of range)");
             return(NULL);
           }
         else if ((gamma - beta) < u->from)
           {
             if (u->from - (gamma - beta) > xtolerance)
               {
                 *DeftemplateError = TRUE;
                 SyntaxErrorMessage(theEnv,"Deftemplate (pi function produces x values too small)");
                 return(NULL);
               }
             beta = gamma - u->from;
           }
         else if ((gamma + beta) > u->to)
           {
             if (gamma + beta - u->to > xtolerance)
               {
                 *DeftemplateError = TRUE;
                 SyntaxErrorMessage(theEnv,"Deftemplate (pi function produces x values too large)");
                 return(NULL);
               }
             beta = u->to - gamma;
           }
        }
     else if (gamma < alfa)
        {
          *DeftemplateError = TRUE;
          SyntaxErrorMessage(theEnv,"Deftemplate (s or z function 2nd parameter must be >= 1st parameter)");
          return(NULL);
        }
     else if (gamma > u->to)
        {
         if (gamma - u->to > xtolerance)
           {
            *DeftemplateError = TRUE;
            SyntaxErrorMessage(theEnv,"Deftemplate (S or Z function 2nd parameter out of range (too large))\n");
            return(NULL);
          }
         gamma = u->to;
        }

     GetToken(theEnv,readSource, inputToken);
     if (inputToken->type == RPAREN)
        {
          int ftype;
          
          if (s_function)       ftype = S_FUNCTION;
          else if (pi_function) ftype = PI_FUNCTION;
          else                  ftype = Z_FUNCTION;
           
          fv = Get_S_Z_or_PI_FuzzyValue(theEnv,alfa, beta, gamma, ftype);

          GetToken(theEnv,readSource, inputToken);
          return(fv);
        }
     else
        {
          *DeftemplateError = TRUE;
          SyntaxErrorMessage(theEnv,"Deftemplate ( ')' expected)");
          return(NULL);
        }
   }
   


#endif /* (! RUN_TIME) && (! BLOAD_ONLY)  */



/************************************************************************
    FUNCTIONS FOR MAINTAINING AND ACCESSING LV'S
 ************************************************************************/

/******************************************************************/
/* InstallFuzzyTemplate:                                          */
/* DeinstallFuzzyTemplate:                                        */
/*                                                                */
/*                                                                */
/******************************************************************/
globle void InstallFuzzyTemplate(
  struct deftemplate *theDeftemplate)
{
   struct primary_term *pt;
   struct fuzzyLv *fzTemplate = theDeftemplate->fuzzyTemplate;

   if (fzTemplate != NULL)
      {
       if (fzTemplate->units != NULL)
          IncrementSymbolCount(fzTemplate->units);
          
       pt = fzTemplate->primary_term_list;
       while (pt != NULL)
          {
            /* set each whichDeftemplate field for each fuzzy value */
            ((struct fuzzy_value *)ValueToFuzzyValue(pt->fuzzy_value_description))
                           ->whichDeftemplate = theDeftemplate;
            /* and install the fuzzy value */
            InstallFuzzyValue((void *)pt->fuzzy_value_description);
            /* NOTE: must decrease the busyCount of the deftemplate
                     that was just incremented by InstallFuzzyValue
                     because these are already associated with the
                     deftemplate and don't hold it busy -- in fact
                     if they do increment the busy count we can't
                     undeftemplate or redefine the deftemplate 
                     when it is not being used anywhere else!!
            */
            theDeftemplate->busyCount--;
            pt = pt->next;
          }
      }  
}


globle void DeinstallFuzzyTemplate(
  void *theEnv,
  struct fuzzyLv *fzTemplate)
{
   struct primary_term *pt, *this_pt;

   if (fzTemplate != NULL)
      {
       if (fzTemplate->units != NULL)
         {
           DecrementSymbolCount(theEnv,fzTemplate->units);
         }
          
       pt = fzTemplate->primary_term_list;
       while (pt != NULL)
          {
            DeinstallFuzzyValue(theEnv,(void *)pt->fuzzy_value_description);
            /* NOTE: must increase the busyCount of the deftemplate
                     that was just deccremented by DeinstallFuzzyValue
                     because these are already associated with the
                     deftemplate and don't hold it busy.
            */
            (ValueToFuzzyValue(pt->fuzzy_value_description))->whichDeftemplate->busyCount++;
            this_pt = pt;
            pt = pt->next;
            rtn_struct(theEnv,primary_term, this_pt);
          }
                  
       /* return the FuzzyLv struct */
       rtn_struct(theEnv,fuzzyLv, fzTemplate);
      }
}





/******************************************************************/
/* rtnPrimaryTermList:                                            */
/*                                                                */
/******************************************************************/
static void rtnPrimaryTermList(
  void *theEnv,
  struct primary_term *pt)
  {
   struct primary_term *this_one;
   
   while (pt != NULL)
     {
      this_one = pt;
      pt = pt->next;
      rtn_struct(theEnv,primary_term,this_one);
     }
  }



  
/******************************************************************/
/* InstallFuzzyValue                                              */
/*                                                                */
/* DeinstallFuzzyValue                                            */
/*                                                                */
/* 1 arg to each -- a (void *) which is really a ptr to a         */
/*                  FUZZY_VALUE_HN (hash node)                    */
/******************************************************************/


globle void InstallFuzzyValue(
  void *fv)
{
   struct fuzzy_value *fvptr;
   
   if (fv != NULL)
     {
      fvptr= (struct fuzzy_value *)ValueToFuzzyValue(fv);
   
      if (fvptr != NULL)
        {
         fvptr->whichDeftemplate->busyCount++;
        }
      IncrementFuzzyValueCount(fv);
     } 
}
           
globle void DeinstallFuzzyValue(
  void *theEnv,
  void *fv)
{
   struct fuzzy_value *fvptr;

   if (fv != NULL)
     {
      fvptr= (struct fuzzy_value *)ValueToFuzzyValue(fv);
   
       if (fvptr != NULL)
         {
           fvptr->whichDeftemplate->busyCount--;
         }
      DecrementFuzzyValueCount(theEnv,(struct fuzzyValueHashNode *) fv);
     } 
}
           
           

/******************************************************************/
/* rtnFuzzyValue                                                  */
/*                                                                */
/******************************************************************/

globle void rtnFuzzyValue(
  void *theEnv,
  struct fuzzy_value *fv)
{
    if (fv != NULL)
      {
         FrtnArray ( theEnv,fv->x, fv->maxn );
         FrtnArray ( theEnv,fv->y, fv->maxn );
         rtn_struct(theEnv,fuzzy_value, fv);
         if (fv->name != NULL) rm(theEnv,fv->name, strlen(fv->name)+1);
      }
}




#endif  /* FUZZY_DEFTEMPLATES */

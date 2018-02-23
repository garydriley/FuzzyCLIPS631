   /*******************************************************/
   /*      "C" Language Integrated Production System      */
   /*                                                     */
   /*                  A Product Of The                   */
   /*             Software Technology Branch              */
   /*             NASA - Johnson Space Center             */
   /*                                                     */
   /*             CLIPS Version 6.00  05/12/93            */
   /*                                                     */
   /*             FUZZY COMMANDS HEADER FILE              */
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



#ifndef _H_fuzzycom
#define _H_fuzzycom




#ifdef LOCALE
#undef LOCALE
#endif

#ifdef _FUZZYCOM_SOURCE_
#define LOCALE
#else
#define LOCALE extern
#endif

    LOCALE void           DeffuzzyCommands(void *theEnv);
    LOCALE void          *getu(void *theEnv);
    LOCALE double         getu_from(void *theEnv);
    LOCALE double         getu_to(void *theEnv);
    LOCALE void          *getu_units(void *theEnv);
    LOCALE void          *get_fs(void *theEnv);
    LOCALE void          *get_fs_template(void *theEnv);
    LOCALE void          *get_fs_lv(void *theEnv);
    LOCALE int            get_fs_length(void *theEnv); 
    LOCALE double         get_fs_value(void *theEnv);
    LOCALE double         get_fs_x(void *theEnv);
    LOCALE double         get_fs_y(void *theEnv);
    LOCALE double         moment_defuzzify(void *theEnv);
    LOCALE double         maximum_defuzzify(void *theEnv);
    LOCALE void           add_fuzzy_modifier(void *theEnv);
    LOCALE void           remove_fuzzy_modifier(void *theEnv);
    LOCALE void           set_fuzzy_inference_type(void *theEnv);
    LOCALE void          *get_fuzzy_inference_type(void *theEnv);
    LOCALE void           set_fuzzy_display_precision(void *theEnv);
    LOCALE long int       get_fuzzy_display_precision(void *theEnv);
    LOCALE void           set_alpha_value(void *theEnv);
    LOCALE double         get_alpha_value(void *theEnv);
    LOCALE void           plot_fuzzy_value(void *theEnv);
    LOCALE struct fuzzy_value *get_fuzzy_slot(void *theEnv);
    LOCALE struct fuzzy_value *fuzzy_union(void *theEnv);
    LOCALE struct fuzzy_value *fuzzy_intersection(void *theEnv);
    LOCALE struct fuzzy_value *fuzzy_modify(void *theEnv);
    LOCALE struct fuzzy_value *create_fuzzy_value(void *theEnv);

#endif


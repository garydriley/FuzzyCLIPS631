(clear)
(load fzbackward.clp)
(bsave "Temp//fzbackward.bin")
(unwatch all)
(reset)
(run)
i
2
0.5
4
0.4
6
1.0
q
(clear)
(bload "Temp//fzbackward.bin")
(reset)
(run)
i
2
0.5
4
0.4
6
1.0
q
(clear)
(load fzshower.clp)
(bsave "Temp//fzshower.bin")
(reset)
(run)
0.1
0.9
10
40
60
70
n
(clear)
(bload "Temp//fzshower.bin")
(reset)
(run)
0.1
0.9
10
40
60
70
n
(clear) ;; Turn Certainty Factor Calculations On and Off
(defglobal ?*count* = 4)

(defrule init
   =>
   (assert (counter ?*count*)))

(defrule big
   (declare (CF 0.8))
   (counter ?c&:(> ?c 2))
   =>
   (assert (value BIG)))

(defrule small
   (declare (CF 0.6))
   (counter ?c&:(<= ?c 2))
   =>
   (assert (value SMALL)))

(defrule print-and-repeat
   (declare (salience -100))
   ?cf <- (counter ?c)
   ?vf <- (value ?v)
   =>
   (printout t "Count is " ?c " with certainty of " (get-cf ?cf) " and value is "
             ?v " with certainty of " (get-cf ?vf) crlf)
   (retract ?cf ?vf)
   (bind ?*count* (- ?*count* 1))
   (if (<= ?*count* 0)
      then
      (halt))
   (disable-rule-cf-calculation) ;; turn off rule cf calculations
   (assert (counter ?*count*))
   (enable-rule-cf-calculation)) ;; turn back on again
(reset)
(run)
(clear) ;; Example 1

(deftemplate age ;definition of fuzzy variable ‘age’
   0 120 years
   ((young (25 1) (50 0))))

(deffacts fuzzy-fact
   (age young)) ; a fuzzy fact

(defrule one ; a rule that matches and asserts fuzzy facts
   (Speed_error big)
   =>
   (assert (Throttle_change small)))
(reset)
(run)
(facts)
(clear) ;; Example 2
(defrule flight-rule
   (declare (CF 0.95)) ;declares certainty factor of the rule
   (animal type bird)
   =>
   (assert (animal can fly)))
(clear) ;; Example 3

(deffacts FuzzyAndUncertainFact
         (Speed_error more_or_less zero) CF 0.9)

(defrule Uncertain_rule
   (declare (CF 0.8) )
   (Johns_age young)
   =>
   (assert (John goes to school)))
(clear) ;; Example 4

(defrule crisp-simple-rule
   (declare (CF 0.7)) ;crisp rule certainty factor of 0.7
   (light_switch off) ;crisp antecedent
   =>
   (assert (illumination_level dark)))  ; fuzzy consequent
(assert (light_switch off) CF 0.8)
(run)
(facts)
(clear) ;; Example 10

(defrule assert-cf-rule
   (declare (CF 0.8));rule CF is 0.8
   (fact 1)
   =>
   (assert (c1))
   (assert (c2) CF 0.7) ; assert c2 with CF 0.7
   (assert (c3))
   (assert (c4)))
(assert (fact 1) CF 0.9)
(run)
(facts)
(clear) ; Example 14

(deftemplate group
     0 9 ( )
     (few (1 0) (2 0.3) (3 0.9) (4 1) (5 0.8) (6 0.5) (7 0)))
(clear) ; Example 17

(deftemplate Tx "output water temperature"
   5 65 Celsius
    (  (cold (z 10 26))
       (OK (PI 2 36))
       (hot (s 37 60))
    ) )
(clear) ; Example 20

(deftemplate temperature
   0 100 C
   (  (cold (z 10 26))           
      (hot (s 37 60))            
      (warm not [ hot or cold ]) ) )
(clear) ; Example 22

(deftemplate temp
   0 100 C
   ( (cold (Z 20 40))
     (hot  (S 60 80))
     (freezing extremely cold) ) )

(defrule temp-rule
   (temp not hot and not cold)
   =>
   (printout t "It’s such a pleasant day" crlf))
(assert (temp ( 50 1) ))
(run)
(clear) ; Example 24 & 25

(deftemplate group
   0 20 members
   ((few (3 1) (6 0))
    (many (4 0) (6 1)) ) )

(defrule simple-LHS
   (group few)
   =>)
(assert (group (3 0) (5 1) (6 0)))
(agenda)
(defrule more-complex-lhs
   ?f <- (group very few or very many)
   =>
   (printout t "We are at the extreme limits of the number of "
               (get-u-units ?f) " in our club" crlf))
(agenda)
(run)
(clear) ; Example 26

(deftemplate height
   0 8 feet
   ((short (Z 3 4.5))
    (medium (pi 0.8 5))
    (tall (S 5.5 6)) ) )
  
(deftemplate person
   (slot name)
   (slot ht (type FUZZY-VALUE height)))
   
(defrule quite-complex-lhs
   (person (name ?n) (ht ?h & very tall))
   =>
   (printout t ?n " is very tall, with a height of about "
                      (maximum-defuzzify ?h) " "
                      (get-u-units ?h) crlf ) )
(assert (person (name Fred) (ht (pi 0 3.6))))
(assert (person (name Sally) (ht very tall)))
(assert (person (name Frank) (ht medium)))
(agenda)
(run)
(facts)

(clear) ; Defuzzification

(deftemplate temperature
   0 100 C
   ( (cold (z 10 26))
     (hot (s 37 60))
     (warm not [ hot or cold ]) ))
(reset)
(assert (temperature warm)) 

(defrule defuzzification-1
   ?f <- (temperature ?)                                                                  
   =>
   (bind ?t (maximum-defuzzify ?f))
   (printout t "Temperature is " ?t crlf))

(defrule defuzzification-2
   (temperature ?fv) 
   =>
   (printout t "Temperature is " (maximum-defuzzify ?fv) crlf))
(run)
(clear) ; FuzzyCLIPS Commands and Functions

(deftemplate temp
   0 100 C
   ((cold (z 20 70))
    (hot (s 30 80))))
(create-fuzzy-value temp cold)
(create-fuzzy-value temp very hot or very cold)
(create-fuzzy-value temp (pi 10 20))
(create-fuzzy-value temp (s 10 20))
(create-fuzzy-value temp (10 1) (20 0))

(defrule test
   =>
   (bind ?fv (create-fuzzy-value temp cold))
   (assert (temp ?fv)))
(reset)
(run)
(facts)
(plot-fuzzy-value t * nil nil (create-fuzzy-value temp hot))

(fuzzy-union (create-fuzzy-value temp cold)
             (create-fuzzy-value temp hot))

(plot-fuzzy-value t ".+*" nil nil
               (create-fuzzy-value temp cold)
               (create-fuzzy-value temp hot)
               (fuzzy-union (create-fuzzy-value temp cold)
                            (create-fuzzy-value temp hot)))

(fuzzy-intersection (create-fuzzy-value temp cold)
                    (create-fuzzy-value temp hot))

(plot-fuzzy-value t ".+*" nil nil
               (create-fuzzy-value temp cold)
               (create-fuzzy-value temp hot)
               (fuzzy-intersection (create-fuzzy-value temp cold)
                                   (create-fuzzy-value temp hot)))

(plot-fuzzy-value t "+*" nil nil
         (create-fuzzy-value temp hot)
         (fuzzy-modify (create-fuzzy-value temp hot) extremely))
(clear)

(deftemplate temp
   0 100 C
   ((cold (z 20 70))
    (hot (s 30 80))))
    
(deftemplate height
   0 8 feet
   ((short (Z 3 4.5))
    (medium (pi 0.8 5))
    (tall (S 5.5 6)) ) )
  
(deftemplate person
   (slot name)
   (slot ht (type FUZZY-VALUE height)))
(reset)
(defglobal ?*f* = (assert (temp hot)))
(defglobal ?*p* = (assert (person (ht tall))))
(get-u temp)
(get-u ?*f*)
(get-u 1)
(get-u (create-fuzzy-value temp hot))
(get-u-from temp)
(get-u-from ?*f*)
(get-u-from 1)
(get-u-from (create-fuzzy-value temp hot))
(get-u-to temp)
(get-u-to ?*f*)
(get-u-to 1)
(get-u-to (create-fuzzy-value temp hot))
(get-u-units temp)
(get-u-units ?*f*)
(get-u-units 1)
(get-u-units (create-fuzzy-value temp hot))
(get-fs ?*f*)
(get-fs 1)
(get-fs (create-fuzzy-value temp hot))
(get-fs-length ?*f*)
(get-fs-length 1)
(get-fs-length (create-fuzzy-value temp hot))
(get-fs-x ?*f* 2)
(get-fs-x 1 2)
(get-fs-x (create-fuzzy-value temp hot) 2)
(get-fs-y ?*f* 2)
(get-fs-y 1 2)
(get-fs-y (create-fuzzy-value temp hot) 2)
(get-fs-lv ?*f*)
(get-fs-lv 1)
(get-fs-lv (create-fuzzy-value temp hot))
(get-fs-value ?*f* 2)
(get-fs-value 1 2)
(get-fs-value (create-fuzzy-value temp hot) 2)
(get-fs-template ?*f*)
(get-fs-template 1)
(get-fs-template (create-fuzzy-value temp hot))
(get-cf ?*f*)
(get-cf 1)
(disable-rule-cf-calculation)
(enable-rule-cf-calculation)
(get-threshold)
(set-threshold 0.0)
(set-threshold 0)
(threshold 0.0)
(threshold 0)
(get-CF-evaluation)
(set-CF-evaluation when-defined)
(get-fuzzy-display-precision)
(set-fuzzy-display-precision 4)
(get-fuzzy-inference-type)
(set-fuzzy-inference-type max-min)
(get-alpha-value)
(set-alpha-value 0.0)
(fuzzyvaluep 45.6)
(fuzzyvaluep "string")
(fuzzyvaluep (create-fuzzy-value temp cold))

(fuzzy-union (create-fuzzy-value temp cold)
             (create-fuzzy-value temp hot))

(fuzzy-intersection (create-fuzzy-value temp cold)
                    (create-fuzzy-value temp hot))
(fuzzy-modify (create-fuzzy-value temp hot) extremely)

(plot-fuzzy-value t "+*" nil nil
    (create-fuzzy-value temp hot)
    (fuzzy-modify (create-fuzzy-value temp hot) extremely))
(get-fuzzy-slot ?*f*)
(get-fuzzy-slot 1)
(get-fuzzy-slot ?*p* ht)
(get-fuzzy-slot 2 ht)
(format nil "Value is '%F'" (get-fuzzy-slot ?*f*))
(format nil "Value is '%F'" (get-fuzzy-slot ?*p* ht))
(moment-defuzzify ?*f*)
(moment-defuzzify 1)
(moment-defuzzify (create-fuzzy-value temp cold))
(maximum-defuzzify ?*f*)
(maximum-defuzzify 1)
(maximum-defuzzify (create-fuzzy-value temp cold))
(is-defuzzify-value-valid)
(clear)

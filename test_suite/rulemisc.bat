(unwatch all)
(clear) ; Test Thing #1
(defrule rule-1 (foo $?b ?x) =>)
(defrule rule-2 (foo $?y) =>)
(clear) ; Test Thing #2
(watch facts)
(watch activations)
(defrule foo (not (not (and (a) (b)))) =>)
(defrule bar (not (and (a) (b))) =>)
(assert (a))
(assert (b))
(unwatch all)
(clear) ; Test Thing #3
(reset)
(defrule foo (initial-fact) (not (a)) =>)
(defrule bar (initial-fact) =>)
(agenda)
(unwatch all)
(clear) ; Test Thing #4
(defrule foo (logical (exists (a ?) (b ?))) => (assert (q)))
(reset)
(assert (a 1) (b 1) (a 2) (b 2) (a 3))
(run)
(watch facts)
(retract 1 2 3 4)
(unwatch all)
(clear) ; Test Thing #5
(defrule rule-1 (a ?x) (not (b ?x)) =>)
(reset)
(assert (a 1) (a 2) (b 2))
(run)
(refresh rule-1)
(agenda)
(clear) ; Test Thing #6
(reset)
(watch facts)
(watch activations)
(defrule all-players-practiced
   (logical (forall (player ?name)
                    (pitched ?name)
                    (batted ?name)))
   =>
   (assert (all-players-have-practiced)))
(assert (player Gary) (pitched Gary) (batted Gary))
(assert (pitched Brian) (player Brian) (batted Brian))
(run)
(retract 3)
(unwatch all)
(clear) ; Test Thing #7
(defrule rule-1
  (team ?x)
  (forall (player ?z ?x) (batted ?z) (pitched ?z))
  =>)
(matches rule-1)
(assert (team Reds))
(matches rule-1)
(assert (player Gary Reds))
(matches rule-1)
(assert (batted Gary))
(matches rule-1)
(assert (pitched Gary))
(matches rule-1)
(clear) ; Test Thing #8 - Fact Addresses References
(defrule theRule 
  ?f <- (this)
  (that ?f)
  =>)
(assert (that =(assert (this))))
(agenda)
(defrule theRule
  ?f <- (a)
  ?f <- (b)
  =>)
(defrule theRule
  (a ?f)
  ?f <- (b)
  =>)
(clear) ; Test Thing #9
(deffacts start (rule-2))
(defrule rule-1 (rule-2) (rule-2 green) =>)
(defrule rule-2 (rule-2 $?) =>)
(reset)
(agenda)
(clear) ; Test Thing #10
(defrule foo (a ?) (b ?) (c ?) =>)
(assert (a 1) (a 2) (b 1) (b 2) (c 1))
(matches foo)
(clear) ; Test Thing #11
(defrule foo 
   (exists (a ?x) (b ?x)) 
   (exists (c ?x) (d ?x))
   =>)
(reset)
(assert (a 1) (b 1) (c 2) (d 2))
(matches foo)
(clear) ; Test Thing #12

(defrule Buggy-Rule
   (A ?a)  
   (not (and (A ?a)
             (B)
             (not (and (C ?c)
                       (test (neq ?c ?a))))))
   =>) 
(reset)
(assert (A G1))
(assert (B))
(assert (C G1))
(agenda)
(clear) ; Matches
(defrule foo 
   (exists (a ?x) (b ?x) (c ?x)) 
   (exists (d ?x) (e ?x) (f ?x))
   (exists (g ?x) (h ?x) (i ?x))
   (j ?x)
   =>)
(assert (a 1) (b 1) (c 1) (d 2) (e 2) (f 2) (g 3) (h 3) (i 3) (j 4))
(matches foo)
(clear) ; Test Thing #13
(deftemplate TAG2100 (slot tag-id))
(deftemplate TAG2300 (slot parent))
(deftemplate TAG2500 (slot parent))
(deftemplate TAG2400 (slot parent))
(deftemplate GCSS-merge-tag (slot tag-id))

(defrule load-data
   =>
   (assert (TAG2300 (parent "1")))
   (assert (TAG2370))
   (assert (TAG2400 (parent "1")));  (matched no)))
   (assert (GCSS-merge-tag (tag-id "1"))))

(defrule TAG2400-AA-Update ""
   (TBX)
   (TAG2100 (tag-id ?td2))
   (TAG2500 (parent ?td2))      
   (exists (GCSS-merge-tag (tag-id ?td3))
           (TAG2400 (parent ?td2 | ?td3)) 
           (not (and (TAG2370)
                     (TAG2300 (parent ?td2 | ?td3)))))
   =>)
(reset)
(run)
(clear) ; Test Thing #14

(deftemplate TAG2100
   (slot source)
   (slot matched)
   (slot sort-order))

(defrule load-data
   =>
   (assert (TAGS100)
           (TAG2100 (source ESI) (matched yes) (sort-order 2))
           (TAG2100 (source GCSS) (matched yes) (sort-order 19))))

(defrule Rule-2 ""
   
   (TAG2100 (source ESI)
            (matched ?m))
            
   (TAG2100 (source GCSS)
            (matched ?m)
            (sort-order ?so1))

   (not (and (TAGS100)
                       
             (not (TAG2100 (source GCSS)
                           (sort-order ?so5&:(< ?so5 ?so1))))))
   
   =>)
(reset)
(run)
(clear) ; Test Thing #15
(watch activations) 
(watch facts) 
(deftemplate foo (slot bar)) 

(defrule modify-with-logical 
   (logical (something))
   ?f <- (foo (bar 1))
  => 
   (modify ?f (bar TRUE))) 

(assert (foo (bar 1)))
(assert (something))
(run) 
(facts)
(unwatch all)
(clear)
(watch activations) 
(watch facts) 
(deftemplate foo (slot bar)) 

(defrule modify-with-logical 
   (logical (something)
   ?f <- (foo (bar 1)))
  => 
   (modify ?f (bar TRUE))) 

(assert (foo (bar 1)))
(assert (something))
(run) 
(facts)
(unwatch all)
(clear) ; Test Thing #16
(watch facts)
(defrule prop
   (logical (level-search ?n))
   (not (level-search ?n1&:(> ?n1 ?n)))
   =>
   (assert (level-search (+ ?n 1))))
(reset)
(assert (level-search 1))
(run 1)
(unwatch facts)
(clear) ; Test Thing #17
(deftemplate Event (slot value) (slot time))

(defrule example-failing-2
  (Event (value 1) (time ?t1))
  (not (and (Event (value 0) (time ?tn))
            (test (< ?t1 ?tn))
            ))
  (test (< ?t1 3))
  =>
  (printout t "Fails 1" crlf))

(deffacts stuff
   (Event (value 1) (time 1)))
(reset)
(agenda)
(clear) ; Test Thing #18
(deftemplate Event (slot value) (slot time))

(defrule example-working
  (Event (value 1) (time ?t1))
  (Event (value 1) (time ?t2))
  (test (< ?t1 ?t2))
  (test (< ?t1 ?t2))
  (not (and (Event (value 0) (time ?tn))
            (test (< ?t1 ?tn))))
 =>
  (printout t "Works 1" crlf)
)

(defrule example-failing-1
  (Event (value 1) (time ?t1))
  (Event (value 1) (time ?t2))
  (test (< ?t1 ?t2))
  (not (and (Event (value 0) (time ?tn))
            (test (< ?t1 ?tn))))
  (test (< ?t1 ?t2))
=>
 (printout t "Fails 1" crlf)
)

(defrule example-simple-working-2
  (Event (value 1) (time ?t1))
  (Event (value 1) (time ?t2))
  (test (< ?t1 ?t2))
  (not (Event (value 0) (time ?tn)))
  (test (< ?t1 ?t2))
=>
 (printout t "Works (Simple!) 2" crlf)
)

(defrule example-failing-2
  (Event (value 1) (time ?t1))
  (Event (value 1) (time ?t2))
  (not (and (Event (value 0) (time ?tn))
            (test (< ?t1 ?tn))))
  (test (< ?t1 ?t2))
  (test (< ?t1 ?t2))
=>
 (printout t "Fails 2" crlf)
)

(defrule example-working-3
  (Event (value 1) (time ?t1))
  (not (and (Event (value 0) (time ?tn))
            (test (< ?t1 ?tn))))
  (Event (value 1) (time ?t2))
  (test (< ?t1 ?t2))
  (test (< ?t1 ?t2))
=>
 (printout t "Works 3" crlf)
)
(assert (Event (value 1) (time 0)))
(assert (Event (value 1) (time 1)))
(agenda)
(clear) ; Test Thing #19
(watch activations)

(defrule xx
   (SAD SD SD01 ?val0)
   (and
      (exists 
         (SAD G ?ix G10 ?val1)
         (SAD G ?ix G10 ?val2)
         (or
            (test (eq ?val1 "IQ")) 
            (test (eq ?val2 "IQ")))) 
      (test (eq ?val0 "ZAH")))
=>)
(assert (SAD SD SD01 "ZAH"))
(assert (SAD SD SD01 "BAH"))
(assert (SAD G 10 G10 "BQ"))
(assert (SAD G 10 G10 "IQ"))
(unwatch activations)
(clear) ; Test Thing #20
(watch activations)

(deffacts xy
   (SAD G 1 GX01 "XX")
   (SAD G 1 GCH 1 GCH03 "AA")
   (SAD G 2 GX01 "CN")
   (SAD G 2 GCH 1 GCH03 "AA")
   (SAD G 3 GX01 "XX")
   (SAD G 3 GCH 1 GCH03 "B00")
   (SAD G 4 GX01 "CN")
   (SAD G 4 GCH 1 GCH03 "B00"))

(defrule if_exists ""
   (SAD G ?ix1 GX01 ?var1)
   (and
      (test (eq ?var1 "CN"))
      (exists 
         (SAD G ?ix1 GCH ?ix2 GCH03 ?var2)
         (test (eq ?var2 "B00"))))
   =>)
(reset)
(unwatch activations)
(clear) ; Test Thing #21 (Sudoku)

(deftemplate possible
   (slot row)
   (slot value))

(deftemplate size-value
   (slot value))

(defrule issue
   (size-value (value ?v))
   (not (and (size-value (value ?v2&:(< ?v2 ?v)))
             (not (possible (row 1) (value ?v2)))))
   =>)

(defrule grid-values
   =>
   (assert (size-value (value 5)))
   (assert (size-value (value 6))))   
(reset)
(agenda)
(watch activations)
(watch facts)
(run 1)
(agenda)
(unwatch facts)
(unwatch activations)
(clear) ; Test Thing #22 (Sudoku)

(deftemplate possible
   (slot row)
   (slot value))

(deftemplate size-value
   (slot value))

(defrule issue
   (x)
   (size-value (value ?v))
   (not (and (y)
             (size-value (value ?v2&:(< ?v2 ?v)))
             (not (possible (row 1) (value ?v2)))))
   =>)

(defrule grid-values
   =>
   (assert (x))
   (assert (y))
   (assert (size-value (value 5)))
   (assert (size-value (value 6))))   
(reset)
(agenda)
(watch activations)
(watch facts)
(run 1)
(agenda)
(unwatch facts)
(unwatch activations)
(clear) ; Test Thing #23 (User Bug)

(deftemplate link
   (slot z)
   (slot y))

(deftemplate xfer
   (slot u)
   (slot z))
   
(deftemplate csp
   (slot y)
   (slot u))
      
(deffacts start
   (csp (y A) (u B))
   (link (z B) (y A))
   (link (z C) (y A))
   (xfer (u B) (z C)))

(defrule rule-bad
   (link (z ?zzz) (y ?yyy))
   (forall (csp (y ?yyy) (u ?uuu))
           (test (progn TRUE))
           (xfer (u ?uuu) (z ?zzz)))
   =>)
(watch facts)
(watch activations)
(reset)
(agenda)
(unwatch facts)
(unwatch activations)
(clear) ; Test Thing #24 (Client Bug)

(deftemplate thing
   (slot id))

(defrule load-data
   =>
   (assert
      (thing
         (id "GCSS2100-001"))))

(defrule Rule-1
   (exists (and (thing (id ?id))
                (thing (id ~?id))))
   =>)
      
(defrule Rule-2
   (before)
   (exists (and (thing (id ?id))
                (thing (id ~?id))))
   (after)
   =>)
(reset)
(agenda)
(run 1)
(agenda)
(clear) ; Test Thing #25 (Client Bug)

(deftemplate thing
   (slot source)
   (slot tag-id))

(defrule load-data
   =>
   (assert
      (thing
         (source BAR)
         (tag-id "thing-001"))))

(deftemplate select-thing
   (slot tag-id))
    
(defrule rule-1
   (thing (source FOO)
          (tag-id ?td1))
   =>)

(defrule rule-2
   (exists (and (thing (source BAR) 
                       (tag-id ?td2))
                (thing (source GCSS)
                       (tag-id ?td3&~?td2))))
   =>)

(defrule rule-3
   (thing (source FOO)
          (tag-id ?td1))
   (exists (and (thing (source BAR) 
                       (tag-id ?td2))
                (thing (source BAR)
                       (tag-id ?td3&~?td2))))
   (not (select-thing))
   =>)

(defrule rule-4  "This needs to share with rule-2"
   (thing (source FOO)
          (tag-id ?td1))
   (exists (and (thing (source BAR) 
                       (tag-id ?td2))
                (thing (source BAR)
                       (tag-id ?td3&~?td2))))
   (not (dabble))
   =>)
(reset)
(agenda)
(run 1)
(agenda)
(clear) ; Test Thing #26 (Client Bug)

(deftemplate thing
   (slot source)
   (slot matched (default no)))

(deftemplate dubob
   (slot source)
   (slot matched (default no)))

(deffacts initial
   (thing
      (source BAR)
      (matched X))
   (thing
      (source FOO)
      (matched X))
   (dubob
      (source FOO)
      (matched Y)))

(defrule rule-1  ""
   (compare)
   (exists (thing (source FOO)
                  (matched ?match&~no))        
           (not (thing (source BAR)
                       (matched ?match))))
   =>)

(defrule rule-2 ""
   (exists (and (thing (source FOO) (matched ~no)) ;; This join should not be shared with the prior join
                (dubob (source FOO) (matched ~no))))   
   =>)
(agenda)
(reset)
(agenda)
(clear) ; Test Thing #27 (Client Bug)

(deftemplate thing
   (slot tag-id))

(deftemplate dubob)

(deffacts data
   (dubob)
   (thing (tag-id "B")))
         
(defrule rule-1 ""
   (thing (tag-id ?td2)) 
   (not (and (dubob)  
             (not (thing (tag-id ~?td2))))) 
   =>)
(agenda)
(reset)
(agenda)
(clear) ; Class to alpha link removal
(watch activations)
(defclass A (is-a USER))
(defclass B (is-a USER))
(defrule foo (object (is-a A)) =>)
(make-instance a1 of A)
(agenda)
(undefrule foo)
(agenda)
(defrule bar (object (is-a B)) =>)
(agenda)
(make-instance a2 of A)
(make-instance b1 of B)
(agenda)
(unwatch activations)
(clear)

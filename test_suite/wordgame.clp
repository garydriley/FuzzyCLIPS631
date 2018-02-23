
;;;======================================================
;;;   Number Puzzle Problem
;;;     
;;;     Solves the number puzzle problem in which
;;;
;;;          GERALD 
;;;        + DONALD
;;;          ------
;;;        = ROBERT
;;;
;;;     CLIPS Version 5.0 Example
;;;
;;;     To execute, merely load, reset and run.
;;;     This example takes alot of memory to execute.
;;;======================================================

(defrule startup
  =>
  (printout t t "The problem is" t t)
  (printout t "   GERALD" t)
  (printout t " + DONALD" t)
  (printout t "   ------" t)
  (printout t " = ROBERT" t t)
  (assert (number 0)
          (number 1)
          (number 2)
          (number 3)
          (number 4)
          (number 5)
          (number 6)
          (number 7)
          (number 8)
          (number 9)
          (letter G)
          (letter E)
          (letter R)
          (letter A)
          (letter L)
          (letter D)
          (letter O)
          (letter N)
          (letter B)
          (letter T)))

(defrule generate-combinations
  (number ?x)
  (letter ?a)
  =>
  (assert (combination ?a ?x)))

(defrule find-solution
  (combination D ?d)
  (combination T ?t&~?d)
  (test (= (mod (+ ?d ?d) 10) ?t))
  (combination L ?l&~?d&~?t)
  (combination R ?r&~?d&~?t&~?l)
  (test (= (mod (+ ?d ?d
                   (* 10 ?l) (* 10 ?l))
                100)
           (+ (* 10 ?r) ?t)))
  (combination A ?a&~?d&~?t&~?l&~?r)
  (combination E ?e&~?d&~?t&~?l&~?r&~?a)
  (test (= (mod (+ ?d ?d
                   (* 10 ?l) (* 10 ?l)
                   (* 100 ?a) (* 100 ?a))
                1000)
           (+ (* 100 ?e) (* 10 ?r) ?t)))
  (combination N ?n&~?d&~?t&~?l&~?r&~?a&~?e)
  (combination B ?b&~?d&~?t&~?l&~?r&~?a&~?e&~?n)
  (test (= (mod (+ ?d ?d
                   (* 10 ?l) (* 10 ?l)
                   (* 100 ?a) (* 100 ?a)
                   (* 1000 ?r) (* 1000 ?n))
                10000)
           (+ (* 1000 ?b) (* 100 ?e) (* 10 ?r) ?t)))
  (combination O ?o&~?d&~?t&~?l&~?r&~?a&~?e&~?n&~?b)
  (combination G ?g&~?d&~?t&~?l&~?r&~?a&~?e&~?n&~?b&~?o)
  (test (= (+ ?d ?d
              (* 10 ?l) (* 10 ?l)
              (* 100 ?a) (* 100 ?a)
              (* 1000 ?r) (* 1000 ?n)
              (* 10000 ?e) (* 10000 ?o)
              (* 100000 ?g) (* 100000 ?d))
           (+ (* 100000 ?r) (* 10000 ?o) (* 1000 ?b) (* 100 ?e) (* 10 ?r) ?t)))
  =>
  (printout t "A Solution is:" t t)
  (printout t "  G = " ?g t)
  (printout t "  E = " ?e t)
  (printout t "  R = " ?r t)
  (printout t "  A = " ?a t)
  (printout t "  L = " ?l t)
  (printout t "  D = " ?d t)
  (printout t "  O = " ?o t)
  (printout t "  N = " ?n t)
  (printout t "  B = " ?b t)
  (printout t "  T = " ?t t)
  (printout t t)
  (printout t "   " ?g ?e ?r ?a ?l ?d t)
  (printout t " + " ?d ?o ?n ?a ?l ?d t) 
  (printout t "   " "------" t)
  (printout t " = " ?r ?o ?b ?e ?r ?t t t))  
  
 

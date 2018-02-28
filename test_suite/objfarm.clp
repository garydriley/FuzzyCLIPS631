 
;;;======================================================
;;;   Farmer's Dilemma Problem
;;;
;;;     Another classic AI problem (cannibals and the 
;;;     missionary) in agricultural terms. The point is
;;;     to get the farmer, the fox the cabbage and the
;;;     goat across a stream.
;;;     But the boat only holds 2 items. If left 
;;;     alone with the goat, the fox will eat it. If
;;;     left alone with the cabbage, the goat will eat
;;;     it.
;;;        This example uses rules and object pattern  
;;;     matching to solve the problem.
;;;
;;;     CLIPS Version 6.0 Example using
;;;     Object Pattern-Matching
;;;
;;;     To execute, merely load, reset and run.
;;;======================================================

(defmodule MAIN 
  (export defclass status))

;;;***********
;;;* CLASSES *
;;;***********

;;; The status instances hold the state  
;;; information of the search tree.

(defclass MAIN::status (is-a USER)
   (slot search-depth
     (type INTEGER) (range 1 ?VARIABLE) (default 1)) 
   (slot parent
     (type INSTANCE-NAME) (default [no-parent]))
   (slot farmer-location 
     (type SYMBOL) (allowed-symbols shore-1 shore-2) (default shore-1))
   (slot fox-location
     (type SYMBOL) (allowed-symbols shore-1 shore-2) (default shore-1))
   (slot goat-location
     (type SYMBOL) (allowed-symbols shore-1 shore-2) (default shore-1))
   (slot cabbage-location
     (type SYMBOL) (allowed-symbols shore-1 shore-2) (default shore-1))
   (slot last-move
     (type SYMBOL) (allowed-symbols no-move alone fox goat cabbage)
     (default no-move)))
   
(defclass MAIN::opposite-of
   (is-a USER)
   (slot value)
   (slot opposite-value))

;;;*****************
;;;* INITIAL STATE *
;;;*****************

(definstances MAIN::startups
  (of status)
  (of opposite-of (value shore-1) (opposite-value shore-2))
  (of opposite-of (value shore-2) (opposite-value shore-1)))

;;;***********************
;;;* GENERATE PATH RULES *
;;;***********************

(defrule MAIN::move-alone 
  ?node <- (object (is-a status)
                   (search-depth ?num)  
                   (farmer-location ?fs))
  (object (is-a opposite-of) (value ?fs) (opposite-value ?ns))
  =>
  (duplicate-instance ?node
    (search-depth (+ 1 ?num))
    (parent (instance-name ?node))
    (farmer-location ?ns)
    (last-move alone)))

(defrule MAIN::move-with-fox
  ?node <- (object (is-a status)
                   (search-depth ?num) 
                   (farmer-location ?fs)
                   (fox-location ?fs))
  (object (is-a opposite-of) (value ?fs) (opposite-value ?ns))
  =>
  (duplicate-instance ?node
    (search-depth (+ 1 ?num))
    (parent (instance-name ?node))
    (farmer-location ?ns)
    (last-move fox)
    (fox-location ?ns)))

(defrule MAIN::move-with-goat 
  ?node <- (object (is-a status)
                   (search-depth ?num) 
                   (farmer-location ?fs)
                   (goat-location ?fs))
  (object (is-a opposite-of) (value ?fs) (opposite-value ?ns))
  =>
  (duplicate-instance ?node
    (search-depth (+ 1 ?num))
    (parent (instance-name ?node))
    (farmer-location ?ns)
    (last-move goat)
    (goat-location ?ns)))

(defrule MAIN::move-with-cabbage
  ?node <- (object (is-a status)
                   (search-depth ?num) 
                   (farmer-location ?fs)
                   (cabbage-location ?fs))
  (object (is-a opposite-of) (value ?fs) (opposite-value ?ns))
  =>
  (duplicate-instance ?node
    (search-depth (+ 1 ?num))
    (parent (instance-name ?node))
    (farmer-location ?ns)
    (last-move cabbage)
    (cabbage-location ?ns)))

;;;******************************
;;;* CONSTRAINT VIOLATION RULES *
;;;******************************

(defmodule CONSTRAINTS 
  (import MAIN defclass status))
  
(defrule CONSTRAINTS::fox-eats-goat 
  (declare (auto-focus TRUE))
  ?node <- (object (is-a status)
                   (farmer-location ?s1)
                   (fox-location ?s2&~?s1)
                   (goat-location ?s2))
  =>
  (unmake-instance ?node))

(defrule CONSTRAINTS::goat-eats-cabbage 
  (declare (auto-focus TRUE))
  ?node <- (object (is-a status)
                   (farmer-location ?s1)
                   (goat-location ?s2&~?s1)
                   (cabbage-location ?s2))
  =>
  (unmake-instance ?node))

(defrule CONSTRAINTS::circular-path 
  (declare (auto-focus TRUE))
  (object (is-a status)
          (search-depth ?sd1)
          (farmer-location ?fs)
          (fox-location ?xs)
          (goat-location ?gs)
          (cabbage-location ?cs))
  ?node <- (object (is-a status)
                   (search-depth ?sd2&:(< ?sd1 ?sd2))
                   (farmer-location ?fs)
                   (fox-location ?xs)
                   (goat-location ?gs)
                   (cabbage-location ?cs))
  =>
  (unmake-instance ?node))

;;;*********************************
;;;* FIND AND PRINT SOLUTION RULES *
;;;*********************************

(defmodule SOLUTION 
  (import MAIN defclass status))
  
;;; The moves instances hold the information of all the moves
;;; made to reach a given state.
       
(defclass SOLUTION::moves (is-a USER)
   (slot id
      (type INSTANCE)) 
   (multislot moves-list 
      (type SYMBOL)
      (allowed-symbols no-move alone fox goat cabbage)))

(defrule SOLUTION::recognize-solution 
  (declare (auto-focus TRUE))
  ?node <- (object (is-a status)
                   (parent ?parent)
                   (farmer-location shore-2)
                   (fox-location shore-2)
                   (goat-location shore-2)
                   (cabbage-location shore-2)
                   (last-move ?move))
  =>
  (unmake-instance ?node)
  (make-instance of moves
     (id ?parent) (moves-list ?move)))

(defrule SOLUTION::further-solution 
  (object (is-a status)
          (name ?name)
          (parent ?parent)
          (last-move ?move))
  ?mv <- (object (is-a moves)
                 (id ?name)
                 (moves-list $?rest))
  =>
  (modify-instance ?mv (id ?parent) (moves-list ?move ?rest)))

(defrule SOLUTION::print-solution 
  ?mv <- (object (is-a moves)
                 (id [no-parent])
                 (moves-list no-move $?m))
  =>
  (unmake-instance ?mv)
  (printout t crlf "Solution found: " crlf crlf)
  (bind ?length (length$ ?m))
  (bind ?i 1)
  (bind ?shore shore-2)
  (while (<= ?i ?length)
     (bind ?thing (nth$ ?i ?m))
     (if (eq ?thing alone)
        then (printout t "Farmer moves alone to " ?shore "." crlf)
        else (printout t "Farmer moves with " ?thing " to " ?shore "." crlf))
     (if (eq ?shore shore-1)
        then (bind ?shore shore-2)
        else (bind ?shore shore-1))
     (bind ?i (+ 1 ?i))))

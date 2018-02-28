(clear) ; Case 1a
(load* "attchtst4.clp")

(defrule fail-pass "Case 1a"
   (a)
   (test (= 1 1))
   =>)

(defrule fail-fail "Case 1a"
   (b)
   (test (!= 2 2))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 1b
(load* "attchtst4.clp")

(defrule pass-fail "Case 1b"
   (not (a))
   (test (= 1 1))
   =>)

(defrule fail-fail "Case 1b"
   (not (b))
   (test (!= 2 2))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 1c
(load* "attchtst4.clp")

(defrule fail-pass "Case 1c"
   (exists (a))
   (test (= 1 1))
   =>)

(defrule fail-fail "Case 1c"
   (exists (b))
   (test (!= 2 2))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 3a
(load* "attchtst4.clp")

(defrule pass-fail "Case 3a"
   (not (and (a)
             (test (= 1 1))))
   =>)

(defrule pass-pass "Case 3a"
   (not (and (b)
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 3b
(load* "attchtst4.clp")

(defrule fail-pass "Case 3b"
   (not (and (not (a))
             (test (= 1 1))))
   =>)

(defrule pass-pass "Case 3b"
   (not (and (not (b))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 3c
(load* "attchtst4.clp")

(defrule pass-fail "Case 3c"
   (not (and (exists (a))
             (test (= 1 1))))
   =>)

(defrule pass-pass "Case 3c"
   (not (and (exists (b))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 3d
(load* "attchtst4.clp")

(defrule fail-pass "Case 3d"
   (exists (and (a)
                (test (= 1 1))))
   =>)

(defrule fail-fail "Case 3d"
   (exists (and (b)
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 3e
(load* "attchtst4.clp")

(defrule pass-fail "Case 3e"
   (exists (and (not (a))
                (test (= 1 1))))
   =>)

(defrule fail-fail "Case 3e"
   (exists (and (not (b))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 3f
(load* "attchtst4.clp")

(defrule fail-pass "Case 3f"
   (exists (and (exists (a))
                (test (= 1 1))))
   =>)

(defrule fail-fail "Case 3f"
   (exists (and (exists (b))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(clear) ; Case 4a
(load* "attchtst4.clp")

(defrule pass-pass-fail "Case 4a"
   (not (and (a)
             (b)
             (test (= 1 1))))
   =>)

(defrule pass-pass-pass "Case 4a"
   (not (and (c)
             (d)
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 4b
(load* "attchtst4.clp")

(defrule pass-fail-pass "Case 4b"
   (not (and (a)
             (not (b))
             (test (= 1 1))))
   =>)

(defrule pass-pass-pass "Case 4b"
   (not (and (c)
             (not (d))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 4c
(load* "attchtst4.clp")

(defrule pass-pass-fail "Case 4c"
   (not (and (a)
             (exists (b))
             (test (= 1 1))))
   =>)

(defrule pass-pass-pass "Case 4c"
   (not (and (c)
             (exists (d))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 4d
(load* "attchtst4.clp")

(defrule fail-fail-pass "Case 4d"
   (exists (and (a)
                (b)
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail "Case 4d"
   (exists (and (c)
                (d)
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 4e
(load* "attchtst4.clp")

(defrule fail-pass-fail "Case 4e"
   (exists (and (a)
                (not (b))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail "Case 4e"
   (exists (and (c)
                (not (d))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 4f
(load* "attchtst4.clp")

(defrule fail-fail-pass "Case 4f"
   (exists (and (a)
                (exists (b))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail "Case 4f"
   (exists (and (c)
                (exists (d))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 4g
(load* "attchtst4.clp")

(defrule pass-fail-fail-pass "Case 4g"
   (not (and (not (and (x)
                       (y)))
             (a)
             (test (= 1 1))))
   =>)

(defrule pass-pass-pass-pass "Case 4g"
   (not (and (not (and (z)
                       (w)))
             (b)
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(assert (x) (z))
(agenda)
(assert (y) (w))
(agenda)
(clear) ; Case 4h
(load* "attchtst4.clp")

(defrule fail-pass-pass-pass "Case 4h"
   (not (and (not (and (x)
                       (y)))
             (not (a))
             (test (= 1 1))))
   =>)

(defrule pass-pass-pass-pass "Case 4h"
   (not (and (not (and (z)
                       (w)))
             (not (b))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(assert (x) (z))
(agenda)
(assert (y) (w))
(agenda)
(clear) ; Case 4i
(load* "attchtst4.clp")

(defrule pass-fail-fail-pass "Case 4i"
   (not (and (not (and (x)
                       (y)))
             (exists (a))
             (test (= 1 1))))
   =>)

(defrule pass-pass-pass-pass "Case 4i"
   (not (and (not (and (z)
                       (w)))
             (exists (b))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(assert (x) (z))
(agenda)
(assert (y) (w))
(agenda)
(clear) ; Case 4j
(load* "attchtst4.clp")

(defrule fail-pass-pass-fail "Case 4j"
   (exists (and (not (and (x)
                          (y)))
                (a)
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 4j"
   (exists (and (not (and (z)
                          (w)))
                (b)
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(assert (x) (z))
(agenda)
(assert (y) (w))
(agenda)
(clear) ; Case 4k
(load* "attchtst4.clp")

(defrule pass-fail-fail-fail "Case 4k"
   (exists (and (not (and (x)
                          (y)))
                (not (a))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 4k"
   (exists (and (not (and (z)
                          (w)))
                (not (b))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(assert (x) (z))
(agenda)
(assert (y) (w))
(agenda)
(clear) ; Case 4l
(load* "attchtst4.clp")

(defrule fail-pass-pass-fail "Case 4l"
   (exists (and (not (and (x)
                          (y)))
                (exists (a))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 4l"
   (exists (and (not (and (z)
                          (w)))
                (exists (b))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (b))
(agenda)
(assert (x) (z))
(agenda)
(assert (y) (w))
(agenda)
(clear) ; Case 5a
(load* "attchtst4.clp")

(defrule fail-pass-fail "Case 5a"
   (a)
   (not (and (b)
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass "Case 5a"
   (c)
   (not (and (d)
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 5b
(load* "attchtst4.clp")

(defrule fail-fail-pass "Case 5b"
   (a)
   (not (and (not (b))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass "Case 5b"
   (c)
   (not (and (not (d))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 5c
(load* "attchtst4.clp")

(defrule fail-pass-fail "Case 5c"
   (a)
   (not (and (exists (b))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass "Case 5c"
   (c)
   (not (and (exists (d))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 5d
(load* "attchtst4.clp")

(defrule fail-fail-pass "Case 5d"
   (a)
   (exists (and (b)
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail "Case 5d"
   (c)
   (exists (and (d)
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 5e
(load* "attchtst4.clp")

(defrule fail-pass-fail "Case 5e"
   (a)
   (exists (and (not (b))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail "Case 5e"
   (c)
   (exists (and (not (d))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 5f
(load* "attchtst4.clp")

(defrule fail-fail-pass "Case 5f"
   (a)
   (exists (and (exists (b))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail "Case 5f"
   (c)
   (exists (and (exists (d))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (c))
(agenda)
(assert (b) (d))
(agenda)
(clear) ; Case 5g
(load* "attchtst4.clp")

(defrule fail-pass-both "Case 5g"
   (a (sa ?x))
   (not (and (b (sb ?x))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5g"
   (c (sc ?x))
   (not (and (d (sd ?x))
             (test (< ?x 0))))
   =>)
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5h
(load* "attchtst4.clp")

(defrule fail-fail-both "Case 5h"
   (a (sa ?x))
   (not (and (not (b (sb ?x)))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5h"
   (c (sc ?x))
   (not (and (not (d (sd ?x)))
             (test (< ?x 0))))
   =>)
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5i
(load* "attchtst4.clp")

(defrule fail-pass-both "Case 5i"
   (a (sa ?x))
   (not (and (exists (b (sb ?x)))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5i"
   (c (sc ?x))
   (not (and (exists (d (sd ?x)))
             (test (< ?x 0))))
   =>)
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5j
(load* "attchtst4.clp")

(defrule fail-fail-both "Case 5j"
   (a (sa ?x))
   (exists (and (b (sb ?x))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5j"
   (c (sc ?x))
   (exists (and (d (sd ?x))
                (test (< ?x 0))))
   =>)
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5k
(load* "attchtst4.clp")

(defrule fail-pass-both "Case 5k"
   (a (sa ?x))
   (exists (and (not (b (sb ?x)))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5k"
   (c (sc ?x))
   (exists (and (not (d (sd ?x)))
                (test (< ?x 0))))
   =>)
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5l
(load* "attchtst4.clp")

(defrule fail-fail-both "Case 5l"
   (a (sa ?x))
   (exists (and (exists (b (sb ?x)))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5l"
   (c (sc ?x))
   (exists (and (exists (d (sd ?x)))
                (test (< ?x 0))))
   =>)
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5m
(load* "attchtst4.clp")

(defrule fail-pass-pass-pass "Case 5m"
   (not (and (not (and (a)
                       (b)))
             (not (and (c)
                       (test (= 1 1))))))
   =>)

(defrule fail-fail-fail-pass "Case 5m"
   (not (and (not (and (d)
                       (e)))
             (not (and (f)
                       (test (!= 2 2))))))
   =>)
(agenda)
(assert (c) (f))
(agenda)
(assert (a) (d))
(agenda)
(assert (b) (e))
(agenda)
(clear) ; Case 5n
(load* "attchtst4.clp")

(defrule pass-fail-fail-pass "Case 5n"
   (not (and (not (and (a)
                       (b)))
             (exists (and (c)
                          (test (= 1 1))))))
   =>)

(defrule pass-pass-pass-pass "Case 5n"
   (not (and (not (and (d)
                       (e)))
             (exists (and (f)
                          (test (!= 2 2))))))
   =>)
(agenda)
(assert (c) (f))
(agenda)
(assert (a) (d))
(agenda)
(assert (b) (e))
(agenda)
(clear) ; Case 5o
(load* "attchtst4.clp")

(defrule fail-pass-both "Case 5o"
   (x)
   (a (sa ?x))
   (not (and (b (sb ?x))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5o"
   (x)
   (c (sc ?x))
   (not (and (d (sd ?x))
             (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5p
(load* "attchtst4.clp")

(defrule fail-fail-both "Case 5p"
   (x)
   (a (sa ?x))
   (not (and (not (b (sb ?x)))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5p"
   (x)
   (c (sc ?x))
   (not (and (not (d (sd ?x)))
             (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5q
(load* "attchtst4.clp")

(defrule fail-pass-both "Case 5q"
   (x)
   (a (sa ?x))
   (not (and (exists (b (sb ?x)))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5q"
   (x)
   (c (sc ?x))
   (not (and (exists (d (sd ?x)))
             (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5r
(load* "attchtst4.clp")

(defrule fail-fail-both "Case 5r"
   (x)
   (a (sa ?x))
   (exists (and (b (sb ?x))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5r"
   (x)
   (c (sc ?x))
   (exists (and (d (sd ?x))
                (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5s
(load* "attchtst4.clp")

(defrule fail-pass-both "Case 5s"
   (x)
   (a (sa ?x))
   (exists (and (not (b (sb ?x)))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5s"
   (x)
   (c (sc ?x))
   (exists (and (not (d (sd ?x)))
                (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 5t
(load* "attchtst4.clp")

(defrule fail-fail-both "Case 5t"
   (x)
   (a (sa ?x))
   (exists (and (exists (b (sb ?x)))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5t"
   (x)
   (c (sc ?x))
   (exists (and (exists (d (sd ?x)))
                (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (c (sc 1)) (a (sa 2)) (c (sc 2)))
(agenda)
(assert (b (sb 1)) (d (sd 1)))
(agenda)
(clear) ; Case 6a
(load* "attchtst4.clp")

(defrule fail-pass-fail-fail-pass "Case 6a"
   (a)
   (not (and (not (and (b)
                       (c)))
             (test (= 1 1))
             (d)))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6a"
   (e)
   (not (and (not (and (f)
                       (g)))
             (test (!= 2 2))
             (h)))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (d) (h))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 6b
(load* "attchtst4.clp")

(defrule fail-pass-pass-pass-fail "Case 6b"
   (a)
   (not (and (exists (and (b)
                          (c)))
             (test (= 1 1))
             (d)))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6b"
   (e)
   (not (and (exists (and (f)
                          (g)))
             (test (!= 2 2))
             (h)))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (d) (h))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 6c
(load* "attchtst4.clp")

(defrule fail-fail-pass-pass-fail "Case 6c"
   (a)
   (exists (and (not (and (b)
                          (c)))
                (test (= 1 1))
                (d)))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6c"
   (e)
   (exists (and (not (and (f)
                          (g)))
                (test (!= 2 2))
                (h)))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (d) (h))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 6d
(load* "attchtst4.clp")

(defrule fail-fail-fail-fail-pass "Case 6d"
   (a)
   (exists (and (exists (and (b)
                             (c)))
                (test (= 1 1))
                (d)))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6d"
   (e)
   (exists (and (exists (and (f)
                             (g)))
                (test (!= 2 2))
                (h)))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (d) (h))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 6e
(load* "attchtst4.clp")

(defrule fail-pass-fail-fail-both "Case 6e"
   (a (sa ?x))
   (not (and (not (and (b (sb ?x))
                       (c (sc ?x))))
             (test (= 1 1))
             (d (sd ?x))))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6e"
   (e (se ?x))
   (not (and (not (and (f (sf ?x))
                       (g (sg ?x))))
             (test (!= 2 2))
             (h (sh ?x))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6f
(load* "attchtst4.clp")

(defrule fail-pass-pass-pass-both "Case 6f"
   (a (sa ?x))
   (not (and (exists (and (b (sb ?x))
                          (c (sc ?x))))
             (test (= 1 1))
             (d (sd ?x))))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6f"
   (e (se ?x))
   (not (and (exists (and (f (sf ?x))
                          (g (sg ?x))))
             (test (!= 2 2))
             (h (sh ?x))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6g
(load* "attchtst4.clp")

(defrule fail-fail-pass-pass-both "Case 6g"
   (a (sa ?x))
   (exists (and (not (and (b (sb ?x))
                          (c (sc ?x))))
                (test (= 1 1))
                (d (sd ?x))))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6g"
   (e (se ?x))
   (exists (and (not (and (f (sf ?x))
                          (g (sg ?x))))
                (test (!= 2 2))
                (h (sh ?x))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6h
(load* "attchtst4.clp")

(defrule fail-fail-fail-fail-both "Case 6h"
   (a (sa ?x))
   (exists (and (exists (and (b (sb ?x))
                             (c (sc ?x))))
                (test (= 1 1))
                (d (sd ?x))))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6h"
   (e (se ?x))
   (exists (and (exists (and (f (sf ?x))
                             (g (sg ?x))))
                (test (!= 2 2))
                (h (sh ?x))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6i
(load* "attchtst4.clp")

(defrule fail-pass-fail-fail-both "Case 6i"
   (x)
   (a (sa ?x))
   (not (and (not (and (b (sb ?x))
                       (c (sc ?x))))
             (test (= 1 1))
             (d (sd ?x))))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6i"
   (x)
   (e (se ?x))
   (not (and (not (and (f (sf ?x))
                       (g (sg ?x))))
             (test (!= 2 2))
             (h (sh ?x))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6j
(load* "attchtst4.clp")

(defrule fail-pass-pass-pass-both "Case 6j"
   (x)
   (a (sa ?x))
   (not (and (exists (and (b (sb ?x))
                          (c (sc ?x))))
             (test (= 1 1))
             (d (sd ?x))))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6j"
   (x)
   (e (se ?x))
   (not (and (exists (and (f (sf ?x))
                          (g (sg ?x))))
             (test (!= 2 2))
             (h (sh ?x))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6k
(load* "attchtst4.clp")

(defrule fail-fail-pass-pass-both "Case 6k"
   (x)
   (a (sa ?x))
   (exists (and (not (and (b (sb ?x))
                          (c (sc ?x))))
                (test (= 1 1))
                (d (sd ?x))))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6k"
   (x)
   (e (se ?x))
   (exists (and (not (and (f (sf ?x))
                          (g (sg ?x))))
                (test (!= 2 2))
                (h (sh ?x))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 6l
(load* "attchtst4.clp")

(defrule fail-fail-fail-fail-both "Case 6l"
   (x)
   (a (sa ?x))
   (exists (and (exists (and (b (sb ?x))
                             (c (sc ?x))))
                (test (= 1 1))
                (d (sd ?x))))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6l"
   (x)
   (e (se ?x))
   (exists (and (exists (and (f (sf ?x))
                             (g (sg ?x))))
                (test (!= 2 2))
                (h (sh ?x))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (d (sd 1)) (h (sh 1)) (d (sd 2)) (h (sh 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7a
(load* "attchtst4.clp")

(defrule fail-fail-fail-pass "Case 7a"
   (a)
   (not (and (not (and (b)
                       (c)))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7a"
   (e)
   (not (and (not (and (f)
                       (g)))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 7b
(load* "attchtst4.clp")

(defrule fail-pass-pass-fail "Case 7b"
   (a)
   (not (and (exists (and (b)
                          (c)))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7b"
   (e)
   (not (and (exists (and (f)
                          (g)))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 7c
(load* "attchtst4.clp")

(defrule fail-pass-pass-fail "Case 7c"
   (a)
   (exists (and (not (and (b)
                          (c)))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7c"
   (e)
   (exists (and (not (and (f)
                          (g)))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 7d
(load* "attchtst4.clp")

(defrule fail-fail-fail-pass "Case 7d"
   (a)
   (exists (and (exists (and (b)
                             (c)))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7d"
   (e)
   (exists (and (exists (and (f)
                             (g)))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a) (e))
(agenda)
(assert (b) (f))
(agenda)
(assert (c) (g))
(agenda)
(clear) ; Case 7e
(load* "attchtst4.clp")

(defrule fail-fail-fail-both "Case 7e"
   (a (sa ?x))
   (not (and (not (and (b (sb ?x))
                       (c (sc ?x))))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7e"
   (e (se ?x))
   (not (and (not (and (f (sf ?x))
                       (g (sg ?x))))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7f
(load* "attchtst4.clp")

(defrule fail-pass-pass-both "Case 7f"
   (a (sa ?x))
   (not (and (exists (and (b (sb ?x))
                          (c (sc ?x))))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7f"
   (e (se ?x))
   (not (and (exists (and (f (sf ?x))
                          (g (sg ?x))))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7g
(load* "attchtst4.clp")

(defrule fail-pass-pass-both "Case 7g"
   (a (sa ?x))
   (exists (and (not (and (b (sb ?x))
                          (c (sc ?x))))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7g"
   (e (se ?x))
   (exists (and (not (and (f (sf ?x))
                          (g (sg ?x))))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7h
(load* "attchtst4.clp")

(defrule fail-fail-fail-both "Case 7h"
   (a (sa ?x))
   (exists (and (exists (and (b (sb ?x))
                             (c (sc ?x))))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7h"
   (e (se ?x))
   (exists (and (exists (and (f (sf ?x))
                             (g (sg ?x))))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7i
(load* "attchtst4.clp")

(defrule fail-fail-fail-both "Case 7i"
   (x)
   (a (sa ?x))
   (not (and (not (and (b (sb ?x))
                       (c (sc ?x))))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7i"
   (x)
   (e (se ?x))
   (not (and (not (and (f (sf ?x))
                       (g (sg ?x))))
             (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7j
(load* "attchtst4.clp")

(defrule fail-pass-pass-both "Case 7j"
   (x)
   (a (sa ?x))
   (not (and (exists (and (b (sb ?x))
                          (c (sc ?x))))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7j"
   (x)
   (e (se ?x))
   (not (and (exists (and (f (sf ?x))
                          (g (sg ?x))))
             (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7k
(load* "attchtst4.clp")

(defrule fail-pass-pass-both "Case 7k"
   (x)
   (a (sa ?x))
   (exists (and (not (and (b (sb ?x))
                          (c (sc ?x))))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7k"
   (x)
   (e (se ?x))
   (exists (and (not (and (f (sf ?x))
                          (g (sg ?x))))
                (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear) ; Case 7l
(load* "attchtst4.clp")

(defrule fail-fail-fail-both "Case 7l"
   (x)
   (a (sa ?x))
   (exists (and (exists (and (b (sb ?x))
                             (c (sc ?x))))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7l"
   (x)
   (e (se ?x))
   (exists (and (exists (and (f (sf ?x))
                             (g (sg ?x))))
                (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a (sa 1)) (e (se 1)) (a (sa 2)) (e (se 2)))
(agenda)
(assert (b (sb 1)) (f (sf 1)))
(agenda)
(assert (c (sc 1)) (g (sg 1)))
(agenda)
(clear)

(clear) ; Case 1a

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

(defrule fail-pass-both "Case 5g"
   (a ?x)
   (not (and (b ?x)
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5g"
   (c ?x)
   (not (and (d ?x)
             (test (< ?x 0))))
   =>)
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5h

(defrule fail-fail-both "Case 5h"
   (a ?x)
   (not (and (not (b ?x))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5h"
   (c ?x)
   (not (and (not (d ?x))
             (test (< ?x 0))))
   =>)
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5i

(defrule fail-pass-both "Case 5i"
   (a ?x)
   (not (and (exists (b ?x))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5i"
   (c ?x)
   (not (and (exists (d ?x))
             (test (< ?x 0))))
   =>)
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5j

(defrule fail-fail-both "Case 5j"
   (a ?x)
   (exists (and (b ?x)
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5j"
   (c ?x)
   (exists (and (d ?x)
                (test (< ?x 0))))
   =>)
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5k

(defrule fail-pass-both "Case 5k"
   (a ?x)
   (exists (and (not (b ?x))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5k"
   (c ?x)
   (exists (and (not (d ?x))
                (test (< ?x 0))))
   =>)
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5l

(defrule fail-fail-both "Case 5l"
   (a ?x)
   (exists (and (exists (b ?x))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5l"
   (c ?x)
   (exists (and (exists (d ?x))
                (test (< ?x 0))))
   =>)
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5m

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

(defrule fail-pass-both "Case 5o"
   (x)
   (a ?x)
   (not (and (b ?x)
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5o"
   (x)
   (c ?x)
   (not (and (d ?x)
             (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5p

(defrule fail-fail-both "Case 5p"
   (x)
   (a ?x)
   (not (and (not (b ?x))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5p"
   (x)
   (c ?x)
   (not (and (not (d ?x))
             (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5q

(defrule fail-pass-both "Case 5q"
   (x)
   (a ?x)
   (not (and (exists (b ?x))
             (test (> ?x 0))))
   =>)

(defrule fail-pass-pass "Case 5q"
   (x)
   (c ?x)
   (not (and (exists (d ?x))
             (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5r

(defrule fail-fail-both "Case 5r"
   (x)
   (a ?x)
   (exists (and (b ?x)
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5r"
   (x)
   (c ?x)
   (exists (and (d ?x)
                (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5s

(defrule fail-pass-both "Case 5s"
   (x)
   (a ?x)
   (exists (and (not (b ?x))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5s"
   (x)
   (c ?x)
   (exists (and (not (d ?x))
                (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 5t

(defrule fail-fail-both "Case 5t"
   (x)
   (a ?x)
   (exists (and (exists (b ?x))
                (test (> ?x 0))))
   =>)

(defrule fail-fail-fail "Case 5t"
   (x)
   (c ?x)
   (exists (and (exists (d ?x))
                (test (< ?x 0))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (c 1) (a 2) (c 2))
(agenda)
(assert (b 1) (d 1))
(agenda)
(clear) ; Case 6a

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

(defrule fail-pass-fail-fail-both "Case 6e"
   (a ?x)
   (not (and (not (and (b ?x)
                       (c ?x)))
             (test (= 1 1))
             (d ?x)))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6e"
   (e ?x)
   (not (and (not (and (f ?x)
                       (g ?x)))
             (test (!= 2 2))
             (h ?x)))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6f

(defrule fail-pass-pass-pass-both "Case 6f"
   (a ?x)
   (not (and (exists (and (b ?x)
                          (c ?x)))
             (test (= 1 1))
             (d ?x)))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6f"
   (e ?x)
   (not (and (exists (and (f ?x)
                          (g ?x)))
             (test (!= 2 2))
             (h ?x)))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6g

(defrule fail-fail-pass-pass-both "Case 6g"
   (a ?x)
   (exists (and (not (and (b ?x)
                          (c ?x)))
                (test (= 1 1))
                (d ?x)))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6g"
   (e ?x)
   (exists (and (not (and (f ?x)
                          (g ?x)))
                (test (!= 2 2))
                (h ?x)))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6h

(defrule fail-fail-fail-fail-both "Case 6h"
   (a ?x)
   (exists (and (exists (and (b ?x)
                             (c ?x)))
                (test (= 1 1))
                (d ?x)))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6h"
   (e ?x)
   (exists (and (exists (and (f ?x)
                             (g ?x)))
                (test (!= 2 2))
                (h ?x)))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6i

(defrule fail-pass-fail-fail-both "Case 6i"
   (x)
   (a ?x)
   (not (and (not (and (b ?x)
                       (c ?x)))
             (test (= 1 1))
             (d ?x)))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6i"
   (x)
   (e ?x)
   (not (and (not (and (f ?x)
                       (g ?x)))
             (test (!= 2 2))
             (h ?x)))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6j

(defrule fail-pass-pass-pass-both "Case 6j"
   (x)
   (a ?x)
   (not (and (exists (and (b ?x)
                          (c ?x)))
             (test (= 1 1))
             (d ?x)))
   =>)

(defrule fail-pass-pass-pass-pass "Case 6j"
   (x)
   (e ?x)
   (not (and (exists (and (f ?x)
                          (g ?x)))
             (test (!= 2 2))
             (h ?x)))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6k

(defrule fail-fail-pass-pass-both "Case 6k"
   (x)
   (a ?x)
   (exists (and (not (and (b ?x)
                          (c ?x)))
                (test (= 1 1))
                (d ?x)))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6k"
   (x)
   (e ?x)
   (exists (and (not (and (f ?x)
                          (g ?x)))
                (test (!= 2 2))
                (h ?x)))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 6l

(defrule fail-fail-fail-fail-both "Case 6l"
   (x)
   (a ?x)
   (exists (and (exists (and (b ?x)
                             (c ?x)))
                (test (= 1 1))
                (d ?x)))
   =>)

(defrule fail-fail-fail-fail-fail "Case 6l"
   (x)
   (e ?x)
   (exists (and (exists (and (f ?x)
                             (g ?x)))
                (test (!= 2 2))
                (h ?x)))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (d 1) (h 1) (d 2) (h 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7a

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

(defrule fail-fail-fail-both "Case 7e"
   (a ?x)
   (not (and (not (and (b ?x)
                       (c ?x)))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7e"
   (e ?x)
   (not (and (not (and (f ?x)
                       (g ?x)))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7f

(defrule fail-pass-pass-both "Case 7f"
   (a ?x)
   (not (and (exists (and (b ?x)
                          (c ?x)))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7f"
   (e ?x)
   (not (and (exists (and (f ?x)
                          (g ?x)))
             (test (!= 2 2))))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7g

(defrule fail-pass-pass-both "Case 7g"
   (a ?x)
   (exists (and (not (and (b ?x)
                          (c ?x)))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7g"
   (e ?x)
   (exists (and (not (and (f ?x)
                          (g ?x)))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7h

(defrule fail-fail-fail-both "Case 7h"
   (a ?x)
   (exists (and (exists (and (b ?x)
                             (c ?x)))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7h"
   (e ?x)
   (exists (and (exists (and (f ?x)
                             (g ?x)))
                (test (!= 2 2))))
   =>)
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7i

(defrule fail-fail-fail-both "Case 7i"
   (x)
   (a ?x)
   (not (and (not (and (b ?x)
                       (c ?x)))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7i"
   (x)
   (e ?x)
   (not (and (not (and (f ?x)
                       (g ?x)))
             (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7j

(defrule fail-pass-pass-both "Case 7j"
   (x)
   (a ?x)
   (not (and (exists (and (b ?x)
                          (c ?x)))
             (test (= 1 1))))
   =>)

(defrule fail-pass-pass-pass "Case 7j"
   (x)
   (e ?x)
   (not (and (exists (and (f ?x)
                          (g ?x)))
             (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7k

(defrule fail-pass-pass-both "Case 7k"
   (x)
   (a ?x)
   (exists (and (not (and (b ?x)
                          (c ?x)))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7k"
   (x)
   (e ?x)
   (exists (and (not (and (f ?x)
                          (g ?x)))
                (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear) ; Case 7l

(defrule fail-fail-fail-both "Case 7l"
   (x)
   (a ?x)
   (exists (and (exists (and (b ?x)
                             (c ?x)))
                (test (= 1 1))))
   =>)

(defrule fail-fail-fail-fail "Case 7l"
   (x)
   (e ?x)
   (exists (and (exists (and (f ?x)
                             (g ?x)))
                (test (!= 2 2))))
   =>)
(assert (x))
(agenda)
(assert (a 1) (e 1) (a 2) (e 2))
(agenda)
(assert (b 1) (f 1))
(agenda)
(assert (c 1) (g 1))
(agenda)
(clear)

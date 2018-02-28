(defmodule MAIN
  (export defclass A))
(defclass MAIN::A
   (is-a USER)
   (role concrete))
(definstances MAIN::A (a-main of A))
(defmodule FOO (export defclass B)
               (export defclass C))
(defclass FOO::A
   (is-a USER)
   (role concrete))
(defclass FOO::B
   (is-a USER))
(defclass FOO::C
   (is-a B)
   (role concrete))
(definstances FOO::A (a-foo of A) (c of C))
(defmodule BAR
   (import MAIN defclass A)
   (import FOO defclass C))
(defmodule WOZ
   (import FOO defclass ?ALL))
(reset)
(set-current-module WOZ)
(instances)
(instances MAIN)
(instances FOO)
(instances BAR)
(instances *)
(instances FOO B)
(instances FOO B inherit)
(instances * A)
(clear)
(defmodule QUIX (export ?ALL))
(defclass A (is-a USER) (role concrete))
(definstances A (of A) (of A))
(defmodule FOO (export ?ALL))
(defclass A (is-a USER) (role concrete))
(definstances A (of A) (of A))
(defmodule BAR (import QUIX ?ALL) (export ?ALL))
(defmodule WOZ (import FOO ?ALL))
(defmodule FRIBBAN (import BAR ?ALL))
(set-current-module MAIN)
(reset)
(instances *)
(instances MAIN)
(instances QUIX)
(instances FOO)
(instances BAR)
(instances WOZ)
(instances FRIBBAN)
(clear)
(defmodule FOO (export defclass FOO))
(defclass FOO (is-a USER) (role concrete))
(definstances FOO (a of FOO))
(defmodule BAR (export defclass BAR))
(defclass BAR (is-a USER) (role concrete))
(definstances BAR (b of BAR))
(defmodule WOZ (import FOO defclass FOO))
(defclass WOZ (is-a USER) (role concrete))
(definstances WOZ (c of WOZ))
(defmodule FRIBBAN (import BAR defclass BAR))
(defclass FRIBBAN (is-a USER) (role concrete))

(defmodule MAIN (import FOO ?ALL)
                (import BAR ?ALL))

(deffunction MAIN::testit()
   (reset)
   (progn$ (?field (get-defmodule-list))
      (set-current-module ?field)
      (printout t ?field ":" crlf)
      (instances)
      (printout t crlf)
      (printout t (instance-existp [a]) " " 
                  (instance-existp [::a]) " "
                  (class (instance-address a)) crlf)
      (printout t (instance-existp [b]) " " 
                  (instance-existp [::b]) " "
                  (class (instance-address b)) crlf)
      (printout t (instance-existp [c]) " " 
                  (instance-existp [::c]) " "
                  (class (instance-address c)) crlf)
   )
)
(testit)
(clear)
(defmodule FOO (export defclass A))
(defclass A (is-a USER) (role concrete))
(definstances A (a of A))
(defmodule BAR (export defclass B))
(defclass B (is-a USER) (role concrete))
(definstances B (c of B))
(defmodule WOZ (import BAR defclass B))
(defclass A (is-a USER) (role concrete))
(definstances A (b of A))

(deffunction testit ()
  (reset)
  (set-current-module WOZ)
  (printout t "TRUE TRUE TRUE FALSE TRUE TRUE TRUE TRUE TRUE" crlf)
  (printout t
     (instance-existp [a]) " "
     (instance-existp [b]) " "
     (instance-existp [c]) " "
     (instance-existp [::a]) " "
     (instance-existp [::b]) " "
     (instance-existp [::c]) " "
     (instance-existp [FOO::a]) " "
     (instance-existp [BAR::c]) " "
     (instance-existp [WOZ::b]) crlf))
(testit)

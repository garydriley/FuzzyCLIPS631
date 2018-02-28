(set-strategy depth)
(unwatch all)
; joinview.bat test
(clear)
(open "Results//joinview.rsl" joinview "w")
(dribble-on "Actual//joinview.out")
(batch "joinview.bat")
(dribble-off)
(load "compline.clp")
(printout joinview "joinview.clp differences are as follows:" crlf)
(compare-files "Expected//joinview.out" "Actual//joinview.out" joinview)
; close result file
(close joinview)

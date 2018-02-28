(set-strategy depth)
(unwatch all)
; attchtst2.bat test
(clear)
(open "Results//attchtst2.rsl" attchtst2 "w")
(dribble-on "Actual//attchtst2.out")
(batch "attchtst2.bat")
(dribble-off)
(load "compline.clp")
(printout attchtst2 "attchtst2.bat differences are as follows:" crlf)
(compare-files "Expected//attchtst2.out" "Actual//attchtst2.out" attchtst2)
; close result file
(close attchtst2)

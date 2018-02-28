(set-strategy depth)
(unwatch all)
; attchtst3.bat test
(clear)
(open "Results//attchtst3.rsl" attchtst3 "w")
(dribble-on "Actual//attchtst3.out")
(batch "attchtst3.bat")
(dribble-off)
(load "compline.clp")
(printout attchtst3 "attchtst3.bat differences are as follows:" crlf)
(compare-files "Expected//attchtst3.out" "Actual//attchtst3.out" attchtst3)
; close result file
(close attchtst3)

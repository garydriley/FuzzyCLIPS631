(set-strategy depth)
(unwatch all)
; attchtst5.bat test
(clear)
(open "Results//attchtst5.rsl" attchtst5 "w")
(dribble-on "Actual//attchtst5.out")
(batch "attchtst5.bat")
(dribble-off)
(load "compline.clp")
(printout attchtst5 "attchtst5.bat differences are as follows:" crlf)
(compare-files "Expected//attchtst5.out" "Actual//attchtst5.out" attchtst5)
; close result file
(close attchtst5)

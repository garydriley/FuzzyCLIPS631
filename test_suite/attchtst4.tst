(set-strategy depth)
(unwatch all)
; attchtst4.bat test
(clear)
(open "Results//attchtst4.rsl" attchtst4 "w")
(dribble-on "Actual//attchtst4.out")
(batch "attchtst4.bat")
(dribble-off)
(load "compline.clp")
(printout attchtst4 "attchtst4.bat differences are as follows:" crlf)
(compare-files "Expected//attchtst4.out" "Actual//attchtst4.out" attchtst4)
; close result file
(close attchtst4)

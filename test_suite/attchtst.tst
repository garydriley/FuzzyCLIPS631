(set-strategy depth)
(unwatch all)
; attchtst.bat test
(clear)
(open "Results//attchtst.rsl" attchtst "w")
(dribble-on "Actual//attchtst.out")
(batch "attchtst.bat")
(dribble-off)
(load "compline.clp")
(printout attchtst "attchtst.bat differences are as follows:" crlf)
(compare-files "Expected//attchtst.out" "Actual//attchtst.out" attchtst)
; close result file
(close attchtst)

(clear)
(str-length "Привет мир")          ; UTF-8 Support
(str-length "여러분 안녕하세요")       ; UTF-8 Support
(str-length "Olá Mundo")           ; UTF-8 Support
(sub-string 8 10 "Привет мир")     ; UTF-8 Support
(sub-string 5 8 "여러분 안녕하세요")  ; UTF-8 Support
(sub-string 3 5 "Olá Mundo")       ; UTF-8 Support
(str-index "" "Привет мир")        ; UTF-8 Support
(str-index "" "여러분 안녕하세요")     ; UTF-8 Support
(str-index "" "Olá Mundo")         ; UTF-8 Support
(str-index "ет" "Привет мир")      ; UTF-8 Support
(str-index "여러분" "여러분 안녕하세요")  ; UTF-8 Support
(str-index "á" "Olá Mundo")        ; UTF-8 Support
Привет мир                         ; UTF-8 Support
여러분 안녕하세요                      ; UTF-8 Support
Olá Mundo                          ; UTF-8 Support
78Пр                               ; UTF-8 Support
7여                                 ; UTF-8 Support
3.4Пр                              ; UTF-8 Support
5.1여                               ; UTF-8 Support
3eПр                               ; UTF-8 Support
5.1e여                              ; UTF-8 Support
3e+Пр                              ; UTF-8 Support
5.1e-여                             ; UTF-8 Support
?Привет                            ; UTF-8 Support
?여러분                              ; UTF-8 Support
$?Привет                           ; UTF-8 Support
$?여러분                             ; UTF-8 Support

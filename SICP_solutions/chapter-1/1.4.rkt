#lang racket

#|
if b is positive, then the result is a plus b
otherwise the result is a minus b
|#

(define (a-plus-abs-b a b)
  ((if (> b 0) + -) a b)) 

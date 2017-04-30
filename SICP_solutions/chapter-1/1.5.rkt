#lang racket

(define (p) (p))
(define (test x y)
  (if (= x 0) 0 y))

(test 0 (p))

#|

Evaluate (p) will cause the interpreter excute an infinite loop, because the body of procedure p is another same procedure.

In applicative-order evaluation, the interpreter will first evaluate (p), thus cause an infinite loop.

In normal-order evaluation, the interpreter will first execute the if expression and finally return 0. 

|#

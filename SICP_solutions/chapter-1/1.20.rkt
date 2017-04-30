#lang racket

(define (gcd a b)
  (if (= b 0)
      a
      (gcd b (remainder a b))))

#|
Evaluate (gcd 206 40) in normal order:

(gcd 206 40)
(gcd 40 (remainder 206 40))
(gcd 6 (remainder 40 6))
(gcd 4 (remainder 6 4))
(gcd 2 (remainder 4 2))
2
|#

#|
In normal order evaluation, the interpreter perform remainder operation 4 times.
Also, in applicative  order evaluation, the interpreter perform remainder operation 4 times.
|#

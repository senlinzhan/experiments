#lang racket

(define (cube-root x)
  (define tolerance 0.001)
  (define (good-enough? guess)
    (< (abs (- (* guess (sqr guess)) x))
       tolerance))
  (define (improve guess)
    (/ (+ (/ x (sqr guess)) (* 2 guess))
       3))
  (define (cube-root-iter guess)
    (if (good-enough? guess)
        guess
        (cube-root-iter (improve guess))))
  (cube-root-iter 1.0))

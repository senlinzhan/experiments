#lang racket
(require rackunit)

(define (smallest-divisor n)
  (find-divisor n 2))

(define (divide? a b)
  (zero? (remainder b a))) 

(define (find-divisor n test-divisor)
  (cond [(> (sqr test-divisor) n) n]
        [(divide? test-divisor n) test-divisor]
        [else (find-divisor n (add1 test-divisor))]))

(check-equal? (smallest-divisor 199) 199)
(check-equal? (smallest-divisor 1999) 1999)
(check-equal? (smallest-divisor 19999) 7)


#lang racket

(define (smallest-divisor n)
  (find-divisor n 2))

(define (divide? a b)
  (zero? (remainder b a))) 

(define (find-divisor n test-divisor)
  (cond [(> (sqr test-divisor) n) n]
        [(divide? test-divisor n) test-divisor]
        [else (find-divisor n (add1 test-divisor))]))

(define (prime? n)
  (= n (smallest-divisor n)))

(define (timed-prime-test n)
  (display n)
  (start-prime-test n (current-milliseconds))
  (newline))

(define (start-prime-test n start-time)
  (if (prime? n)
      (report-prime (- (current-milliseconds) start-time))
      null))

(define (report-prime elapsed-time)
  (display " *** ")
  (display elapsed-time))

#lang racket
(require rackunit)

(define (double n)
  (+ n n))

(define (halve n)
  (/ n 2))

(define (fast-mul a b)
  (define (fast-mul-iter remain x y)
    (cond [(= y 0) remain]
          [(= y 1) (+ remain x)]
          [(odd? y) (fast-mul-iter (+ remain x)
                                   x
                                   (sub1 y))]
          [else (fast-mul-iter remain
                               (double x)
                               (halve y))]))
  (fast-mul-iter 0 a b))

(check-equal? (fast-mul 10 0) (* 10 0))
(check-equal? (fast-mul 10 1) (* 10 1))
(check-equal? (fast-mul 10 2) (* 10 2))
(check-equal? (fast-mul 10 3) (* 10 3))
(check-equal? (fast-mul 10 4) (* 10 4))
(check-equal? (fast-mul 10 5) (* 10 5))
(check-equal? (fast-mul 10 6) (* 10 6))
(check-equal? (fast-mul 10 7) (* 10 7))
(check-equal? (fast-mul 10 8) (* 10 8))
(check-equal? (fast-mul 10 9) (* 10 9))
(check-equal? (fast-mul 10 10) (* 10 10))
(check-equal? (fast-mul 10 11) (* 10 11))
(check-equal? (fast-mul 10 12) (* 10 12))
(check-equal? (fast-mul 10 13) (* 10 13))

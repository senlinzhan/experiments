#lang racket
(require rackunit)

(define (sum-of-squares x y)
  (+ (sqr x)
     (sqr y)))

(define (max x y)
  (if (> x y)
      x
      y))

(define (two-max-squares x y z)
  (cond [(> x y) (sum-of-squares x (max y z))]
        [else (sum-of-squares y (max x z))]))

(check-equal? (two-max-squares 1 2 3) 13)
(check-equal? (two-max-squares 1 3 2) 13)
(check-equal? (two-max-squares 2 1 3) 13)
(check-equal? (two-max-squares 2 3 1) 13)
(check-equal? (two-max-squares 3 1 2) 13)
(check-equal? (two-max-squares 3 2 1) 13)


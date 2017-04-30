#lang racket
(require rackunit)

(define (fast-expt b n)
  (define (fast-expt-iter coefficient base exponent)
    (cond [(= exponent 0) 1]
          [(= exponent 1) (* coefficient base)]
          [(odd? exponent)
           (fast-expt-iter (* coefficient base) 
                           base 
                           (sub1 exponent))]
          [(even? exponent) 
           (fast-expt-iter coefficient 
                      (sqr base) 
                      (/ exponent 2))]))
  (fast-expt-iter 1 b n))           

(check-equal? (fast-expt 2 0) (expt 2 0))
(check-equal? (fast-expt 2 1) (expt 2 1))
(check-equal? (fast-expt 2 2) (expt 2 2))
(check-equal? (fast-expt 2 3) (expt 2 3))
(check-equal? (fast-expt 2 4) (expt 2 4))
(check-equal? (fast-expt 2 5) (expt 2 5))
(check-equal? (fast-expt 2 6) (expt 2 6))
(check-equal? (fast-expt 2 7) (expt 2 7))
(check-equal? (fast-expt 2 8) (expt 2 8))
(check-equal? (fast-expt 2 9) (expt 2 9))
(check-equal? (fast-expt 2 10) (expt 2 10))
(check-equal? (fast-expt 2 11) (expt 2 11))
(check-equal? (fast-expt 2 12) (expt 2 12))
(check-equal? (fast-expt 2 13) (expt 2 13))
(check-equal? (fast-expt 2 14) (expt 2 14))
(check-equal? (fast-expt 2 15) (expt 2 15))
(check-equal? (fast-expt 2 16) (expt 2 16))




#lang racket
(require rackunit)

(define (pascal-triangle row col)
  (if (or (= col 0) (= col row))
      1
      (+ (pascal-triangle (sub1 row) (sub1 col))
         (pascal-triangle (sub1 row) col))))

(check-equal? (pascal-triangle 0 0) 1)
(check-equal? (pascal-triangle 1 0) 1)
(check-equal? (pascal-triangle 1 1) 1)
(check-equal? (pascal-triangle 2 0) 1)
(check-equal? (pascal-triangle 2 1) 2)
(check-equal? (pascal-triangle 2 2) 1)
(check-equal? (pascal-triangle 3 0) 1)
(check-equal? (pascal-triangle 3 1) 3)
(check-equal? (pascal-triangle 3 2) 3)
(check-equal? (pascal-triangle 3 3) 1)
(check-equal? (pascal-triangle 4 0) 1)
(check-equal? (pascal-triangle 4 1) 4)
(check-equal? (pascal-triangle 4 2) 6)
(check-equal? (pascal-triangle 4 3) 4)
(check-equal? (pascal-triangle 4 4) 1)


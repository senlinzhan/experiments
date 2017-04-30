#lang racket
(require rackunit)

(define (call-p-times angle)
  (if (not (> (abs angle) 0.1))
      0
      (add1 (call-p-times (/ angle 3.0)))))

(check-equal? (call-p-times 12.15) 5)

#lang racket
(require rackunit)

(define (plus-v1 a b)
  (if (= a 0)
      b
      (add1 (plus-v1 (sub1 a) b))))

(define (plus-v2 a b)
  (if (= a 0)
      b
      (plus-v2 (sub1 a) (add1 b))))

(check-equal? (plus-v1 4 5) 9)
(check-equal? (plus-v2 4 5) 9)

#|
plus-v1 is a liner recursive process, for example, evaluate (plus-v1 4 5) will cause:
(plus-v1 4 5)
(add1 (plus-v1 3 5))
(add1 (add1 (plus-v1 2 5)))
(add1 (add1 (add1 (plus-v1 1 5))))
(add1 (add1 (add1 (add1 (plus-v1 0 5)))))
(add1 (add1 (add1 (add1 5))))
(add1 (add1 (add1 6)))
(add1 (add1 7))
(add1 8)
9
|#

#|
plus-v2 is a liner iterative process, for example, evaluate (plus-v2 4 5) will cause:
(plus-v2 4 5)
(plus-v2 3 6)
(plus-v2 2 7)
(plus-v2 1 8)
(plus-v2 0 9)
9
|#

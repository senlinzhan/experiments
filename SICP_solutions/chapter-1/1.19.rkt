#lang racket
(require rackunit)

(define (fib n)
  (fib-iter 1 0 0 1 n))
(define (fib-iter a b p q count)
  (cond [(= count 0) b]
        [(even? count)
         (fib-iter a
                   b
                   (+ (sqr p) (sqr q))
                   (+ (sqr q) (* 2 (* p q)))
                   (/ count 2))]
        [else (fib-iter (+ (* b q) (* a q) (* a p))
                        (+ (* b p) (* a q))
                        p
                        q
                        (sub1 count))]))

(check-equal? (fib 0) 0)
(check-equal? (fib 1) 1)
(check-equal? (fib 2) 1)
(check-equal? (fib 3) 2)
(check-equal? (fib 4) 3)
(check-equal? (fib 5) 5)
(check-equal? (fib 6) 8)
(check-equal? (fib 7) 13)
(check-equal? (fib 8) 21)
(check-equal? (fib 9) 34)
(check-equal? (fib 10) 55)

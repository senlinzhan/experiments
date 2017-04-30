#lang racket
(require rackunit)

(define (f-v1 n)
  (if (< n 3) 
      n
      (+ (f-v1 (- n 1))
         (* 2 (f-v1 (- n 2)))
         (* 3 (f-v1 (- n 3))))))

(define (f-v2 n)
  (define (f-iter a b c count)
    (if (= count 0)
        c
        (f-iter (+ a (* 2 b) (* 3 c))
                a 
                b
                (sub1 count))))
  (f-iter 2 1 0 n))

(check-equal? (f-v1 0) (f-v2 0))
(check-equal? (f-v1 1) (f-v2 1))
(check-equal? (f-v1 2) (f-v2 2))
(check-equal? (f-v1 3) (f-v2 3))
(check-equal? (f-v1 4) (f-v2 4))
(check-equal? (f-v1 5) (f-v2 5))
(check-equal? (f-v1 6) (f-v2 6))
(check-equal? (f-v1 7) (f-v2 7))
(check-equal? (f-v1 8) (f-v2 8))
(check-equal? (f-v1 9) (f-v2 9))
(check-equal? (f-v1 10) (f-v2 10))

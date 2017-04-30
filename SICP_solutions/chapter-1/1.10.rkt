#lang racket
(require rackunit)

(define (A x y)
  (cond [(= y 0) 0]
        [(= x 0) (* 2 y)]
        [(= y 1) 2]
        [else (A (- x 1) (A x (- y 1)))]))

(check-equal? (A 1 10) (expt 2 10))
(check-equal? (A 2 4) (expt 2 (expt 2 (expt 2 2))))
(check-equal? (A 3 3) (A 2 4))

(define (f n) (A 0 n))
(define (g n) (A 1 n))
(define (h n) (A 2 n))

;; (f n) computes (* 2 n)
(check-equal? (f 0) (* 2 0))
(check-equal? (f 1) (* 2 1))
(check-equal? (f 2) (* 2 2))
(check-equal? (f 3) (* 2 3))
(check-equal? (f 4) (* 2 4))
(check-equal? (f 5) (* 2 5))
(check-equal? (f 6) (* 2 6))

;; (g n) computes (expt 2 n)
(check-equal? (g 0) 0)
(check-equal? (g 1) (expt 2 1))
(check-equal? (g 2) (expt 2 2))
(check-equal? (g 3) (expt 2 3))
(check-equal? (g 4) (expt 2 4))
(check-equal? (g 5) (expt 2 5))
(check-equal? (g 6) (expt 2 6))

;; (h n) computes as below:
(check-equal? (h 0) 0)
(check-equal? (h 1) 2)
(check-equal? (h 2) (expt 2 (h 1)))
(check-equal? (h 3) (expt 2 (expt 2 (h 1))))
(check-equal? (h 4) (expt 2 (expt 2 (expt 2 (h 1)))))
(check-equal? (h 5) (expt 2 (expt 2 (expt 2 (expt 2 (h 1))))))

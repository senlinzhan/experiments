#lang racket

(define (new-if predicate then-clause else-clause)
  (cond [predicate then-clause]
        [else else-clause]))

(define (average a b)
  (/ (+ a b) 2))

(define (sqrt x)
  (define tolerance 0.001)
  (define (good-enough? guess)
    (< (abs (- (sqr guess) x)) tolerance))
  (define (improve guess)
    (average guess (/ x guess)))
  (define (sqrt-iter guess)
    (new-if (good-enough? guess)
            guess
            (sqrt-iter (improve guess))))
  (sqrt-iter 1.0))

#|

The difference between new-if and if expression is that new-if is a procedure and if not.

For example, when execute the code block:
(new-if (good-enough? guess)
        guess
        (sqrt-iter (improve guess)))

Because new-if is a procedure, so before the interpreter execute it's body, it's operand will first be evaluated.

Thus the three operand will be evaluated before execute new-if's body.

Noticing the third operand of new-if is another sqrt-iter procedure, so will cause the infinite loop.

|#


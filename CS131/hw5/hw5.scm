#lang scheme

; Defines custom acceptor function. Original solution
; takes in acceptor as from input.

(define (acceptor frag)
  (call/cc (lambda (cc)
    (cons frag (lambda () (cc #f))))))

(define match-junk
  (lambda (k frag acceptor)
    (or (acceptor frag)
	(and (< 0 k)
	     (pair? frag)
	     (match-junk (- k 1) (cdr frag) acceptor)))))

(define match-*
  (lambda (matcher frag acceptor)
    (or (acceptor frag)
	(matcher frag
		 (lambda (frag1)
		   (and (not (eq? frag frag1))
			(match-* matcher frag1 acceptor)))))))

(define make-matcher-rec
  (lambda (pat)
    (cond

     ((symbol? pat)
      (lambda (frag acceptor)
	(and (pair? frag)
	     (eq? pat (car frag))
	     (acceptor (cdr frag)))))

     ((eq? 'or (car pat))
      (let make-or-matcher ((pats (cdr pat)))
	(if (null? pats)
	    (lambda (frag acceptor) #f)
	    (let ((head-matcher (make-matcher-rec (car pats)))
		  (tail-matcher (make-or-matcher (cdr pats))))
	      (lambda (frag acceptor)
		(or (head-matcher frag acceptor)
		    (tail-matcher frag acceptor)))))))

     ((eq? 'list (car pat))
      (let make-list-matcher ((pats (cdr pat)))
	(if (null? pats)
	    (lambda (frag acceptor) (acceptor frag))
	    (let ((head-matcher (make-matcher-rec (car pats)))
		  (tail-matcher (make-list-matcher (cdr pats))))
	      (lambda (frag acceptor)
		(head-matcher frag
			      (lambda (frag1)
				(tail-matcher frag1 acceptor))))))))

     ((eq? 'junk (car pat))
      (let ((k (cadr pat)))
	(lambda (frag acceptor)
	  (match-junk k frag acceptor))))

     ((eq? '* (car pat))
      (let ((matcher (make-matcher-rec (cadr pat))))
	(lambda (frag acceptor)
	  (match-* matcher frag acceptor)))))))

(define (make-matcher pat) 
  (lambda (frag)
    ((make-matcher-rec pat) frag acceptor)))

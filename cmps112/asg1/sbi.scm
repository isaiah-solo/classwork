#!/afs/cats.ucsc.edu/courses/cmps112-wm/usr/racket/bin/mzscheme -qr
;; $Id: sbi.scm,v 1.3 2016-09-23 18:23:20-07 - - $
;;
;; Isaiah Solomon
;; icsolomo
;; CMPS 112 Fall 2016
;;
;; NAME
;;    sbi.scm - silly basic interpreter
;;
;; SYNOPSIS
;;    sbi.scm filename.sbir
;;
;; DESCRIPTION
;;    The file mentioned in argv[1] is read and assumed to be an SBIR
;;    program, which is the executed.  Currently it is only printed.
;;

(define *stderr* (current-error-port))

(define *run-file*
    (let-values
        (((dirpath basepath root?)
              (split-path (find-system-path 'run-file))
        ))
        (path->string basepath)
    )
)

(define (die list)
    (for-each (lambda (item) (display item *stderr*)) list)
    (newline *stderr*)
    (exit 1)
)

(define (usage-exit)
    (die `("Usage: " ,*run-file* " filename"))
)

(define (readlist-from-inputfile filename)
    (let ((inputfile (open-input-file filename)))
        (if (not (input-port? inputfile))
            (die `(,*run-file* ": " ,filename ": open failed"))
            (let ((program (read inputfile)))
                (close-input-port inputfile)
                    program)
        )
    )
)

;; ===================================================

(define *symbols* (make-hash))

(define (put-symbol! key value)
        (hash-set! *symbols* key value)
)

(define native-hash-table (make-hash))

(define label-hash-table (make-hash))

(define length-of
    (lambda (ls)
        (if (null? ls) 0
            (+ (length (cdr ls)) 1)
        )
    )
)

(for-each
    (lambda (pair)
        (put-symbol! (car pair) (cadr pair)))
    `(
        (log10_2 0.301029996)
        (sqrt_2  1.414213562)
        (e       2.718281828)
        (pi      3.141592654)
        (div     ,(lambda (x y) (floor (/ x y))))
        (log10   ,(lambda (x) (/ (log x) (log 10.0))))
        (mod     ,(lambda (x y) (- x (* (div x y) y))))
        (quot    ,(lambda (x y) (truncate (/ x y))))
        (rem     ,(lambda (x y) (- x (* (quot x y) y))))
        (<>      ,(lambda (x y) (not (= x y))))
        (+ ,+) (- ,-) (* ,*) (/ ,/) (abs ,abs) 
        (<= ,<=) (>= ,>=) (= ,=) (> ,>) (tan ,tan)
        (< ,<) (^ ,expt) (atan ,atan) (sin ,sin) (cos ,cos)
        (ceil ,ceiling) (exp ,exp) (floor ,floor)
        (asin ,asin) (acos ,acos) (round ,round)
        (log ,log) (sqrt ,sqrt)
    )
)

(define (evaluate-expression expr)
    (cond
        ((number? expr) expr)
        ((string? expr) expr)
        ((hash-has-key? *symbols* expr)
            (hash-ref *symbols* expr)
        )
        ((pair? expr)
            (if (hash-has-key? *symbols* (car expr))
                (let ((head (hash-ref *symbols*  (car expr))))
                    (cond 
                        ((number? head) head)
                        ((vector? head)
                        ((procedure? head)
                            (apply head (map (lambda (x)
                                (evaluate-expression x))
                                (cdr expr)))
                        )
                            (vector-ref head (cadr expr))
                        )
                        (else (die "Error: Invalid table."))
                    )
                )
                (die (list "Error: " 
                    (car expr) " not in symbol table!\n")
                )
            )
        )
    )
)

(define (assign-variable expression count)
    (if (null? expression) count
        (let ((input (read)))
            (if (eof-object? input) -1
                (begin
                    (put-symbol! (car expression) input)
                    (set! count (+ 1 count))
                    (assign-variable (cdr expression) count)
                )
            )
        )
    )
)

(define (silly-print expression)
    (map (lambda (input) (display (evaluate-expression input)))
        expression)
    (newline)
)

(define (silly-dim expression)
    (set! expression (car expression))
    (let ((arr
        (make-vector (evaluate-expression (cadr expression))
            (car expression)
        )))
        (put-symbol! (car expression) (+ (evaluate-expression
            (cadr expression)) 1)
        )
    )
)

(define (silly-let expression)
    (put-symbol! (car expression) (evaluate-expression
        (cadr expression)))
)

(define (silly-input expression)
    (put-symbol! 'inputcount 0)
    (if (null? (car expression))
        (put-symbol! 'inputcount -1)
        (begin
            (put-symbol! 'inputcount (assign-variable expression 0))
        )
    )
)

(define (silly-if expression)
    (printf "If")
)

(define (silly-goto expression)
    (printf "Goto")
)

(define (execute-line instr program linenr)
    (when (not (hash-has-key? native-hash-table (car instr)))
        (die "~s is not valid." (car instr))
    )
    (cond
        ((eq? (car instr) 'goto)
            (evaluate-line program (hash-ref label-hash-table
                (cadr instr))
            )
        )
        ((eq? (car instr) 'if)
            (if (evaluate-expression (car (cdr instr)))
                (evaluate-line program (hash-ref label-hash-table
                    (cadr (cdr instr)))
                )
                (evaluate-line program (+ linenr 1))
            )
        )
        ((eq? (car instr) 'print)
            (if (null? (cdr instr))
                (newline)
                (silly-print (cdr instr))
            )
            (evaluate-line program (+ linenr 1))
        )
        (else
            ((hash-ref native-hash-table (car instr)) (cdr instr))
            (evaluate-line program (+ linenr 1))
        )
    )
)

(define (evaluate-line program linenr)
    (when (> (length-of program) linenr)
        (let((line (list-ref program linenr)))
            (cond
                ((= (length-of line) 3)
                    (set! line (cddr line))
                    (execute-line (car line) program linenr)
                )
                ((and (= (length-of line) 2) (pair? (cadr line)))
                    (set! line (cdr line))
                    (execute-line (car line) program linenr)
                )
                (else 
                    (evaluate-line program (+ linenr 1))
                )
            )
        )
    )
)

(define (hash-labels program)
    (map (lambda (line) 
        (when (not (null? line))
            (when (or (= 3 (length-of line))
                    (and (= 2 (length-of line)) 
                        (not (pair? (cadr line)))
                    ))
                (hash-set! label-hash-table (cadr line) (- (car line)
                    1 ))
            )
        )
    ) program)
)

(define (main arglist)
    (if (or (null? arglist) (not (null? (cdr arglist))))
        (usage-exit)
        (let* ((sbprogfile (car arglist))
              (program (readlist-from-inputfile sbprogfile)))
              
              (hash-labels program)
              (evaluate-line program 0)
        )
    )
)

(for-each
    (lambda (pair)
        (hash-set! native-hash-table (car pair) (cadr pair))
    )
  ` (
        (print ,silly-print)
        (goto  (silly-goto))
        (input ,silly-input)
        (dim   ,silly-dim)
        (if    (silly-if))
        (let   ,silly-let)
    )
)

(main (vector->list (current-command-line-arguments)))

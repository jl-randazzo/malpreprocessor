;problem 1
(defun fact (num)
  (cond ((<= num 0) 0)
        ((>= num 1) 1)
        (t (* num (fact (- num 1))))))

;problem 2
(defun xsearch (num lst)
  (cond ((null lst) 0)
        ((listp (car lst)) (+ (xsearch num (car lst)) (xsearch num (cdr lst))))
        ((not (numberp (car lst))) (xsearch num (cdr lst)))
        ((= num (car lst)) (+ 1 (xsearch num (cdr lst))))
        (t (xsearch num (cdr lst)))
        ))

;problem 3
(defun divide-by-3 (lst)
  (cond ((null lst) 0)
    ((listp (car lst)) (+ (divide-by-3 (car lst)) (divide-by-3 (cdr lst))))
        ((= (mod (car lst) 3) 0) (+ 1 (divide-by-3 (cdr lst))))
        (t (divide-by-3 (cdr lst)))))

;problem 4 these two go together
(defun check-dup (lst)
  (cond ((null lst) nil)
        (t (or (check-dup (cdr lst)) (find-dup (car lst) (cdr lst))))))

(defun find-dup (num lst)
  (cond ((null lst) nil)
        ((= num (car lst)) t)
        (t (find-dup num (cdr lst)))))

;problem 5
(defun non-neg (lst)
  (cond ((null lst) nil)
	((< (car lst) 0) (cons (- 0 (car lst)) (non-neg (cdr lst))))
	(t (cons (car lst) (non-neg (cdr lst))))))

;problem 6 three go together
 (defun convert (tup)
  (cond ((equal (car (cdr tup)) 'C) (C-to-F (car tup)))
	(t (F-to-C (car tup)))))

(defun C-to-F (far)
  (+ 32 (* 9 (/ far 5))))

(defun F-to-C (cel)
  (* 5 (/ (- cel 32) 9)))

;problem 7 these three go together
(defun check-group (lst)
  (cond ((null lst) 0)
        (t (check-group-2 (car lst) (cdr lst)))))

(defun check-group-2 (last lst)
  (cond ((null lst) 0)
        ((equal last (car lst)) (+ 1 (spin-group last (cdr lst))))
        (t (check-group-2 (car lst) (cdr lst)))))

(defun spin-group (last lst)
  (cond ((null lst) 0)
        ((equal last (car lst)) (spin-group last (cdr lst)))
        (t (check-group-2 (car lst) (cdr lst)))))

(defun make-recent (word lst)
  (find-word word lst nil))

(defun find-word (word lst prevlst)
  (cond ((null lst) (cons word prevlst))
        ((equal word (car lst)) (cons word (append prevlst (cdr lst))))
        (t (find-word word (cdr lst) (cons prevlst (car lst))))))

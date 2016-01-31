#!/usr/bin/env roseus

(if (not (find-package "EUS_LP")) (make-package "EUS_LP"))

(defvar eus_lp::*plugin*
  (let* ((pkg "eus_lp")
         (so (read-line (piped-fork (format nil "find `rospack find ~A`/ | grep lib~A.so" pkg pkg)) nil)))
    (if so (load-foreign so)
      (labels
          ((library_search
            (str &key
                 (depth 0)
                 colon-pos lib-path)
            (format t "  [~A] target=" depth)
            (cond
             ((eq (length str) 0)
              (format t "~% ~A_plugin_not_found in ~A.l~%" pkg pkg)
              (exit -1))
             ((and (setq colon-pos (or (position #\: str) (length str)))
                   (setq lib-path (subseq str 0 colon-pos))
                   (setq lib-path
                         (if (eq (aref lib-path (- (length lib-path) 1)) #\/)
                             (subseq lib-path 0 (- (length lib-path) 1))
                           lib-path))
                   (probe-file (setq lib-path
                                     (print (format nil "~A/lib~A.so" lib-path pkg)))))
              (load-foreign lib-path))
             (t
              (library_search (subseq str (min (length str) (+ colon-pos 1)))
                              :depth (+ depth 1))))))
        (library_search (unix:getenv "LD_LIBRARY_PATH"))))))

;; int eus_lp_lpsolve(int eq_dim, int ieq_dim, int state_dim, double* f, double* x, double* objective, double* ieq_coeff, double* ieq_matrix, double* eq_coeff, double* eq_matrix)
;; int eus_lp_clp(int eq_dim, int ieq_dim, int state_dim, double* f, double* x, double* coeff, int _element_size, double* elements, long* _rowindices, long* _colindices, double* collower, double* colupper, double* rowlower, double* rowupper) {


(defforeign eus_lp::_lp-solve eus_lp::*plugin* "eus_lp_lpsolve" (:integer :integer :integer :string :string :string :string :string :string :string) :integer)
(defforeign eus_lp::_clp eus_lp::*plugin* "eus_lp_clp" (:integer :integer :integer :string :string :string :integer :string :string :string :string :string :string :string) :integer)

;; max: 2.5 x1 + 5 x2 + 3.4 x3;
;; C1: 2 x1 + 10 x2 + 4 x3 <= 425;
;; C2: 6 x1 + 5 x2 + 8 x3 <= 400;
;; C3: 7 x1 + 10 x2 + 8 x3 <= 600;
;;
;; Value of objective function: 266.25
;; Actual values of the variables:
;; x1 30
;; x2 34
;; x3 6.25

(defun eus_lp::lp-solve
  (&key
   (f (instantiate float-vector 1))
   (x (instantiate float-vector 3))
   (objective-coeff-vector (float-vector 0 -2.5 -5 -3.4))
   (ieq-coeff-vector (float-vector 425 400 600))
   (ieq-coeff-matrix (send (make-matrix 3 4 '((0 2 10 4) (0 6 5 8) (0 7 10 8))) :get-val 'entity))
   (eq-coeff-vector (instantiate float-vector 0))
   (eq-coeff-matrix (instantiate float-vector 0))
   (state-dim (length x))
   (eq-dim (length eq-coeff-vector))
   (ieq-dim (length ieq-coeff-vector)))
  (eus_lp::_lp-solve eq-dim ieq-dim state-dim f x objective-coeff-vector ieq-coeff-vector ieq-coeff-matrix eq-coeff-vector eq-coeff-matrix)
  x)

(defun eus_lp::clp
  (&key
   (inf 1e+10)
   (f (instantiate float-vector 1))
   (x (instantiate float-vector 3))
   (state-min-vector (float-vector 0 0 0))
   (state-max-vector (float-vector inf inf inf))
   (objective-coeff-vector (float-vector -2.5 -5 -3.4))
   (ieq-coeff-vector-max (float-vector 425 400 600))
   (ieq-coeff-vector-min (float-vector (* -1 inf) (* -1 inf) (* -1 inf)))
   (ieq-coeff-matrix (send (make-matrix 3 3 '((2 10 4) (6 5 8) (7 10 8))) :get-val 'entity))
   (eq-coeff-vector (instantiate float-vector 0))
   (eq-coeff-matrix (instantiate float-vector 0))
   (state-dim (length x))
   (eq-dim (length eq-coeff-vector))
   (ieq-dim (length ieq-coeff-vector-min))
   (dummy (instantiate integer-vector 0))
   )
  (eus_lp::_clp eq-dim ieq-dim state-dim f x objective-coeff-vector 0 ieq-coeff-matrix dummy dummy state-min-vector state-max-vector ieq-coeff-vector-min ieq-coeff-vector-max)
  x)

(defun eus_lp::test-all
  nil
  (let* ((ref-x (float-vector 30 34 6.25))
	 (ref-f (float-vector 266.25))
	 (clp-x (instantiate float-vector 3))
	 (clp-f (instantiate float-vector 1))
	 (lpsolve-x (instantiate float-vector 3))
	 (lpsolve-f (instantiate float-vector 1))
	 (clp-tm) (lpsolve-tm))
    (setq clp-tm (bench2 (eus_lp::clp :f clp-f :x clp-x)))
    (warning-message 6 "[eus_lp::clp] ~A(at ~A), diff: ~A(~A), time: ~A~%"
		     (aref clp-f 0) clp-x
		     (norm (v+ clp-f ref-f)) (norm (v- clp-x ref-x)) clp-tm)
    (setq lpsolve-tm (bench2 (eus_lp::lp-solve :f lpsolve-f :x lpsolve-x)))
    (warning-message 6 "[eus_lp::lpsolve] ~A(at ~A), diff: ~A(~A), time: ~A~%"
		     (aref lpsolve-f 0) lpsolve-x
		     (norm (v+ lpsolve-f ref-f)) (norm (v- lpsolve-x ref-x))
		     lpsolve-tm)
    t
    ))

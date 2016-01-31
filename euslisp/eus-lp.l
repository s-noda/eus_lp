#!usr/bin/env roseus

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
;; ;; int eus_lp_clp(double* f, double* x, int element_size, int* rowindices, int* colindices, double* elements, double* collower, double* colupper, double* coeff, double* rowlower, double* rowupper
;;  int eus_lp_clp(int eq_dim, int ieq_dim, int state_dim, double* f, double* x, int element_size, double* elements, long* _rowindices, long* _colindices, double* collower, double* colupper, double* coeff, double* rowlower, double* rowupper) {


(defforeign eus_lp::_lp-solve eus_lp::*plugin* "eus_lp_lpsolve" (:integer :integer :integer :string :string :string :string :string :string :string) :integer)
(defforeign eus_lp::_clp eus_lp::*plugin* "eus_lp_clp" (:integer :integer :integer :string :string :integer :string :string :string :string :string :string :string :string) :integer)


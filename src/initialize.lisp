;;;; Defines main entry point into the ecl machine
;; Move this to another package to hide two dangerous working-instance calls

(defparameter TAU (* 2.0 PI))

;; These functions should be hidden at a later time
(defmacro with-new-enemy-shot (&rest body)
  `(let ((old (get-working-instance))
	 (new (add-new-enemy-shot)))
     (set-working-instance new)
     ,@body
     (set-working-instance old)))

;; Helper function
(defun in-range (val lower upper)
  (and (>= val lower)
       (<= val upper)))

;; Load patterns for shots
(print (load "src/shot_patterns.lisp"))
(print *package*)

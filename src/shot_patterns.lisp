;; Package to maintain bullets
(defun do-nothing (index)
  "simple dummy function for bullets that keep heading and attributes"
  t)

(defun step-function (index)
  (when (= index 30)
    (set-color 255 0 255 255)
    (set-direction (* PI .5) nil))
  (when (= index 60)
    (set-direction (* TAU .75) nil))
  (when (= index 90)
    (set-color 255 127 255 255)
    (set-direction (random PI) nil)))



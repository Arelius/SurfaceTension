(init-world (make-vec2 200 200) (make-vec2 -200 -200) (make-vec2 0 -93.0))

(set-game-var 'GroundCoefficient 0.99)

(init-camera 200 (/ 9 12) 0.5)

(set-debug-draw #t)

(make-static-box (make-vec2 0 -100) 200 10)

(make-static-box (make-vec2 -200 0) 10 200)

(make-static-box (make-vec2 200 0) 10 200)

(init-fluids 2.5 1.0 0.01)

(make-fluid-box (make-vec2 -190 -50) (make-vec2 190 -90) 100 5)

(init-player (make-vec2 0 20))
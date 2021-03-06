(define (problem coffee)
  (:domain coffee)
  (:init (probabilistic 0.5 (in-office))
	 (probabilistic 0.5 (raining))
	 (probabilistic 0.5 (has-umbrella))
	 (probabilistic 0.5 (is-wet))
	 (probabilistic 0.5 (has-coffee))
	 (probabilistic 0.5 (user-has-coffee)))
  (:metric maximize (reward)))

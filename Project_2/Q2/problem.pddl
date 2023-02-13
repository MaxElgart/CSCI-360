(define (problem problem_logistics)
(:domain basic_logistics)
(:requirements :strips :typing)

    (:objects 
        g1 g2 g3 g4 g5 g6 g7 - ground
        ctr1 ctr2 ctr3 ctr4 ctr5 - counter
        p1 - pot
        c1 c2 - cook
        ingred1 ingred2 ingred3 - ingredient
    )
    
    (:init
        ;; cooks is at g1 and g6, respectively
        (at c1 g1)
        (at c2 g6)
        
        ;; TODO: complete pot is on counter ctr1
        (on p1 ctr1)
        ;; TODO: complete ingredients are on counters ctr3, ctr4 and ctr5, respectively
        (on ingred1 ctr3)
        (on ingred2 ctr4)
        (on ingred3 ctr5)
        ;; TODO: complete all ground adjacent to counter (define as show in Figure 2)
        (adjacent g2 ctr1)
        (adjacent g5 ctr1)
        (adjacent g5 ctr3)
        (adjacent g7 ctr5)
        (adjacent g6 ctr4)
        (adjacent g3 ctr4)
        
        ;; TODO: complete all ground connections (define as show in Figure 2)
        (connected g1 g2)
        (connected g1 g3)
        (connected g2 g4)
        (connected g2 g1)
        (connected g3 g4)
        (connected g3 g1)
        (connected g4 g2)
        (connected g4 g3)
        (connected g4 g5)
        (connected g4 g6)
        (connected g5 g4)
        (connected g5 g7)
        (connected g6 g4)
        (connected g6 g7)
        (connected g7 g5)
        (connected g7 g6)
    )
    
    (:goal (and 
        ;; TODO: complete this section
        (at c1 g1)
        (at c2 g6)
        (in ingred1 p1)
        (in ingred2 p1)
        (in ingred3 p1)
    ))
)
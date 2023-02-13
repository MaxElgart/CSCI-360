(define (domain basic_logistics)
(:requirements :strips :typing)
    
    (:types 
        location locatable - object
        items cook - locatable
        ingredient pot - items
        ground counter - location
    )
    
    (:predicates
        (connected ?from ?to - ground)
        (at ?c - cook ?g - ground)
        (in ?i - ingredient ?p - pot)
        (on ?itm - items ?ctr - counter)
        (hold ?i - ingredient ?c - cook)
        (adjacent ?g - ground ?ctr - counter)
    )
    
    ;;================;;
    ;; cook actions ;;
    ;;================;;
    
    (:action walk
      :parameters (?c - cook ?from ?to - ground)
      :precondition (and
        ;; TODO: Add the preconditions here
        (connected ?from ?to)
        (at ?c ?from)
      )
      :effect (and
        ;; TODO: Add the effects here
        (at ?c ?to)
        (not (at ?c ?from))
      )
    )
    
    (:action pick_ingredient
      :parameters (?i - ingredient ?c - cook ?ctr - counter ?stand - ground)
      :precondition (and
        ;; TODO: Add the preconditions here
        (on ?i ?ctr)
        (adjacent ?stand ?ctr)
        (at ?c ?stand)
      )
      :effect (and
        ;; TODO: Add the effects here
        (hold ?i ?c)
      )
    )
    
    (:action drop_ingredient
      ;; TODO: Complete defining this action
      :parameters (?i - ingredient ?c - cook ?ctr - counter ?stand - ground ?p - pot)
      :precondition (and
        ;; TODO: Add the preconditions here
        (adjacent ?stand ?ctr)
        (at ?c ?stand)
        (hold ?i ?c)
        (on ?p ?ctr)
      )
      :effect (and
        ;; TODO: Add the effects here
        (in ?i ?p)
        (not (hold ?i ?c))
      )
    )
    
)
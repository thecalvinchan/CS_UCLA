let rec contains = fun a b ->
    match b with
        | []->false
        | h::t -> if a = h then true else contains a t;;

let rec subset = fun a b ->
    match a with
        | []->true
        | h::t->if contains h b then subset t b else false;;

let proper_subset = fun a b ->
    if subset a b AND length a < length b then true else false;;

let equal_sets = fun a b ->
    subset a b && subset b a;;

let rec set_diff = fun a b c->
    match a with
        | []->c
        | h::t->if contains h b 
            then set_diff t b c
            else set_diff t b (h::c);;

let rec computed_fixed_point = fun eq f x->
    if eq (f x) x then x
    else computed_fixed_point eq f (f x);;

let rec compute_rec_func = fun f p x->
    if p > 0 then compute_rec_func f (p-1) (f x) else x;;

let rec computed_periodic_point = fun eq f p x->
    if eq (compute_rec_func f p x) x then x
    else computed_periodic_point eq f p (f x);;

let rec filter_blind_alleys = fun g->

    let check_term = fun sym r->
        match sym with
            | T->true
            | N->if contains (sym,_) (filter_blind_alleys r) then true
                else false;; in

        let rec check_rhs_term = fun rhs q->
            match rhs with
                | []->true
                | h::t-> if check_term h q then check_rhs_term t g
                    else false;; in

    match g with
        | []->g
        | (lhs,rhs)::t-> if check_rhs_term rhs q then (lhs,rhs)::(filter_blind_alleys t)
            else (filter_blind_alleys t);;


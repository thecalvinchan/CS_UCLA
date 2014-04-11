let rec contains = fun a b ->
    match b with
        | []->false
        | h::t -> if a = h then true else contains a t;;

let rec subset = fun a b ->
    match a with
        | []->true
        | h::t->if contains h b then subset t b else false;;

let proper_subset = fun a b ->
    (subset a b) && not (subset b a);;

let equal_sets = fun a b ->
    subset a b && subset b a;;

let rec set_diff = fun a b->
    match a with
        | []->[]
        | h::t->if contains h b 
            then set_diff t b
            else h::(set_diff t b);;

let rec computed_fixed_point = fun eq f x->
    if eq (f x) x then x
    else computed_fixed_point eq f (f x);;

let rec compute_rec_func = fun f p x->
    if p > 0 then compute_rec_func f (p-1) (f x) else x;;

let rec computed_periodic_point = fun eq f p x->
    if eq (compute_rec_func f p x) x then x
    else computed_periodic_point eq f p (f x);;

type ('terminal,'nonterminal) symbol = T of 'terminal | N of 'nonterminal;;

let equal_rules (r1,a) (r2,b) = equal_sets a b;;
 
let terminates = fun x nt-> 
    match x with 
        | T x->true
        | N x->contains x nt
;;
 
let rec validate = fun rhs nt->
    match rhs with
        | []-> true
        | h::t->if terminates h nt then validate t nt
            else false
;;
 
let rec find_term_nonterms = fun g nt->
    match g with
        | [] -> nt
        | (lhs,rhs)::t ->
            if validate rhs nt && not (contains lhs nt) 
            then find_term_nonterms t (lhs::nt)
            else find_term_nonterms t nt
;;
 
let filter (g,nt) = (g, find_term_nonterms g nt);;
 
let rec order = fun g nt->
    match g with 
        | [] -> []
        | (lhs,rhs)::t ->
            if validate rhs nt then (lhs,rhs)::(order t nt)
            else order t nt
;;
 
let filter_blind_alleys = fun g->
    match g with
        | (a,b) -> 
            (a, order b (snd (computed_fixed_point (equal_rules) (filter) (b,[]))))
;;

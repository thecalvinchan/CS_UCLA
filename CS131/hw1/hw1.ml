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

let rec contain_tuple = fun a b ->
    match b with
        | []->false
        | (rs,ls)::t -> if a = rs then true else contain_tuple a t;;

let rec filter_grammar = fun g ts->
    let check_ter = fun sym next->
        match sym with
            | T sym->true
            | N sym->contains sym ts || contain_tuple sym (filter_grammar next ts)
    in
    let rec filter_rules = fun r ng->
        match r with
            | []->true
            | h::t->if check_ter h ng then filter_rules t ng
                else false
    in
    match g with
        | []->g
        | (ls,rs)::tail->if filter_rules rs tail then (ls,rs)::(filter_grammar tail (ls::ts))
            else filter_grammar tail ts;;
 
let rec filter_blind_alleys = fun g->
    match g with
        | (ls,rs)->(ls, filter_grammar rs []);;

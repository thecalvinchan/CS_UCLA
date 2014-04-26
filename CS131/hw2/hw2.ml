type ('nonterm, 'term) symbol = N of 'nonterm | T of 'term;;

let rec prod_function = fun sym rules ->
    match rules with 
    | [] -> []
    | (ls,rs)::t -> if ls = sym then rs::(prod_function sym t)
        else prod_function sym t;;

let convert_grammar = fun gram1 ->
    ((fst gram1, function sym -> prod_function sym (snd gram1)));;
    (* must be passed a correct hw1 grammar *)

let rec gen_rhs = fun rules sym acceptor path frag -> 
    let rec match_right = fun rules rs acceptor path frag ->
        match frag with 
            | [] -> if rs = [] then acceptor path frag 
                else None
            | h::t -> if rs = [] then acceptor path frag
                else match rs with
            | [] -> None
            | (N x)::tl -> gen_rhs rules x (match_right (rules) tl acceptor) path frag
            | (T x)::tl -> if h = x then match_right rules tl acceptor path t
                else None
    in
    let rec check_rs = fun rules sym rs acceptor path frag ->
        if List.length rs <= 0 then None
        else match rs with 
            | [] -> None
            | h::t -> 
                match match_right (rules) h acceptor (List.append path [(sym,h)]) frag with
                | None -> check_rs rules sym t acceptor path frag
                | Some (p, f) -> Some (p, f)
    in
    check_rs rules sym (rules sym) acceptor path frag;;
 
let parse_prefix gram acceptor frag =
    gen_rhs (snd gram) (fst gram) acceptor [] frag;;

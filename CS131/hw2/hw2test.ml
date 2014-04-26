let accept_all derivation string = Some (derivation, string)
let accept_empty_suffix derivation = function
   | [] -> Some (derivation, [])
   | _ -> None

(* An example grammar for a small subset of Awk, derived from but not
   identical to the grammar in
   <http://www.cs.ucla.edu/classes/winter06/cs132/hw/hw1.html>.
   Note that this grammar is not the same as Homework 1; it is
   instead the same as the grammar under "Theoretical background"
   above.  *)

type awksub_nonterminals =
  | Expr | Term | Lvalue | Incrop | Binop | Num

let awkish_grammar =
  (Expr,
   function
     | Expr ->
         [[N Term; N Binop; N Expr; N Binop; N Expr];
          [N Term]]
     | Term ->
     [[N Num];
      [N Lvalue];
      [N Incrop; N Lvalue];
      [N Lvalue; N Incrop];
      [T"("; N Expr; T")"]]
     | Lvalue ->
     [[T"$"; N Expr]]
     | Incrop ->
     [[T"++"];
      [T"--"]]
     | Binop ->
     [[T"+"];
      [T"-"]]
     | Num ->
     [[T"0"]; [T"1"]; [T"2"]; [T"3"]; [T"4"];
      [T"5"]; [T"6"]; [T"7"]; [T"8"]; [T"9"]])

let rec contains_lvalue = function
  | [] -> false
  | (Lvalue,_)::_ -> true
  | _::rules -> contains_lvalue rules

let accept_only_non_lvalues rules frag =
  if contains_lvalue rules
  then None
  else Some (rules, frag)

let test_1 = 
  ((parse_prefix awkish_grammar accept_only_non_lvalues
      ["3"; "-"; "$"; "9"; "7"; "8"])
   = Some ([(Expr, [N Term]); (Term, [N Num]); (Num, [T "3"])], ["-"; "$"; "9"; "7"; "8"]))

let test_2 =
  ((parse_prefix awkish_grammar accept_only_non_lvalues
      ["2"; "+"; "3"; "-"; "1"; "+"; "$"; "1"; "-"; "8"])
   = Some ([(Expr, [N Term; N Binop; N Expr; N Binop; N Expr]); (Term, [N Num]); (Num, [T "2"]);
    (Binop, [T "+"]); (Expr, [N Term]); (Term, [N Num]); (Num, [T "3"]); (Binop, [T "-"]); 
    (Expr, [N Term]); (Term, [N Num]); (Num, [T "1"])],
    ["+"; "$"; "1"; "-"; "8"]))

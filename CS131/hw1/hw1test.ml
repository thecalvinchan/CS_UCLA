let subset_test0 = subset [2;3] [1;2;3;7;9]
let subset_test1 = subset [3;1;3] [1;2;3;4;5]
let subset_test2 = not (subset [1;3;0] [2;1;5])

let proper_subset_test0 = proper_subset [2;3] [1;2;3]
let proper_subset_test1 = proper_subset [3;1;3;2] [1;2;3;4]
let proper_subset_test2 = not (proper_subset [3] [3;3])

let equal_sets_test0 = equal_sets [1;3] [3;1;3;1]
let equal_sets_test1 = not (equal_sets [1;2;3;4] [3;1;3])

let set_diff_test0 = equal_sets (set_diff [3;4] [1;4;3;1]) []
let set_diff_test1 = equal_sets (set_diff [4;3;1;3] [1;3]) [4]
let set_diff_test2 = equal_sets (set_diff [4;2;1] []) [1;2;4]
let set_diff_test3 = equal_sets (set_diff [] [4;7;1]) []

let computed_fixed_point_test0 =
  computed_fixed_point (=) (fun x -> x / 2) 4096 = 0
let computed_fixed_point_test1 =
  computed_fixed_point (=) (fun x -> x *. 5.) 5. = infinity
let computed_fixed_point_test2 =
  computed_fixed_point (=) sqrt 10. = 1.
let computed_fixed_point_test3 =
  ((computed_fixed_point (fun x y -> abs_float (x -. y) < 1.)
             (fun x -> x /. 2.)
             10.)
   = 1.25)

let computed_periodic_point_test0 =
  computed_periodic_point (=) (fun x -> x / 2) 0 (-1) = -1
let computed_periodic_point_test1 =
  computed_periodic_point (=) (fun x -> x *. x -. 1.) 2 0.5 = -1.

type awksub_nonterminals =
  | Expr | Lvalue | Incrop | Binop | Num

let awksub_rules =
   [Expr, [T"("; N Expr; T")"];
    Expr, [N Num];
    Expr, [N Expr; N Binop; N Expr];
    Expr, [N Lvalue];
    Expr, [N Incrop; N Lvalue];
    Expr, [N Lvalue; N Incrop];
    Lvalue, [T"$"; N Expr];
    Incrop, [T"++"];
    Incrop, [T"--"];
    Binop, [T"+"];
    Binop, [T"-"];
    Num, [T"0"];
    Num, [T"1"];
    Num, [T"2"];
    Num, [T"3"];
    Num, [T"4"];
    Num, [T"5"];
    Num, [T"6"];
    Num, [T"7"];
    Num, [T"8"];
    Num, [T"9"]]

let awksub_grammar = Expr, awksub_rules

let awksub_test0 =
  filter_blind_alleys awksub_grammar = awksub_grammar

let awksub_test1 =
  filter_blind_alleys (Expr, List.tl awksub_rules) = (Expr, List.tl awksub_rules)

let awksub_test2 =
  filter_blind_alleys (Expr, List.tl (List.tl awksub_rules)) =
    (Expr,
     [Incrop, [T "++"];
      Incrop, [T "--"];
      Binop, [T "+"];
      Binop, [T "-"];
      Num, [T "0"]; Num, [T "1"]; Num, [T "2"]; Num, [T "3"]; Num, [T "4"];
      Num, [T "5"]; Num, [T "6"]; Num, [T "7"]; Num, [T "8"]; Num, [T "9"]])

let awksub_test3 =
  filter_blind_alleys (Expr, List.tl (List.tl (List.tl awksub_rules))) =
    filter_blind_alleys (Expr, List.tl (List.tl awksub_rules))

type giant_nonterminals =
  | Conversation | Sentence | Grunt | Snore | Shout | Quiet

let giant_grammar =
  Conversation,
  [Snore, [T"zzz"];
   Quiet, [];
   Grunt, [T"grunt"];
   Shout, [T"shout!"];
   Sentence, [N Quiet];
   Sentence, [N Grunt];
   Sentence, [N Shout];
   Conversation, [N Snore];
   Conversation, [N Sentence; T","; N Conversation]]

let giant_test0 =
  filter_blind_alleys giant_grammar = giant_grammar

let giant_test1 =
  filter_blind_alleys (Sentence, List.tl (snd giant_grammar)) =
    (Sentence,
     [Quiet, []; Grunt, [T "grunt"]; Shout, [T "shout!"];
      Sentence, [N Quiet]; Sentence, [N Grunt]; Sentence, [N Shout]])

let giant_test2 =
  filter_blind_alleys (Sentence, List.tl (List.tl (snd giant_grammar))) =
    (Sentence,
     [Grunt, [T "grunt"]; Shout, [T "shout!"];
      Sentence, [N Grunt]; Sentence, [N Shout]])

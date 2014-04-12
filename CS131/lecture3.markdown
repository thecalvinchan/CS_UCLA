OCaml & currying

    let cons1 = fun x -> 1::x
    int list -> int list

## Matching 

    let cur h::t -> h
    let cur = function h::t->h
    let cur = fun x -> match x with h:: _ ->h

    match x with 
    | h:: _ ->h
    | [a;b;c] ->b
    | x->12

    a::b->a
    x->3::x

Patterns 
constant
var 
P1,P2,P3 (patterns)
P1::P2 list head p! tail p2
[P1;P2;P3] list with effs P1..P3

match [3,9,12] with [a;b;c;d] -> d+a
a::b::c::d::e::f -> f
[] -> 12
a::b -> a
x->3::x

    $ocaml
    #let cur h::_ = h
    WARNING: this expression isn't fully matched
    Here's a counter example: []
    var cur a list -> a = <fun>
    # cur [];;
    (runtime error)
    # Write robust code that works for this cases

    let car1 = fun x -> math x with
    | h::_->h
    | []->0

    let cur1 = function
    | h::_->h
    | []->0

    let cur2 d = function
    'a -> 'a list -> a
    | []->d
    | h::_->h
    # cur2 "abc" [3;9;12]
    # typeError

##Recursion

    let rec length = function
    'a list -> int
    | []->0
    | _::t->length t + 1

    let rec reverse = function
    'a list list -> 'a list = <fun>
    | []->[]
    | h::t->reverse t @ h
    
    # bug! types are wrong

    let rec reverse = function
    'a list list -> 'a list = <fun>
    | []->[]
    | h::t->reverse t @ [h]

    # Cost O(|t|)
    # Recursively O(N^2)

    # revapp L A = (reverse L)@A
    let rec revapp l a = match l with
    | [] -> a
    | h::t-> revapp t (h::a)

    let rec revapp a = function
    | [] -> a
    | h::t -> revapp (h::a) t
    
    let reverse = revapp []

Using an accumulator. Accumulates the work and keeps track of all the work already done.

    let rec revapp = 
        fun a ->
            fun l->
                match l with
                | [] -> a
                | h::t -> revapp (h::a) t

    fun x->x+1
    function x->x+1
    fun (x,y) -> x+y
    function (x,y) -> x+y

    # fun is for currying
    # fun x y -> x+y
    # function is for multiple pattern matching
    # function  | (0,y)-> y
                | (x;_)-> x

    let rec minlist = function
        | h::t -> min h (minlist t)
        | [] -> 999999999999
    # we get int list -> int
    # but we want 'a lsit -> 'a
    let min a b = 
        if a<b then a else b
    # 'a -> 'a -> 'a

    let rec minlist maxval min = function
    | h::t -> min h (minlist maxval min t)
    | [] -> maxval

    # 'a -> ('a->'a->'a) -> 'a list -> 'a
    minlist (1e300*1e300)
        max [-2.0;5.7;1e9]

##Defining your own types

Discriminated unions: represented as a pointer to the start of a piece of storage. Everything that follows is specific to the type we are talking about

    type foobar = 
        # constructors
        # can use constructor as if it were a function
        | Foo of int
        | Bar of string * int
        | Baz of int list
    match fb with 
        | Foo n -> n
        | Bar (_,m) -> m
        | Baz (h::_) -> h
        | _ -> 0

    type 'a foobar = 
        | Foo of 'a
        | Bar of string*'a
        | Baz of 'a list

    type 'a option = 
        | None
        | Some of 'a

    Some[1;3;5]
    #int list option
    None
    #'a option

    type 'a list = 
        | []
        | :: of 'a * 'a list

    type 'a,'b mylist =
        | Empty
        | Cons of 'a * 'a mylist

#Syntax

__syntax__: form independent of meaning

- "Colorless green ideas sleep furiously" (form good meaning absent)
- "Ireland has leprechauns galore." (meaning is well understood, syntax is nonstandard)
- more complicated than you think, unfortunately

Tokens
    bottom level
        10110110110110001
        break into bytes cause we don't like bits
        
        printf("Japanese Characters\n");
            Use encoding (UTF-8)

        7-bit c
        11-bit c
        16-bit c
        beginning bits identify how many bytes are remaining

Tokenization: need to know what is not a token

    - white space
    - comments
    - trigraphs

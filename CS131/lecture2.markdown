    int fd = open (...);
    if (fd >=0) {...}
    if (close(fd)<0) {
        // Different on Windows and Unix
    }

close(fd) returns -1 on failure on everything except Windows
On windows, close(fd) throws an exception

gcc -I ...
./uninstal.h

    #include "ownstuff.h"
    #include "/usr/include/uninstal.h"
    #ifdef __WINDOWS__
    #include "C:/..."

    static int rpl_close(int fd) {
        TRY
            n = close(fd);
        CATCH
            n = -1;
            errno = EBADF;
        DONE
        return n;
    }
    #undef close
    #define close rpl_close

    #include "/usr/include/uninstall.h"
    #endif

CAN YOU DO THIS IN C++?? 
Might be on Midterm/Final

##Successful languages evolve

BASIC on GE 225 mainframe
    40 microsec to add.
    500 microsec to divide
    ~ 40KB RAM
    20 simultaneous users
BASIC interpreter
GE 225 machine code
~ 5KB of RAM
time shared
2 char. identifier limit
    $_limit string
    I-N => int
    others => float

PDP11 (c.1975)
4 microsec to add
16KB RAM
1.2 microsec memory cycle time
C originated on this machine

int i;
char * p;
p[i] = * (p+i)

        1975            2014
i + 10  4 microsec      1 nanosec
* p     1.2 microsec    100 nanosec

Big backend queries in Google
Functional Model, stolen from lisp/ML/Haskell
MapReduce

    - Map: performs action on list, returns list
    - Reduce: reduces a list to fewer items

__imperative languages__: based on commands. sequencial actions. most common actions are assignment. C/C++, Java, Python
__functional languages__: based on function calls. actions based on functions F(G(x),H(y,z)). No state, only values. Lisp, Scheme, ML, OCaml, Haskell, F#
__logical languages__: based on goals. blue(x) & green(y)... x has to be blue and y has to be green. No assignment. Prolog, Mercury

Lots of different ways to categorize languages. Can filter based on OOP or not-OOP, scripting or compiled, etc.

##Functional programming

J. Backus Fortran ~1956 imperative
motivation for something better

- parallelisability, optimization
- clarity
- rebellion against Von Neumann style of computing (bottleneck)

###Functional basics

__function__: mapping from a domain to a range
__functional forms__: functions where either the domain or range is a function
__evaluation order__: not controlled by sequencing order, rather recursion
__no side effects__: no change to any variable value. no I/O or device change
__referential transparency__: every time you refer to a value or expression, what you see is what you get

##OCaml

###Basic Properties

- functional core
- compile-time type checking (like Java, C/C++)
- need not write down the types all the time (like Python, Scheme)
- no need to worry about storage management
- good support for higher-order functions

    $ocaml
    # 3+4*5;;
    - : int = 23
    # let x = 37*32;;
    x : int = 1369
    # if 1 < x then "a" else "b";;
    - : string = "a"
    # 0.3 *. 12.0
    # 1,"x";;
    - : int * string = (1,"x")
    # [1;2;9]
    - : int list = [1,2,9] 4
    # []
    - : 'a list = []

###Operators
h::t => "cons"
    appends h to beginning of t
t@u => "concat"
    concats u to end of t

###Functions

    # fun x -> x + 1;;
    - : int -> int = <fun>
    # let plus1 = fun x -> x + 1;;
    plus1 : int -> int = <fun>
    # plus1 27;;
    - : int = 28
    # (fun x -> x + 5) 12;;

__anonymous function__: functions without names "lambda expression"

    # let cons (x,y) = x::y;;
    cons : 'a * 'a list -> 'a list = <fun>
    # cons (3,[5;12])
    # let ccons = fun x -> fun y -> x::y;;
    ccons : 'a -> 'a list -> 'a list = <fun>
    #ccons 3 [4;12]
    #ccons 3;;
    - : int list -> int list = <fun>

__currying__: functions that return functions

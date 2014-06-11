iterate_all(_, []).
iterate_all(Function, [Head|Tail]):-
        call(Function, Head), 
        iterate_all(Function, Tail).
 
check_dom(N, L):-
        fd_domain(L, 1, N).

row_len(N, L):-
        length(L, N).
 
get_el(T, I-J, V):-
        nth(I, T, Row), 
        nth(J, Row, V).

trans([], []).
trans([F|Fs], T):-
        trans(F, [F|Fs], T).
 
trans([], _, []).
trans([_|R], M, [T|Ts]):-
        lists_firsts_rests(M, T, Ms), 
        trans(R, Ms, Ts).
 
lists_firsts_rests([], [], []).
lists_firsts_rests([[F|Os]|Rest], [F|Fs], [Os|Oss]):-
    lists_firsts_rests(Rest, Fs, Oss).
 
% fdomain kenken implementation
 
add_list(T, [], Res, Res).
add_list(T, [H|L], S, Res):-
    get_el(T, H, X), 
    Sum #= S + X, 
    add_list(T, L, Sum, Res).
 
mult_list(T, [], Res, Res).
mult_list(T, [H|L], P, Res):-
    get_el(T, H, X), 
    Prod #= P * X, 
    mult_list(T, L, Prod, Res).
 
calc(T, +(Res, List)):-
    add_list(T, List, 0, Res).
calc(T, *(Res, List)):-
    mult_list(T, List, 1, Res).
calc(T, /(Res, A, B)):-
    get_el(T, A, X), 
    get_el(T, B, Y), 
    (X * Res #= Y ; Y * Res #= X).
calc(T, -(Res, A, B)):-
    get_el(T, A, X), 
    get_el(T, B, Y), 
    (Res #= X - Y ; Res #= Y - X).
 
kenken(N, C, T):-
    length(T, N), 
    iterate_all(row_len(N), T), 
    iterate_all(check_dom(N), T), 
    iterate_all(fd_all_different, T), 
    trans(T, XTrans), 
    iterate_all(fd_all_different, XTrans), 
    iterate_all(calc(T), C), 
    iterate_all(fd_labeling, T), 
    statistics.
 
% plain kenken implementation
 
range(N, L):-
    findall(X, between(1, N, X), L).
 
check_doplain([], _).
check_doplain([H|T], R):-
    member(H, R), 
    check_doplain(T, R).
 
is_set(L):-
    setof(X, member(X, L), S), 
    length(L, N), 
    length(S, N).
 
check_dom_plain([], _).
check_dom_plain([H|T], R):-
    check_doplain(H, R), 
    is_set(H), 
    check_dom_plain(T, R).
 
add_list_plain(T, [], Res, Res).
add_list_plain(T, [H|L], S, Res):-
    get_el(T, H, X), 
    Sum is S + X, 
    add_list_plain(T, L, Sum, Res).
 
mult_list_plain(T, [], Res, Res).
mult_list_plain(T, [H|L], P, Res):-
    get_el(T, H, X), 
    Prod is P * X, 
    mult_list_plain(T, L, Prod, Res).
 
calc_plain(T, +(Res, List)):-
    add_list_plain(T, List, 0, Res).
calc_plain(T, *(Res, List)):-
    mult_list_plain(T, List, 1, Res).
calc_plain(T, /(Res, A, B)):-
    get_el(T, A, X), 
    get_el(T, B, Y), 
    (Res is X // Y ; Res is Y // X).
calc_plain(T, -(Res, A, B)):-
    get_el(T, A, X), 
    get_el(T, B, Y), 
    (Res is X - Y ; Res is Y - X).
 
plain_kenken(N, C, T):-
    length(T, N), 
    range(N, R), 
    iterate_all(row_len(N), T), 
    check_dom_plain(T, R), 
    trans(T, XTrans), 
    iterate_all(is_set, XTrans), 
    iterate_all(calc_plain(T), C), 
    statistics.
 
% test case 
 
kenken_testcase(
    4, 
    [
        *(12, [1-1, 2-1]), 
        -(2, 1-2, 1-3), 
        +(2, [1-4]), 
        -(1, 2-2, 3-2), 
        /(2, 2-3, 3-3), 
        -(1, 2-4, 3-4), 
        +(1, [3-1]), 
        /(2, 4-1, 4-2), 
        +(4, [4-3, 4-4])
    ]
).

type tree = Int of int | Mem of tree | Move of tree * tree

let rec traverse = function
  | Int n -> print_int n
  | Mem exp ->
    print_string "Mem(";
    traverse exp;
    print_string ")"
  | Move (dst, src) ->
    print_string "Move(";
    traverse dst;
    print_string ",";
    traverse src;
    print_string ")"
;;

let matcher = function
  | Mem t ->
    print_string "Mem! ";
    traverse (Mem t);
    print_newline ()
  | Move (dst, Mem t) ->
    print_string "Move with src Mem! ";
    traverse (Move (dst, Mem t));
    print_newline ()
  | t ->
    print_string "Wildcard ";
    traverse t;
    print_newline ()
;;

let () =
  let i1 = Int 42 in
  let i2 = Int 21 in
  let mem = Mem i1 in
  let move1 = Move (i2, mem) in
  let move2 = Move (i2, i1) in
  matcher i1;
  matcher move1;
  matcher move2;
  matcher mem
;;

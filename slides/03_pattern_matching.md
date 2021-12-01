Pattern-matching for dummies
============================

### OCaml > all

![A camel](img/gringo.jpg){width=20%}

\pause

- **OCaml** is the best language taught in EPITA
\pause
- Fight me
\pause

One particularly nice feature: its **pattern-matching** system.

### Don't trust me?

![It's literally written in the
doc](img/fonctionnalite_vraiment_cool.png){width=80%}

### Basic matching

```ocaml
let is_the_answer n =
  match n with
  | 42 -> "The answer"
  | _ -> "Not the answer"
;;

let () = print_endline (is_the_answer 42)
```

### Algebraic types

Algebraic types are **composite types** common in functional programming. There
are two main types:

- **product types**: type $\alpha$ *and* type $\beta$ *and*...
- **sum type**: type $\alpha$ *or* type $\beta$ *or*...

\pause

In more common terms, a tuple-like type or a union-like type.

### An example

```ocaml
type a_or_b = A | B

let string_of_a_or_b x =
  match x with
  | A -> "A"
  | B -> "B"
;;

let () = print_endline (string_of_a_or_b A) (* "A" *)
```

### It gets better

```ocaml
type a_or_b = A | B of int

let string_of_a_or_b x =
  match x with
  | A -> "A"
  | B n when n = 42 -> "The answer"
  | B _ -> "B"
;;

let () = print_endline (string_of_a_or_b (B 21)) (* "B" *)
```

### The killer feature

OCaml pattern-matching is **very expressive**, thanks to:

- **wildcard** patterns
\pause
- **guards** to check specific conditions
\pause
- **destructuration** to match on inner fields

\pause
\vfill
#### Compiler safety
Thanks to exhaustiveness checks, you cannot write a `match` with unusable or
missing cases. This greatly reduces the number of errors you could write.

### Back to our problem

Let's put it all in practice!

```ocaml
type tree = Int of int | Mem of tree | Move of tree * tree
let rec traverse = function ... (* print the tree recursively *)

let matcher = function
  | Mem t ->
    print_string "Mem! "; traverse (Mem t)
  | Move (dst, Mem t) ->
    print_string "Move with src Mem! "; traverse (Move (dst, Mem t))
  | t ->
    print_string "Wildcard "; traverse t
;;
```

\pause
Last OCaml example, I swear

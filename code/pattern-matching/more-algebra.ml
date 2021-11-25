type a_or_b = A | B of int

let string_of_a_or_b x =
  match x with
  | A -> "A"
  | B n when n = 42 -> "The answer"
  | B _ -> "B"
;;

let () = print_endline (string_of_a_or_b (B 21)) (* "B" *)

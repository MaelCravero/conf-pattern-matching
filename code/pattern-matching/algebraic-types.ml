type a_or_b = A | B

let string_of_a_or_b x =
  match x with
  | A -> "A"
  | B -> "B"


let () = print_endline (string_of_a_or_b A) (* "A" *)

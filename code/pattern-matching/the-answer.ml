let is_the_answer n =
  match n with
  | 42 -> "The answer"
  | _ -> "Not the answer"


let () = print_endline (is_the_answer 42) (* "The answer" *)

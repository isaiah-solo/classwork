(* $Id: trimzeros.ml,v 1.1 2012-02-10 17:34:29-08 - - $ *)

(*
* Trim off zeros from the end of a list.  If the list is a number
* represented in reveerse order, this trims high-order digits, as
* would be needed after a subtraction.
*)

let trimzeros list =
    let rec trimzeros' list' = match list' with
        | []       -> []
        | [0]      -> []
        | car::cdr ->
             let cdr' = trimzeros' cdr
             in  match car, cdr' with
                 | 0, [] -> []
                 | car, cdr' -> car::cdr'
    in trimzeros' list
;;

trimzeros [4;3;2;1;0;0;0;0];;

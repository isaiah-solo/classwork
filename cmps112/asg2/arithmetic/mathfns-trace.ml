(* $Id: mathfns-trace.ml,v 1.1 2012-02-07 19:44:53-08 - - $ *)

(*
* Power and square root functions.
* Uses tail recursive accumulator-style coding.
*)

let even number = number mod 2 = 0
let frexp' = frexp (* So we can trace these functions. *)
let ldexp' (frac, expt) = ldexp frac expt

let rec power' (base, expt, result) = match expt with
    | 0                   -> result
    | expt when even expt -> power' (base *. base, expt / 2, result)
    | expt                -> power' (base, expt - 1, base *. result)

let power (base, expt) =
    if expt < 0 then power' (1. /. base, - expt, 1.)
                else power' (base, expt, 1.)

let rec sqrt' (number, approx) =
    let next = (approx +. number /. approx) /. 2.
    in  if abs_float (next -. approx) /. approx <= epsilon_float *. 2.
        then approx
        else sqrt' (number, next)

let sqrt number =
    if number < 0.
    then raise (Invalid_argument ("sqrt of " ^ string_of_float number))
    else let frac, expt = frexp' number
         in  sqrt' (number, ldexp' (frac, expt / 2))

;;
#trace power  ;;
#trace power' ;;
#trace sqrt   ;;
#trace frexp' ;;
#trace ldexp' ;;
#trace sqrt'  ;;


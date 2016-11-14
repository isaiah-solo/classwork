(* $Id: etc.mli,v 1.1 2009-02-03 17:24:36-08 - - $ *)

(*
* Main program and system access.
*)

val execname : string

val quit : unit -> unit

val eprint : string list -> unit

val lexeprint : Lexing.position -> string list -> unit

val usageprint : string list -> unit


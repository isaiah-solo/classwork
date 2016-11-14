(* $Id: etc.ml,v 1.1 2009-02-03 17:24:36-08 - - $ *)

open Lexing
open Printf

let execname = Filename.basename Sys.argv.(0)

let exit_code_ref = ref 0

let quit () = exit !exit_code_ref

let eprintlist message =
    (exit_code_ref := 1;
     flush_all ();
     List.iter (fprintf stderr "%s") message;
     fprintf stderr "\n";
     flush_all ())

let eprint message = eprintlist (execname :: ": " :: message)

let lexeprint position message =
    eprint (position.pos_fname :: ": "
            :: string_of_int position.pos_lnum :: ": "
            :: message)

let usageprint message =
    eprintlist ("Usage: " :: execname :: " " :: message)


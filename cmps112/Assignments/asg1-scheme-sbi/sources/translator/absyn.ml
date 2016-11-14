(* $Id: absyn.ml,v 1.1 2009-02-03 17:24:36-08 - - $ *)

(*
* Abstract syntax definitions for SB->IR.
*)

type linenr     = int
and  variable   = string
and  label      = string
and  number     = float
and  oper       = string
and  arrayfn    = variable * expr

and  print      = Printexpr of expr
                | String of string

and  memref     = Arrayfn of arrayfn
                | Variable of variable

and  expr       = Binop of oper * expr * expr
                | Unop of oper * expr
                | Memref of memref
                | Constant of number

and  stmt       = Dim of arrayfn
                | Let of memref * expr
                | Goto of label
                | If of expr * label
                | Print of print list
                | Input of memref list

and  program    = (linenr * label option * stmt option) list


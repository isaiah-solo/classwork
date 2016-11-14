(* $Id: main.ml,v 1.1 2009-02-03 17:24:36-08 - - $ *)

open Absyn
open Etc
open Lexing
open Printf

(*
* Functions for printing out the absyn tree in Scheme format.
*)

let
rec pr'print file print = match print with
    | Printexpr (expr)           -> fprintf file " %a" pr'expr expr
    | String (string)            -> fprintf file " %s" string

and pr'memref file memref = match memref with
    | Arrayfn (arrayfn)          -> pr'arrayfn file arrayfn
    | Variable (variable)        -> fprintf file "%s" variable

and pr'expr file expr = match expr with
    | Binop (oper, expr1, expr2) -> fprintf file "(%s %a %a)"
                                    oper pr'expr expr1 pr'expr expr2
    | Unop (oper, expr)          -> fprintf file "(%s %a)"
                                    oper pr'expr expr
    | Memref (memref)            -> pr'memref file memref
    | Constant (number)          -> fprintf file "%.15g" number

and pr'stmt file stmt = match stmt with
    | None                       -> ()
    | Some (Dim (arrayfn))       -> fprintf file "(dim %a)"
                                    pr'arrayfn arrayfn
    | Some (Let (memref, expr))  -> fprintf file "(let %a %a)"
                                    pr'memref memref pr'expr expr
    | Some (Goto (label))        -> fprintf file "(goto %s)" label
    | Some (If (expr, label))    -> fprintf file "(if %a %s)"
                                    pr'expr expr label
    | Some (Print (prints))      -> fprintf file "(print%a)"
                                    pr'prints prints
    | Some (Input (memrefs))     -> fprintf file "(input%a)"
                                    pr'inputs memrefs

and pr'arrayfn file (variable, expr) =
    fprintf file "(%s %a)" variable pr'expr expr

and pr'line file (line, label, stmt) =
    let s'label = match label with
        | None         -> ""
        | Some (label) -> label
    in  fprintf file "(%5d %-8s %a)\n" line s'label pr'stmt stmt

and pr'input   file inputs  = fprintf file " %a" pr'memref inputs

and pr'prints  file prints  = List.iter (pr'print file) prints

and pr'inputs  file memrefs = List.iter (pr'input file) memrefs

and pr'lines   file lines   = List.iter (pr'line file) lines

and pr'program file program = fprintf file "(\n%a)\n" pr'lines program

(*
* Main program reads a file and prints to stdout.
*)

let translatefile filename =
    try (printf ";;File: %s\n" filename;
         let sourcefile =
             if filename = "-" then stdin else open_in filename in
         let lexbuf = Lexing.from_channel sourcefile in
         let absyn = Parser.program Lexer.token lexbuf in
         printf "%a" pr'program absyn)
    with Sys_error (string) -> eprint [string]

let _ = if !Sys.interactive
        then ()
        else match Array.length Sys.argv with
             | 1 -> translatefile "-"
             | 2 -> translatefile Sys.argv.(1)
             | _ -> usageprint ["[filename.sb]"]


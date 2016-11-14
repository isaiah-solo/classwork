#!/bin/env ocaml
(* $Id: registers.ml,v 1.2 2015-10-22 13:18:39-07 - - $ *)

(*
 * Show how to load and store registers using an array.
 * Indices are just characters.
 *)

open Printf
let aget = Array.get
let aset = Array.set
let amake = Array.make
let ord = Char.code

type command = Load of char | Store of char * float

let command_list = [
    Load '=';
    Store ('a', 3.1415926535897932384626433832795029);
    Load 'a';
    Store ('m', 2.7182818284590452353602874713526625);
    Load 'm';
    Load '?'
]

let symbol_table = amake 256 (false, 0.0)

let do_command = function
    | Load sym -> (
         let entry = aget symbol_table (ord sym)
         in match entry with
            | false, _ -> printf "register '%c' is empty\n%!" sym
            | true, value -> printf "%c = %g\n%!" sym value
      )
    | Store (sym, value) -> (
         aset symbol_table (ord sym) (true, value);
         printf "%c := %g\n%!" sym value
      )

let _ = List.iter do_command command_list


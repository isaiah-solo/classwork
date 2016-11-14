(* $Id: lexer.mll,v 1.1 2009-02-03 17:24:36-08 - - $ *)

{
(******** BEGIN LEXER SEMANTICS ********)

open Absyn
open Etc
open Lexing
open Parser
open Printf

let lexerror lexbuf =
    lexeprint (lexeme_start_p lexbuf)
              ["invalid character `" ^ (lexeme lexbuf) ^ "'"]

let newline lexbuf =
    let incrline pos =
        {pos with pos_lnum = pos.pos_lnum + 1; pos_bol = pos.pos_cnum}
    in  (lexbuf.lex_start_p <- incrline lexbuf.lex_start_p;
         lexbuf.lex_curr_p <- incrline lexbuf.lex_curr_p)

let list lexbuf =
    let pos = lexeme_start_p lexbuf
    in  (if pos.pos_bol = pos.pos_cnum
         then printf ";;%4d: " pos.pos_lnum;
         printf "%s" (lexeme lexbuf))

let keylist = [
        "dim"  , DIM   ;
        "goto" , GOTO  ;
        "if"   , IF    ;
        "input", INPUT ;
        "let"  , LET   ;
        "print", PRINT ;
    ]

let keyhash : (string, token) Hashtbl.t
            = Hashtbl.create (List.length keylist)

let _ = List.iter (fun (word, token) -> Hashtbl.add keyhash word token)
                   keylist

let identkeyword ident =
    try Hashtbl.find keyhash ident
    with Not_found -> IDENT ident

(******** END LEXER SEMANTICS ********)
}

let letter          = ['a'-'z' 'A'-'Z' '_']
let digit           = ['0'-'9']
let fraction        = (digit+ '.'? digit* | '.' digit+)
let exponent        = (['E' 'e'] ['+' '-']? digit+)

let comment         = ('#' [^'\n']*)
let ident           = (letter (letter | digit)*)
let number          = (fraction exponent?)
let string          = '"' [^'\n' '"']* '"'

rule token          = parse
    | eof           { EOF }
    | [' ' '\t']    { list lexbuf; token lexbuf }
    | comment       { list lexbuf; token lexbuf }
    | "\n"          { list lexbuf; newline lexbuf; EOL }
    | ":"           { list lexbuf; COLON }
    | ","           { list lexbuf; COMMA }
    | "("           { list lexbuf; LPAR }
    | ")"           { list lexbuf; RPAR }
    | "="           { list lexbuf; EQUAL (lexeme lexbuf) }
    | "<>"          { list lexbuf; RELOP (lexeme lexbuf) }
    | "<"           { list lexbuf; RELOP (lexeme lexbuf) }
    | "<="          { list lexbuf; RELOP (lexeme lexbuf) }
    | ">"           { list lexbuf; RELOP (lexeme lexbuf) }
    | ">="          { list lexbuf; RELOP (lexeme lexbuf) }
    | "+"           { list lexbuf; ADDOP (lexeme lexbuf) }
    | "-"           { list lexbuf; ADDOP (lexeme lexbuf) }
    | "*"           { list lexbuf; MULOP (lexeme lexbuf) }
    | "/"           { list lexbuf; MULOP (lexeme lexbuf) }
    | "%"           { list lexbuf; MULOP (lexeme lexbuf) }
    | "^"           { list lexbuf; POWOP (lexeme lexbuf) }
    | number        { list lexbuf; NUMBER (lexeme lexbuf) }
    | string        { list lexbuf; STRING (lexeme lexbuf) }
    | ident         { list lexbuf; identkeyword (lexeme lexbuf) }
    | _             { list lexbuf; lexerror lexbuf; token lexbuf }


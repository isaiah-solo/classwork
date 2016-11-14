type token =
  | RELOP of (string)
  | EQUAL of (string)
  | ADDOP of (string)
  | MULOP of (string)
  | POWOP of (string)
  | IDENT of (string)
  | NUMBER of (string)
  | STRING of (string)
  | COLON
  | COMMA
  | LPAR
  | RPAR
  | EOL
  | EOF
  | DIM
  | LET
  | GOTO
  | IF
  | PRINT
  | INPUT

open Parsing;;
let _ = parse_error;;
# 4 "parser.mly"
(******** BEGIN PARSER SEMANTICS ********)

open Absyn
open Etc
open Lexing

let syntax () = lexeprint (symbol_start_pos ()) ["syntax error"]

let linenr () = (symbol_start_pos ()).pos_lnum

(******** END PARSER SEMANTICS ********)
# 38 "parser.ml"
let yytransl_const = [|
  265 (* COLON *);
  266 (* COMMA *);
  267 (* LPAR *);
  268 (* RPAR *);
  269 (* EOL *);
    0 (* EOF *);
  270 (* DIM *);
  271 (* LET *);
  272 (* GOTO *);
  273 (* IF *);
  274 (* PRINT *);
  275 (* INPUT *);
    0|]

let yytransl_block = [|
  257 (* RELOP *);
  258 (* EQUAL *);
  259 (* ADDOP *);
  260 (* MULOP *);
  261 (* POWOP *);
  262 (* IDENT *);
  263 (* NUMBER *);
  264 (* STRING *);
    0|]

let yylhs = "\255\255\
\001\000\002\000\002\000\002\000\003\000\003\000\003\000\003\000\
\004\000\005\000\005\000\005\000\005\000\005\000\005\000\005\000\
\011\000\011\000\013\000\013\000\012\000\012\000\008\000\008\000\
\010\000\010\000\009\000\009\000\014\000\014\000\015\000\015\000\
\016\000\016\000\016\000\016\000\007\000\006\000\006\000\006\000\
\006\000\006\000\006\000\006\000\000\000"

let yylen = "\002\000\
\002\000\003\000\003\000\000\000\002\000\001\000\001\000\000\000\
\002\000\002\000\004\000\002\000\004\000\002\000\001\000\002\000\
\003\000\001\000\001\000\001\000\003\000\001\000\001\000\001\000\
\003\000\003\000\003\000\001\000\003\000\001\000\003\000\001\000\
\003\000\002\000\001\000\001\000\004\000\001\000\001\000\001\000\
\001\000\001\000\001\000\001\000\002\000"

let yydefred = "\000\000\
\004\000\000\000\045\000\000\000\000\000\038\000\001\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\006\000\
\000\000\003\000\039\000\043\000\040\000\041\000\044\000\042\000\
\000\000\010\000\000\000\024\000\000\000\012\000\000\000\035\000\
\000\000\036\000\000\000\000\000\000\000\030\000\000\000\020\000\
\000\000\014\000\000\000\000\000\016\000\002\000\000\000\000\000\
\000\000\000\000\000\000\000\000\005\000\009\000\000\000\000\000\
\034\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\033\000\000\000\000\000\000\000\
\013\000\029\000\031\000\017\000\021\000\037\000"

let yydgoto = "\002\000\
\003\000\004\000\014\000\015\000\016\000\027\000\028\000\034\000\
\041\000\036\000\042\000\045\000\043\000\037\000\038\000\039\000"

let yysindex = "\004\000\
\000\000\000\000\000\000\001\000\003\255\000\000\000\000\091\255\
\091\255\091\255\048\255\006\255\091\255\026\255\097\255\000\000\
\010\255\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\033\255\000\000\033\255\000\000\046\255\000\000\048\255\000\000\
\048\255\000\000\092\255\034\255\057\255\000\000\071\255\000\000\
\075\255\000\000\076\255\077\255\000\000\000\000\091\255\091\255\
\091\255\048\255\006\255\091\255\000\000\000\000\048\255\048\255\
\000\000\255\254\048\255\048\255\048\255\091\255\048\255\048\255\
\006\255\091\255\028\255\075\255\000\000\075\255\075\255\057\255\
\000\000\000\000\000\000\000\000\000\000\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\068\255\000\000\000\000\000\000\083\255\
\087\255\090\255\093\255\023\255\094\255\000\000\104\255\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\067\255\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\072\255\000\000\025\255\000\000\
\005\255\000\000\003\255\105\255\000\000\000\000\000\000\000\000\
\000\000\000\000\106\255\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\107\255\000\000\108\255\109\255\088\255\
\000\000\000\000\000\000\000\000\000\000\000\000"

let yygindex = "\000\000\
\000\000\000\000\000\000\000\000\106\000\252\255\002\000\250\255\
\253\255\000\000\057\000\060\000\000\000\062\000\226\255\096\000"

let yytablesize = 276
let yytable = "\017\000\
\007\000\061\000\029\000\025\000\001\000\030\000\044\000\035\000\
\031\000\026\000\069\000\006\000\032\000\040\000\019\000\018\000\
\033\000\019\000\054\000\019\000\020\000\021\000\022\000\023\000\
\024\000\032\000\032\000\032\000\032\000\058\000\061\000\044\000\
\074\000\075\000\032\000\015\000\032\000\032\000\046\000\078\000\
\032\000\029\000\025\000\055\000\030\000\044\000\035\000\056\000\
\026\000\062\000\031\000\067\000\068\000\006\000\032\000\070\000\
\071\000\073\000\033\000\044\000\063\000\019\000\020\000\021\000\
\022\000\023\000\024\000\023\000\023\000\023\000\023\000\023\000\
\028\000\028\000\028\000\064\000\023\000\061\000\023\000\023\000\
\008\000\028\000\023\000\028\000\028\000\065\000\066\000\028\000\
\027\000\027\000\027\000\039\000\059\000\060\000\061\000\043\000\
\006\000\027\000\040\000\027\000\027\000\041\000\042\000\027\000\
\019\000\020\000\021\000\022\000\023\000\024\000\047\000\048\000\
\049\000\050\000\051\000\052\000\007\000\022\000\015\000\011\000\
\053\000\076\000\072\000\025\000\026\000\077\000\057\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\005\000\000\000\000\000\000\000\000\000\000\000\006\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\008\000\009\000\
\010\000\011\000\012\000\013\000"

let yycheck = "\004\000\
\000\000\003\001\009\000\008\000\001\000\010\000\013\000\011\000\
\003\001\008\000\012\001\006\001\007\001\008\001\010\001\013\001\
\011\001\013\001\009\001\014\001\015\001\016\001\017\001\018\001\
\019\001\001\001\002\001\003\001\004\001\033\000\003\001\009\001\
\063\000\064\000\010\001\013\001\012\001\013\001\013\001\012\001\
\016\001\048\000\047\000\011\001\049\000\052\000\050\000\002\001\
\047\000\016\001\003\001\055\000\056\000\006\001\007\001\059\000\
\060\000\062\000\011\001\066\000\004\001\014\001\015\001\016\001\
\017\001\018\001\019\001\001\001\002\001\003\001\004\001\005\001\
\001\001\002\001\003\001\005\001\010\001\003\001\012\001\013\001\
\013\001\010\001\016\001\012\001\013\001\010\001\010\001\016\001\
\001\001\002\001\003\001\009\001\001\001\002\001\003\001\009\001\
\006\001\010\001\009\001\012\001\013\001\009\001\009\001\016\001\
\014\001\015\001\016\001\017\001\018\001\019\001\014\001\015\001\
\016\001\017\001\018\001\019\001\013\001\013\001\013\001\013\001\
\015\000\065\000\061\000\016\001\016\001\066\000\031\000\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\255\
\000\001\255\255\255\255\255\255\255\255\255\255\006\001\255\255\
\255\255\255\255\255\255\255\255\255\255\255\255\014\001\015\001\
\016\001\017\001\018\001\019\001"

let yynames_const = "\
  COLON\000\
  COMMA\000\
  LPAR\000\
  RPAR\000\
  EOL\000\
  EOF\000\
  DIM\000\
  LET\000\
  GOTO\000\
  IF\000\
  PRINT\000\
  INPUT\000\
  "

let yynames_block = "\
  RELOP\000\
  EQUAL\000\
  ADDOP\000\
  MULOP\000\
  POWOP\000\
  IDENT\000\
  NUMBER\000\
  STRING\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'stmts) in
    Obj.repr(
# 28 "parser.mly"
                                ( List.rev _1 )
# 233 "parser.ml"
               : Absyn.program))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'stmts) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'stmt) in
    Obj.repr(
# 30 "parser.mly"
                                ( _2::_1 )
# 241 "parser.ml"
               : 'stmts))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'stmts) in
    Obj.repr(
# 31 "parser.mly"
                                ( syntax (); _1 )
# 248 "parser.ml"
               : 'stmts))
; (fun __caml_parser_env ->
    Obj.repr(
# 32 "parser.mly"
                                ( [] )
# 254 "parser.ml"
               : 'stmts))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'label) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'action) in
    Obj.repr(
# 34 "parser.mly"
                                ( (linenr (), Some _1, Some _2) )
# 262 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'action) in
    Obj.repr(
# 35 "parser.mly"
                                ( (linenr (), None, Some _1) )
# 269 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'label) in
    Obj.repr(
# 36 "parser.mly"
                                ( (linenr (), Some _1, None) )
# 276 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    Obj.repr(
# 37 "parser.mly"
                                ( (linenr (), None, None) )
# 282 "parser.ml"
               : 'stmt))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'ident) in
    Obj.repr(
# 39 "parser.mly"
                                ( _1 )
# 289 "parser.ml"
               : 'label))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'arrayfn) in
    Obj.repr(
# 41 "parser.mly"
                                ( Dim (_2) )
# 296 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 2 : 'memref) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _4 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 42 "parser.mly"
                                ( Let (_2, _4) )
# 305 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'ident) in
    Obj.repr(
# 43 "parser.mly"
                                ( Goto (_2) )
# 312 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 2 : 'relexpr) in
    let _4 = (Parsing.peek_val __caml_parser_env 0 : 'ident) in
    Obj.repr(
# 44 "parser.mly"
                                ( If (_2, _4) )
# 320 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'prints) in
    Obj.repr(
# 45 "parser.mly"
                                ( Print (_2) )
# 327 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    Obj.repr(
# 46 "parser.mly"
                                ( Print ([]) )
# 333 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'inputs) in
    Obj.repr(
# 47 "parser.mly"
                                ( Input (_2) )
# 340 "parser.ml"
               : 'action))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'print) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'prints) in
    Obj.repr(
# 49 "parser.mly"
                                ( _1::_3 )
# 348 "parser.ml"
               : 'prints))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'print) in
    Obj.repr(
# 50 "parser.mly"
                                ( [_1] )
# 355 "parser.ml"
               : 'prints))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 52 "parser.mly"
                                ( Printexpr (_1) )
# 362 "parser.ml"
               : 'print))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 53 "parser.mly"
                                ( String (_1) )
# 369 "parser.ml"
               : 'print))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'memref) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'inputs) in
    Obj.repr(
# 55 "parser.mly"
                                ( _1::_3 )
# 377 "parser.ml"
               : 'inputs))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'memref) in
    Obj.repr(
# 56 "parser.mly"
                                ( [_1] )
# 384 "parser.ml"
               : 'inputs))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'ident) in
    Obj.repr(
# 58 "parser.mly"
                                ( Variable (_1) )
# 391 "parser.ml"
               : 'memref))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'arrayfn) in
    Obj.repr(
# 59 "parser.mly"
                                ( Arrayfn (_1) )
# 398 "parser.ml"
               : 'memref))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 61 "parser.mly"
                                ( Binop (_2, _1, _3) )
# 407 "parser.ml"
               : 'relexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'expr) in
    Obj.repr(
# 62 "parser.mly"
                                ( Binop (_2, _1, _3) )
# 416 "parser.ml"
               : 'relexpr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'expr) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'term) in
    Obj.repr(
# 64 "parser.mly"
                                ( Binop (_2, _1, _3) )
# 425 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'term) in
    Obj.repr(
# 65 "parser.mly"
                                ( _1 )
# 432 "parser.ml"
               : 'expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'term) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'factor) in
    Obj.repr(
# 67 "parser.mly"
                                ( Binop (_2, _1, _3) )
# 441 "parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'factor) in
    Obj.repr(
# 68 "parser.mly"
                                ( _1 )
# 448 "parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'primary) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'factor) in
    Obj.repr(
# 70 "parser.mly"
                                ( Binop (_2, _1, _3) )
# 457 "parser.ml"
               : 'factor))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'primary) in
    Obj.repr(
# 71 "parser.mly"
                                ( _1 )
# 464 "parser.ml"
               : 'factor))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 73 "parser.mly"
                                ( _2 )
# 471 "parser.ml"
               : 'primary))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : string) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'primary) in
    Obj.repr(
# 74 "parser.mly"
                                ( Unop (_1, _2) )
# 479 "parser.ml"
               : 'primary))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 75 "parser.mly"
                                ( Constant (float_of_string (_1)) )
# 486 "parser.ml"
               : 'primary))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'memref) in
    Obj.repr(
# 76 "parser.mly"
                                ( Memref (_1) )
# 493 "parser.ml"
               : 'primary))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'ident) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'expr) in
    Obj.repr(
# 78 "parser.mly"
                                ( (_1, _3) )
# 501 "parser.ml"
               : 'arrayfn))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 80 "parser.mly"
                                ( _1 )
# 508 "parser.ml"
               : 'ident))
; (fun __caml_parser_env ->
    Obj.repr(
# 81 "parser.mly"
                                ( "dim" )
# 514 "parser.ml"
               : 'ident))
; (fun __caml_parser_env ->
    Obj.repr(
# 82 "parser.mly"
                                ( "goto" )
# 520 "parser.ml"
               : 'ident))
; (fun __caml_parser_env ->
    Obj.repr(
# 83 "parser.mly"
                                ( "if" )
# 526 "parser.ml"
               : 'ident))
; (fun __caml_parser_env ->
    Obj.repr(
# 84 "parser.mly"
                                ( "input" )
# 532 "parser.ml"
               : 'ident))
; (fun __caml_parser_env ->
    Obj.repr(
# 85 "parser.mly"
                                ( "let" )
# 538 "parser.ml"
               : 'ident))
; (fun __caml_parser_env ->
    Obj.repr(
# 86 "parser.mly"
                                ( "print" )
# 544 "parser.ml"
               : 'ident))
(* Entry program *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
|]
let yytables =
  { Parsing.actions=yyact;
    Parsing.transl_const=yytransl_const;
    Parsing.transl_block=yytransl_block;
    Parsing.lhs=yylhs;
    Parsing.len=yylen;
    Parsing.defred=yydefred;
    Parsing.dgoto=yydgoto;
    Parsing.sindex=yysindex;
    Parsing.rindex=yyrindex;
    Parsing.gindex=yygindex;
    Parsing.tablesize=yytablesize;
    Parsing.table=yytable;
    Parsing.check=yycheck;
    Parsing.error_function=parse_error;
    Parsing.names_const=yynames_const;
    Parsing.names_block=yynames_block }
let program (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 1 lexfun lexbuf : Absyn.program)

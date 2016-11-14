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

val program :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Absyn.program

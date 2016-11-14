
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOK_VOID = 258,
     TOK_BOOL = 259,
     TOK_CHAR = 260,
     TOK_INT = 261,
     TOK_STRING = 262,
     TOK_PROTOTYPE = 263,
     TOK_IF = 264,
     TOK_ELSE = 265,
     TOK_WHILE = 266,
     TOK_RETURN = 267,
     TOK_STRUCT = 268,
     TOK_NEWSTRING = 269,
     TOK_FALSE = 270,
     TOK_TRUE = 271,
     TOK_NULL = 272,
     TOK_NEW = 273,
     TOK_ARRAY = 274,
     TOK_PARAMLIST = 275,
     TOK_EQ = 276,
     TOK_NE = 277,
     TOK_LT = 278,
     TOK_LE = 279,
     TOK_GT = 280,
     TOK_GE = 281,
     TOK_FUNCTION = 282,
     TOK_IDENT = 283,
     TOK_INTCON = 284,
     TOK_CHARCON = 285,
     TOK_STRINGCON = 286,
     TOK_INDEX = 287,
     TOK_BLOCK = 288,
     TOK_CALL = 289,
     TOK_IFELSE = 290,
     TOK_INITDECL = 291,
     TOK_RETURNVOID = 292,
     TOK_POS = 293,
     TOK_NEG = 294,
     TOK_NEWARRAY = 295,
     TOK_TYPEID = 296,
     TOK_FIELD = 297,
     TOK_DECLID = 298,
     TOK_ORD = 299,
     TOK_CHR = 300,
     TOK_ROOT = 301,
     TOK_PTR = 302,
     TOK_GOTO = 303,
     TOK_SIZEOF = 304,
     TOK_VARDECL = 305
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;



.so Tmac.mm-etc
.if t .Newcentury-fonts
.INITR* \n[.F]
.SIZE 12 14
.TITLE CMPS-112 Fall\~2016 Program\~2 "Functionally Ocaml"
.RCS "$Id: asg2-ocaml-dc.mm,v 1.12 2016-09-30 17:00:16-07 - - $"
.PWD
.URL
.EQ
delim $$
.EN
.H 1 "Overview"
In this assignment, 
you will implement a desk calculator in Ocaml, 
a language with strong static type checking.
Your program will be a strict subset of
.V= dc (1),
although it will not have all of its functions.
Begin by reading the man page for
.V= dc (1)
and experimenting with it.
Study its input format, output format, error messages, and exit status.
.P
Your program will read the single file (if specified) as does
.V= dc
and then read 
.V= stdin .
Implement the following 
.V= dc
functions\(::
.V= "+ - * / % \[ha] c d f l p s" .
Note that all of these letters are lower case.
.H 1 "Implementation Notes"
.ALX 1 ()
.LI
You
.E= "may not use"
the
.V= Num
or
.V= Big_int
modules in the Ocaml library.
Instead, you will implement your own version of bigint
by representing an integer by a product of a sign with a list
of integers.
The largest value of int in Ocaml is
.br
.ce 1
$4611686018427387903 = roman 0x3FFFFFFFFFFFFFFF = 2 sup 62 - 1$
.br
which is one bit less than what you might normally expect.
This is because one bit is used in each word for tagging.
.LI
The ideal representation therefore would be to use eight-digit numbers
in a list.
However, in order to make sure that the lists are working,
which is the point of the assignment,
you can store only one digit in each element of a list.
This is incredibly wasteful of storage,
but possible makes the representation easier.
The
.V= dc
utility actually uses character arrays with two decimal digits per byte.
.LI
Since arithmetic operations proceed from the lowest order digit to
the highest,
represent your numbers with the lowest order digit at the
front of the list and the leftmost digit at the end.
.LI
.E= "Do not"
use any loops in your program.
All iteration should be done via recursion,
and whenever possible,
by using higher-order functions like map.
.LI
First implement input and output of numbers.
Make sure your output duplicates
.V= dc
for very large numbers.
Note that an underscore prefixing a number
makes it negative.
The minus sign is strictly for subtraction.
.LI
Next, implement addition and subtraction.
To do this, you will need two functions
.V= add
and
.V= sub
which just compare signs and then call
.V= add'
or
.V= sub'
as appropriate to do the work on their absolute values.
When you subtract, make sure that the first argument is
always the larger one.
.LI
You will need a function 
.V= cmp
which returns a comparison value in the same way as does
.V= strcmp
in C.
This can move from the low order digits to the high order digits
tail recursively and stop at the end of the shorter list,
or by maintaining an actual comparison when the two lists turn out
to be the same length.
.LI
Make sure that you always canonicalize your answers by deleting
leading 0 digits.
This is only an issue with absolute subtraction,
since addition can only lengthen the number.
All other operations are implemented in terms of addition and
subtraction.
.LI
To implement multiplication, you add appropriate elements of
the right column.
To implement division, you add appropriate elements of the
left column.
The remainder is just whatever is left over after finishing the
division,
so your division function should return two results as a tuple,
namely the quotient and remainder, 
and the main module then ignores the one not wanted.
.LI
Exponentiation will then be trivial, since it is a simple matter 
to call the other functions.
.H 1 "Interactive Use"
As with some other languages,
.V= ocaml
can be used interactively for testing purposes\(::
.TVCODE* 1 "-bash-1\[Do] " "ocaml"
.TVCODE* 1 "        OCaml version 4.02.1"
.TVCODE* 1 "# " "let f x y = x + y;;"
.TVCODE* 1 "val f : int -> int -> int = <fun>"
.TVCODE* 1 "# " "f 3 4;;"
.TVCODE* 1 "- : int = 7"
.TVCODE* 1 "# " "\[ha]D"
You will, of course, need to add it to your
.V= \[Do]PATH \(::
.TVCODE* 1 "-bash-2\[Do] " "which ocaml"
.TVCODE* 1 "/afs/cats.ucsc.edu/courses/cmps112-wm/usr/ocaml/bin/ocaml"
Unfortunately, while interacting with
.V= ocaml ,
the up arrow can not be used to recover earlier lines.
Using it just causes
.V= ocaml 
to print 
.V= \[ha][[A .
To get around this, you can use 
.V= rlwrap ,
as in
.TVCODE* 1 "-bash-3\[Do] " "rlwrap ocaml"
Then you can use the arrow keys to navigate.
.TVCODE* 1 "-bash-4\[Do] " "which rlwrap"
.TVCODE* 1 "/afs/cats.ucsc.edu/courses/cmps112-wm/usr/rlwrap/bin/rlwrap"
.H 1 "What to Submit"
.V= Makefile ,
.V= bigint.ml ,
.V= bigint.mli ,
.V= maindc.ml ,
and
.V= scanner.mll .
Note that 
.V= scanner.ml
is a generated file and should be made by the 
.V= Makefile .
Also,
.V= dc.ml
is a debugging tool, not to be submitted.
Testing will be done on the 
.V= ocamlrun
script
.V= ocamldc ,
which should be runnable from the commandline.
.P
Program testing\(::
Test data will be fed to
.V= dc (1)
as well as to your program and the output will be checked with
.V= diff (1).
.FINISH

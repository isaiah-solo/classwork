(* $Id: muldivrem-trace.ml,v 1.1 2012-02-07 19:44:53-08 - - $ *)

let double number = number + number

let rec mul' (multiplier, powerof2, multiplicand') =
    if powerof2 > multiplier
    then multiplier, 0
    else let remainder, product =
             mul' (multiplier, double powerof2, double multiplicand')
         in  if remainder < powerof2
             then remainder, product
             else remainder - powerof2, product + multiplicand'

let mul (multiplier, multiplicand) =
    let _, product = mul' (multiplier, 1, multiplicand)
    in  product

let rec divrem' (dividend, powerof2, divisor') =
    if divisor' > dividend
    then 0, dividend
    else let quotient, remainder =
             divrem' (dividend, double powerof2, double divisor')
         in  if remainder < divisor'
             then quotient, remainder
             else quotient + powerof2, remainder - divisor'

let divrem (dividend, divisor') = divrem' (dividend, 1, divisor')

let div (dividend, divisor) =
    let quotient, _ = divrem (dividend, divisor)
    in quotient

let rem (dividend, divisor) =
    let _, remainder = divrem (dividend, divisor)
    in remainder

;;
#trace mul     ;;
#trace mul'    ;;
#trace div     ;;
#trace rem     ;;
#trace divrem  ;;
#trace divrem' ;;


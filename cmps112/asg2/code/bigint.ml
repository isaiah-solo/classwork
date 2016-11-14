(* $Id: bigint.ml,v 1.5 2014-11-11 15:06:24-08 - - $ *)

open Printf

module Bigint = struct

    type sign     = Pos | Neg
    type bigint   = Bigint of sign * int list
    let  radix    = 10
    let  radixlen =  1

    let length    = List.length
    let car       = List.hd
    let cdr       = List.tl
    let map       = List.map
    let reverse   = List.rev
    let strcat    = String.concat
    let strlen    = String.length
    let strsub    = String.sub
    let zero      = Bigint (Pos, [])

    let charlist_of_string str = 
        let last = strlen str - 1
        in  let rec charlist pos resultult =
            if pos < 0
            then resultult
            else charlist (pos - 1) (str.[pos] :: resultult)
        in  charlist last []

    let bigint_of_string str =
        let len = strlen str
        in  let to_intlist first =
                let substr = strsub str first (len - first) in
                let digit char = int_of_char char - int_of_char '0' in
                map digit (reverse (charlist_of_string substr))
            in  if   len = 0
                then zero
                else if   str.[0] = '_'
                     then Bigint (Neg, to_intlist 1)
                     else Bigint (Pos, to_intlist 0)

    let string_of_bigint (Bigint (sign, value)) =
        match value with
        | []    -> "0"
        | value -> let reversed = reverse value
                   in  strcat ""
                       ((if sign = Pos then "" else "-") ::
                        (map string_of_int reversed))

    let rec cmp' left right = match (left, right) with
        | [], []         -> 0
        | left, []       -> 1
        | [], right      -> -1
        | left, right    -> 
            let result = cmp' (cdr left) (cdr right)
            in if result != 0 then result else 
                if (car left) < (car right) then -1 else
                    if (car left) > (car right) then 1 else 0
    
    let cmp (Bigint (lneg, left)) (Bigint (rneg, right)) = 
        cmp' left right
    
    let rec remove_zeroes' list' = match list' with
        | []        -> []
        | [0]       -> []
        | car::cdr  ->
                let cdr' = remove_zeroes' cdr in
                    match car, cdr' with
                        | 0, [] -> []
                        | car, cdr' -> car::cdr'

    let remove_zeroes list =
        remove_zeroes' list

    let rec add' left right carry = match (left, right, carry) with
        | left, [], 0        -> left
        | [], right, 0       -> right
        | left, [], carry    -> add' left [carry] 0
        | [], right, carry   -> add' [carry] right 0
        | car1::cdr1, car2::cdr2, carry ->
          let sum = car1 + car2 + carry
          in  remove_zeroes (sum mod radix :: add' cdr1 cdr2
              (sum / radix))
    
    let rec sub' left right borrow = match (left, right, borrow) with
        | left, [], 0        -> left
        | [], right, 0       -> right
        | left, [], borrow    -> sub' left [borrow] 0
        | [], right, borrow   -> sub' [borrow] right 0
        | car1::cdr1, car2::cdr2, borrow ->
            let difference = (car1 - car2 - borrow) < 0 in
                let sum = (
                    if difference then
                        (car1 + 10 - car2 - borrow) 
                    else (car1 - car2 - borrow)
                ) in
                remove_zeroes 
            (sum :: sub' cdr1 cdr2 (if difference then 1 else 0))

    let add (Bigint (lneg, left)) (Bigint (rneg, right)) =
        let result = cmp (Bigint (lneg, left)) (Bigint (rneg, right));
        in if result = 0 then
                match (lneg, rneg) with
                    | Pos, Pos    -> Bigint (Pos, add' left right 0)
                    | Pos, Neg    -> zero
                    | Neg, Pos    -> zero
                    | Neg, Neg    -> Bigint (Neg, add' left right 0)
            else if result = 1 then
                match (lneg, rneg) with
                    | Pos, Pos    -> Bigint (Pos, add' left right 0)
                    | Pos, Neg    -> Bigint (Pos, sub' left right 0)
                    | Neg, Pos    -> Bigint (Neg, sub' left right 0)
                    | Neg, Neg    -> Bigint (Neg, add' left right 0)
            else
                match (lneg, rneg) with
                    | Pos, Pos    -> Bigint (Pos, add' right left 0)
                    | Pos, Neg    -> Bigint (Neg, sub' right left 0)
                    | Neg, Pos    -> Bigint (Pos, sub' right left 0)
                    | Neg, Neg    -> Bigint (Neg, add' right left 0)
    
    let sub (Bigint (lneg, left)) (Bigint (rneg, right)) =
        let result = cmp (Bigint (lneg, left)) (Bigint (rneg, right));
        in if result = 0 then
                match (lneg, rneg) with
                    | Pos, Pos    -> zero
                    | Pos, Neg    -> Bigint (Pos, add' left right 0)
                    | Neg, Pos    -> Bigint (Neg, add' left right 0)
                    | Neg, Neg    -> zero
            else if result = 1 then
                match (lneg, rneg) with
                    | Pos, Pos    -> Bigint (Pos, sub' left right 0)
                    | Pos, Neg    -> Bigint (Pos, add' left right 0)
                    | Neg, Pos    -> Bigint (Neg, add' left right 0)
                    | Neg, Neg    -> Bigint (Neg, sub' left right 0)
            else
                match (lneg, rneg) with
                    | Pos, Pos    -> Bigint (Neg, sub' right left 0)
                    | Pos, Neg    -> Bigint (Pos, add' right left 0)
                    | Neg, Pos    -> Bigint (Neg, add' right left 0)
                    | Neg, Neg    -> Bigint (Pos, sub' right left 0)

    let double number =
        add' number number 0
    
    let rec mul' (multiplier, powerof2, multiplicand') =
        if cmp' powerof2 multiplier = 1 then
            multiplier, []
        else
            let remainder, product =
                    mul' (multiplier,  double powerof2, 
                    double multiplicand') in
                if cmp' remainder powerof2 = -1 then
                    remainder, product
                else
                    (sub' remainder powerof2 0), 
                    (add' product multiplicand' 0)
                    
    let mul (Bigint (lneg, multiplier)) (Bigint (rneg, multiplicand)) =
        let neg = match (lneg, rneg) with
            | lneg, rneg when lneg = rneg -> Pos
            | lneg, rneg -> Neg
        in let _, product = mul' (multiplier, (1 :: []), multiplicand)
        in Bigint(neg, product)
        
    let rec divrem' (dividend, powerof2, divisor') =
        if cmp' divisor' dividend = 1
        then [], dividend
        else let quotient, remainder =
                 divrem' (dividend, double powerof2, double divisor')
             in  if cmp' remainder divisor' = -1
                 then quotient, remainder
                 else (add' quotient powerof2 0), 
                    (sub' remainder divisor' 0)
                 
    let divrem (dividend, divisor') = 
        divrem' (dividend, (1 :: []), divisor')
        
    let div (Bigint (lneg, dividend)) (Bigint (rneg, divisor)) =
        let neg = match (lneg, rneg) with
            | lneg, rneg when lneg = rneg -> Pos
            | lneg, rneg -> Neg
        in let quotient, _ = divrem (dividend, divisor)
        in Bigint(neg, quotient)
        
    let rem (Bigint (lneg, dividend)) (Bigint (rneg, divisor)) =
        let neg = match (lneg, rneg) with
            | lneg, rneg when lneg = rneg -> Pos
            | lneg, rneg -> Neg
        in let _, remainder = divrem (dividend, divisor)
        in Bigint(neg, remainder)

    let even (x: int list) = match x with
        | [] -> false
        | x -> car x mod 2 = 0

    let odd (x: int list) = match x with
        | [] -> true
        | x -> car x mod 2 = 1

    let square var = 
        let _, result = mul' (var, (1 :: []), var)
        in result
       
    let rec pow' (base: int list) (exponent: int list) 
            (resultult: int list) = match exponent with
        | [] -> resultult
        | exponent  when odd exponent -> 
                let value1 = sub' exponent (1 :: []) 0 in
                    let _, value2 = mul' (resultult, (1 :: []), base) in
                        pow' base value1 value2
        | exponent -> let value1 = square base in
            let value2, _ = divrem' (exponent, (1 :: []), (2 :: [])) in
                pow' value1 value2 resultult
            
    let pow  (Bigint (lneg, base)) (Bigint (rneg, exponent)) =
        let neg = if even exponent then Pos else lneg
        in if rneg = Pos then 
            let result = pow' base exponent (1 :: [])
            in Bigint (neg, result)
        else
            Bigint (neg, [])

end

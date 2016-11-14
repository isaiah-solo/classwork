#!/usr/bin/perl
# $Id: egypt-mul.perl,v 1.1 2012-02-07 19:44:53-08 - - $

$0 =~ s|.*/||;
print STDERR "Usage: $0 multiplier multiplicand\n" and exit
unless @ARGV == 2 and ($left, $right) = @ARGV
       and $left =~ m/^\d+$/ and $right =~ m/^\d+$/;

print "$0: verification: $left * $right = ", $left * $right, "\n\n";

$top = 1;
while ($top <= $left) {
   push @stack, [$top, $right];
   $top += $top;
   $right += $right;
}

$result = 0;
$fmt = "%12s %12s   %12s %12s\n";
while (@stack) {
   printf $fmt, "", "", $left, $result;
   ($top, $right) = @{pop @stack};
   if ($top <= $left) {
      $left -= $top;
      $result += $right;
      printf $fmt, $top, $right, "- " . $top, "+ " . $right;
   }else {
      printf $fmt, $top, $right, "- 0", "+ 0";
   }
   printf $fmt, "", "", "= " . $left, "= " . $result;
   printf "\n";
}

printf $fmt, "", "", "zero", "product";


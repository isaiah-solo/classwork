function [f, G] = simple_quadratic(x)

  x1 = x(1, 1);
  x2 = x(2, 1);
  
  f = (3 * x1 - 9)^2 + (x2 - 4)^2; % 9x1^2 - 54x1 + x2^2 - 8x2 + 97
  G = [((18 * x1) - 54) ; ((2 * x2) - 8)];
  
end
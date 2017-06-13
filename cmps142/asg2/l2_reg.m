function [f, G] = l2_reg(w)

  f = w.' .* w;
  G = 2 * w;
  
end
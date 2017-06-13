function [f , G] = obj (X, w, y, b)
  [f1, G1] = cross_entropy(X, w, y);
  [f2, G2] = l2_reg(w);
  f = f1 + b * f2;
  G = G1 + b * G2;
end
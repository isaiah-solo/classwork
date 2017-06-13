function [pass, maxErr] = grad_check(fun, sizeVec, rep, tol)
  if nargin < 4
    tol = 1E-6;
  end
  
  maxErr = 0;
  for i = 1 : rep
    w0 = randn(sizeVec(1), sizeVec(2));
    [g1] = gradest(fun, w0);
    [f, g2] = fun(w0);
    maxErr = max(maxErr, abs(g1(:) - g2(:)));
  end
  
  if maxErr < tol
    pass = 1; 
  else
    pass = 0;
  end
end
function [f, G] = cross_entropy(X, w, y)

  t = size(X, 1);
  sum = 0;
  yhatsum = 0;
  for i = 1 : t - 1
    yhat = sigmoid(X(i, :) * w);
    L = -y(i, 1) * log(yhat) - (1 - y(i, 1)) * log(1 - yhat);
    sum = sum + L;
    yhatsum = yhatsum + yhat;
  end
  f = sum;
  
  G = X.' * (sigmoid(X.' * w) - y);
  
end
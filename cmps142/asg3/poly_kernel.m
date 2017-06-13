function [K] = poly_kernel(X1, X2, d)

    K = (1 + linear_kernel(X1, X2)) .^ (d);
    
end
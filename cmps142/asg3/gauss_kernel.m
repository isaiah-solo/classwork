function [K] = gauss_kernel(X1, X2, sigma)

    X12 = (X1 - X2);
    K = exp(-((X12.' * X12) / ((2 * sigma)^(2))));
    
end
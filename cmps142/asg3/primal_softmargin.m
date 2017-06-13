function [w, b] = primal_softmargin(X, y, beta)
    
    n = size(X, 1);

    H = eye(n + 1);
    H(n + 1, n + 1) = 0;
    f = zeros(n + 1, 1);
    
    z = [X, ones(n, 1)];
    disp(y);
    disp(X);
    A = -diag(y)*X;
    %A = [(-(diag(y)) * X), (-(diag(y))), -1;
    %    0, 0, -1];
    be = [-1; 0];
    z = quadprog(H, f, A, be);
    
    w = z(1);
    b = z(2);
    
end
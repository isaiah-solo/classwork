function [lambda, b] = dual_softmargin(K, y, beta)
    
    

    Ag = [-1; 1];
    bg = [-1; 0];
    Ae = [y];
    be = [0];

    lambda = quadprog(H, f, Ag, bg, Ae, be);

end
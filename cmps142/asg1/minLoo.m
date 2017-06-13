function w = minLoo(X, y)

    t = size(y, 1);

    A = [X, -ones(t, 1); -X, -ones(t, 1)];
    b = [y; -y];
    f = [0; 0; 1];

    z = linprog(f, A, b);

    w = z(1:end-1);

end
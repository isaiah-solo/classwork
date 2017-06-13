function w = minL1(X, y)

    t = size(y, 1);

    A = [X, -eye(t); -X, -eye(t)];
    b = [y; -y];
    f = [0; 0; ones(t, 1);];

    z = linprog(f, A, b);

    w = z(1:2);

end
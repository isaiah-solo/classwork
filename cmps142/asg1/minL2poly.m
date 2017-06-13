function c = minL2poly(x, y, d)
    X = [];
    for i=1:d+1
        v = [];
        for j=1:size(x, 1)
            v = [v; x(j)^(d + 1 - i)];
        end
        X = [X, v];
    end
    c = (X' * X) \ (X' * y);
end
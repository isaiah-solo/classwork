function w = minL2(X, y)

    w = (X' * X) \ (X' * y);

end
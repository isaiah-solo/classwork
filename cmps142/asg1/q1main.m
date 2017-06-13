n = 2;
t = 10;
te = 1000;
u = [0; ones(n-1,1)];
sigma = 0.1;
X = [ones(t,1) rand(t, n-1)];
Xt = [ones(te,1) rand(te, n-1)];

y1 = X * u + randn(t, 1) * sigma;
y2 = X * u + randn(t, 1) ./ randn(t, 1) * sigma;
y3 = X * u + randn(t, 1) .* randn(t, 1) * sigma;

yt1 = Xt * u + randn(te, 1) * sigma;
yt2 = Xt * u + randn(te, 1) ./ randn(te, 1) * sigma;
yt3 = Xt * u + randn(te, 1) .* randn(te, 1) * sigma;

maty1 = createM(X, y1);
maty2 = createM(X, y2);
maty3 = createM(X, y3);

matyt1 = createM(Xt, yt1);
matyt2 = createM(Xt, yt2);
matyt3 = createM(Xt, yt3);

clf;
plot((X(:,2)'), y', 'k*');
hold;
plot([0 1], [w2(1) sum(w2)], 'r-');
plot([0 1], [w1(1) sum(w1)], 'g-');
plot([0 1], [woo(1) sum(woo)], 'b-');
print -deps experiment.1.1.<m>.ps;

function matrix = createM(X, y)

    w1 = minL1(X, y);
    w2 = minL2(X, y);
    woo = minLoo(X, y);
    matrix = [cre((X * w1), y);
        cre((X * w2), y);
        cre((X * woo), y)];

end

function matrix = cre(w, h)

    errl1 = errL1(w, h);
    errl2 = errL2(w, h);
    errloo = errLoo(w, h);
    
    matrix = [errl1, errl2, errloo];

end

function err = errL1(w, y)

    err = sum(abs(w - y));

end

function err = errL2(w, y)

    err = sum((w - y) .^ 2);

end

function err = errLoo(w, y)

    err = max(abs(w - y));

end
t = 10;
te = 1000;
sigma = 0.1;
x = rand(t, 1);
xt = rand(te, 1);

y1 = double(x > 0.5);
y2 = 0.5 - 10.4 *x.*(x-0.5).*(x - 1) + sigma * randn(t, 1);

yt1 = double(xt > 0.5);
yt2 = 0.5 - 10.4 *xt.*(xt-0.5).*(xt - 1) + sigma * randn(te, 1);

clf;
axis([0 1 -0.5 1.5]);
hold;
plot(xt', yt2', 'k*');
xx = (0:1000)/1000;
yy = 0.5 - 10.4 * xx .* (xx - 0.5) .* (xx - 1);

c1 = minL2poly(xt, yt2, 1);
c3 = minL2poly(xt, yt2, 3);
c5 = minL2poly(xt, yt2, 5);
c9 = minL2poly(xt, yt2, 9);

err1 = errL2Poly(xt, yt2, 1, c1);
err3 = errL2Poly(xt, yt2, 3, c3);
err5 = errL2Poly(xt, yt2, 5, c5);
err9 = errL2Poly(xt, yt2, 9, c9);

plot(xx, yy, 'k:');
plot(xx, polyval(c1, xx), 'r-');
plot(xx, polyval(c3, xx), 'g-');
plot(xx, polyval(c5, xx), 'b-');
plot(xx, polyval(c9, xx), 'm-');
print -deps experiment.1.2.<m>.ps;

function err = errL2Poly(x, y, d, c)
    X = getMatrix(x, y, d);
    err = ((X * c) - y)' * ((X * c) - y);
    err = err / size(y, 1);
end

function mat = getMatrix(x, y, d)
    X = [];
    for i=1:d+1
        v = [];
        for j=1:size(x, 1)
            v = [v; x(j)^(d + 1 - i)];
        end
        X = [X, v];
    end
    mat = X;
end
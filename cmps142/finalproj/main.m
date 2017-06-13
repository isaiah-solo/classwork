% SMOOTHS
X = sort([x(:, 3) pred']);
new_x = X(:, 1);
new_pred = X(:, 2);

% PLOTS
plot(new_x, new_pred, 'r-');
hold on;
plot(x(:, 3), y', 'k*');

% ERRORS
knn = [];
lnr = [];
lgr = [];
svm = [];

for i = 1:length(iters)
    knn = [knn, errors(i, 1)];
    lnr = [lnr, errors(i, 2)];
    lgr = [lgr, errors(i, 3)];
    svm = [svm, errors(i, 4)];
end

figure
plot(iters, knn, 'r-');
hold on;
plot(iters, lnr, 'g-');
plot(iters, lgr, 'b-');
plot(iters, svm, 'm-');
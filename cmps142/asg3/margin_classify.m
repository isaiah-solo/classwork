function [yhat] = margin_classify(Xtest, w, b)
    yhat = sign((Xtest.' * w + b));
end
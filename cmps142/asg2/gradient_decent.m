function [xvals, fvals] = gradient_decent(func, x0, options)

  xvals = [];
  fvals = [];
  xprev = x0;
  
  for i = 1 : options.NumIterations
    [f, G] = func(xprev);
    xvals = [xvals ; xprev.'];
    fvals = [fvals ; f];

    xnext = xprev - (options.StepSize * G);
    xprev = xnext;
  end
  
end
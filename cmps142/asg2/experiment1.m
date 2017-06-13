function experiment1(num_iterations, step_size)
  x0 = [0;0];
  options = struct('NumIterations', num_iterations, 'StepSize', step_size);
  [xvals, fvals] = gradient_decent(@simple_quadratic, x0, options);

  clf
  plot(fvals , 'b-');
  title('Change in the value of function over iterations.')
  xlabel('Iteration Num')
  ylabel('simple\_quadratic(x)')
  figure

  clf
  plot(xvals(:, 1), xvals(:, 2), '--gs' , 'MarkerSize' , 5 , 'MarkerFaceColor' , 'r')
  title('Trajectory of x over iterations, The first 10 points are numbered.')
  xlabel('x_1')
  ylabel('x_2')
  point_labels = num2str((0 : 10)', '%d');
  offset = (max(xvals(1 : 11, 1)) - min(xvals(1 : 11, 1))) / 40;
  text(xvals(1 : 11, 1) + offset, xvals(1 : 11, 2), point_labels)
end
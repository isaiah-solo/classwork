import tensorflow as tf

###############################################################################
# Create the tensor x of the value 
#   [[[2, -2, 4], [0, 1, 2]], [[1, 2, 3], [7, 1, 9]]] 
# and y as a tensor of ones with the same shape as x.
# Return a Boolean tensor that yields Trues with the same shape as x and y if 
# x equals y element-wise.
# Hint: Look up tf.equal().
###############################################################################
def p1():
	x = tf.constant([[[2, -2, 4], [0, 1, 2]], [[1, 2, 3], [7, 1, 9]]])
	y = tf.constant([[[1, 1, 1], [1, 1, 1]], [[1, 1, 1], [1, 1, 1]]])
	return tf.equal(x, y)

################################################################################
# Creates one variable 'x' of the value [3., -4.] and a placeholder 'y' of the 
# same shape as 'x'. Given a scalar z returns a triple containing 
#   x
#   y 
#   and a TensorFlow object that returns x + y if z > 0, and x - y otherwise. 
# Hint: Look up tf.cond().
################################################################################
def p2(z):
	x = tf.Variable([3., -4.])
	y = tf.placeholder(x.dtype)
	return x, y, tf.cond(tf.less(0, z), lambda: x + y, lambda: x - y)

###############################################################################
# Given 2d tensors x, y, and z, returns a tensor object for  x' * y^-1 + z 
# where x' is the transpose of x and y^-1 is the inverse of y. The dimensions 
# of tensors will be compatible.
# Hint: See "Matrix Math Functions" in TensorFlow documentation.
###############################################################################
def p3(x, y, z):
	xt = tf.transpose(x)
	yi = tf.matrix_inverse(y)
	return xt * yi + z

###############################################################################
# Given a TensorFlow object that describes a convex function and a TensorFlow 
# session, return the the minimum value of the function found by gradient 
# decent. Use 0.01 learning rate and 10000 steps. 
###############################################################################
def p4(objective_function, sess):
	optimizer = tf.train.GradientDescentOptimizer(0.01)
	train_op = optimizer.minimize(objective_function)
	for i in range(10000):
		sess.run(train_op)
	return sess.run(objective_function)


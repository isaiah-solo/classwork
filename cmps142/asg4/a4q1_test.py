import tensorflow as tf
import a4q1 as q1

def print_name(f):
  def helper(x):
    print('In function %s:' % f.__name__)
    print('-' * 40)
    result = f(x)
    print('')
    return result
  return helper


@print_name
def p1_test(sess):
  print(sess.run(q1.p1()))

@print_name
def p2_test(sess):
  x1, y1, op1 = q1.p2(9)
  x2, y2, op2 = q1.p2(-1)

  sess.run(tf.global_variables_initializer())
  print(sess.run(op1, {y1: [[1, 2]]}))
  print(sess.run(op2, {y2: [[2, 9]]}))
  sess.run(x2.assign([2, 9]))
  print(sess.run(op2, {y2: [[2, 9]]}))


@print_name
def p3_test(sess):
  x = tf.constant([[1., 2.], [3., 4.]])
  y = tf.constant([[5., 0.], [0., 5.]])
  z = tf.ones([2, 2], tf.float32)
  print(sess.run(q1.p3(x, y, z)))


@print_name
def p4_test(sess):
  x1 = tf.Variable([0.], tf.float32)
  x2 = tf.Variable([0.], tf.float32)
  f = ((tf.constant(3.) * x1 - tf.constant(9.)) ** 2 + 
       (x2 - tf.constant(4.)) ** 2 + tf.constant(1.))

  sess.run(tf.global_variables_initializer())
  print(q1.p4(f, sess))
  print(sess.run([x1, x2]))

sess = tf.Session()
p1_test(sess)
p2_test(sess)
p3_test(sess)
p4_test(sess)

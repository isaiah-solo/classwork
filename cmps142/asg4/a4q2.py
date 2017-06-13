import tensorflow as tf
import numpy as np

tf.logging.set_verbosity(tf.logging.ERROR)

def get_data(filename, permissions):
  x1 = []
  x2 = []
  y = []
  training = open(filename, permissions)
  lines = training.readlines()
  for line in lines:
    values = line.split()
    x1.append([float(values[0])])
    x2.append([float(values[1])])
    y.append([float(values[2])])
  return x1, x2, y

def regress():
  features = [tf.contrib.layers.real_valued_column("x1", dimension = 1),
              tf.contrib.layers.real_valued_column("x2", dimension = 1)]

  model = tf.contrib.learn.LinearRegressor(
    feature_columns = features,
    optimizer=tf.train.FtrlOptimizer(learning_rate=5.0))

  x1_train, x2_train, y_train = get_data("a4q2_train.txt", "r")
  x1_test, x2_test, y_test = get_data("a4q2_test.txt", "r")

  train_fn = tf.contrib.learn.io.numpy_input_fn(
    {"x1": np.array(x1_train), "x2": np.array(x2_train)},
    np.array(y_train),
    batch_size = 4,
    num_epochs = 1000)

  test_fn = tf.contrib.learn.io.numpy_input_fn(
    {"x1": np.array(x1_test), "x2": np.array(x2_test)},
    np.array(y_test),
    batch_size = 4,
    num_epochs = 1000)

  model.fit(input_fn=train_fn, steps=1000)

  result = model.evaluate(input_fn=test_fn)

  print " "
  print "loss: " + str(result["loss"])
  print "x1: " + str(model.get_variable_value('linear/x1/weight'))
  print "x2: " + str(model.get_variable_value('linear/x2/weight'))
  print "bias: " + str(model.get_variable_value('linear/bias_weight'))
  print " "

regress()


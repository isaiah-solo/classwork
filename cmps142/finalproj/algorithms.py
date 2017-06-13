import matplotlib
matplotlib.use('Agg')

import scipy.io as sio
import matplotlib.pyplot as plt
import pylab
import numpy as np
import sys
import csv

from matplotlib.colors import ListedColormap
from sklearn import svm, neighbors, datasets, linear_model
from sklearn.metrics import accuracy_score
from sklearn.model_selection import validation_curve
from sklearn.linear_model import Ridge

np.set_printoptions(threshold=np.inf)

# TSV INDICES
ESSAY_ID = 0
ESSAY_SET = 1
ESSAY_TEXT = 2
RATER_1 = 3
RATER_2 = 4
TOTAL = 5

# DATA FIELDS
TARGETS = TOTAL

# DATA
def get_essays(data_set):
  essays = list()
  for element in data_set:
    essays.append(element[ESSAY_TEXT])
  return essays

def get_targets(data_set):
  targets = list()
  for element in data_set:
    targets.append(element[TARGETS])
  return targets

# FEATURES
def average_word_length(essay):
  words = essay.split()

  average_length = 0
  for word in words:
    average_length += len(word)
  average_length /= len(words)

  return average_length

def average_sentence_length(essay):
  sentences = essay.split('.')

  average_length = 0
  for sentence in sentences:
    words = sentence.split()
    average_length += len(words)
  average_length /= len(sentences)

  return average_length

def essay_length(essay):
  words = essay.split()

  return len(words)

def diversity_count(essay):
  return essay.count('@')

def define_features(data_set):
  essays = get_essays(data_set)

  inputs = list()
  for essay in essays:
    features = list()
    features.append(average_word_length(essay))
    features.append(average_sentence_length(essay))
    features.append(essay_length(essay))
    features.append(diversity_count(essay))
    inputs.append(features)

  return inputs

def get_essay_features(essay):
  inputs = list()
  features = list()
  features.append(average_word_length(essay))
  features.append(average_sentence_length(essay))
  features.append(essay_length(essay))
  features.append(diversity_count(essay))
  inputs.append(features)

  return inputs

# ALGORITHMS
def nearest_neighbors(xtrain, ytrain, xtest, ytest, save=False):
  n_neighbors = 15
  clf = neighbors.KNeighborsClassifier(n_neighbors, weights='uniform')
  clf.fit(xtrain, ytrain)

  print "KNN: " + str(clf.score(xtest, ytest))

  if save == False:
    return clf.score(xtest, ytest)

  sio.savemat('pred.mat', {'pred':clf.predict(xtest)})
  sio.savemat('y.mat', {'y':ytest})

def linear_regression(xtrain, ytrain, xtest, ytest, save=False):
  clf = linear_model.LinearRegression()
  clf.fit(xtrain, ytrain)

  print "Linear Regression: " + str(clf.score(xtest, ytest))

  if save == False:
    return clf.score(xtest, ytest)

  sio.savemat('x.mat', {'x':xtest})
  sio.savemat('pred.mat', {'pred':clf.predict(xtest)})
  sio.savemat('y.mat', {'y':ytest})

def logistic_regression(xtrain, ytrain, xtest, ytest, save=False):
  clf = linear_model.LogisticRegression()
  clf.fit(xtrain, ytrain)

  print "Logistic Regression: " + str(clf.score(xtest, ytest))

  if save == False:
    return clf.score(xtest, ytest)

  sio.savemat('x.mat', {'x':xtest})
  sio.savemat('pred.mat', {'pred':clf.predict(xtest)})
  sio.savemat('y.mat', {'y':ytest})

def sv_machine(xtrain, ytrain, xtest, ytest, save=False):
  clf = svm.SVR(gamma=1e-5)
  clf.fit(xtrain, ytrain)

  print "SVM: " + str(clf.score(xtest, ytest))

  if save == False:
    return clf.score(xtest, ytest)

  sio.savemat('pred.mat', {'pred':clf.predict(xtest)})
  sio.savemat('y.mat', {'y':ytest})

def do_algorithm(training_set, testing_set):
  xtrain = np.array(define_features(training_set)).astype(np.float)
  ytrain = np.array(get_targets(training_set)).astype(np.float)

  xtest = np.array(define_features(testing_set)).astype(np.float)
  ytest = np.array(get_targets(testing_set)).astype(np.float)

  rates = list()
  iterations = list()

  for i in range(200, 1000, 200):
    print str(i) + " -----------"
    nn = nearest_neighbors(xtrain[0:i], ytrain[0:i], xtest, ytest)
    lnr = linear_regression(xtrain[0:i], ytrain[0:i], xtest, ytest, save=True)
    lgr = logistic_regression(xtrain[0:i], ytrain[0:i], xtest, ytest)
    sv = sv_machine(xtrain[0:i], ytrain[0:i], xtest, ytest)

    rates.append([nn, lnr, lgr, sv])
    iterations.append(i)

  errors = np.array(rates).astype(np.float)
  iters = np.array(iterations).astype(np.float)

  sio.savemat('classifier_errors.mat', {'errors':errors})
  sio.savemat('classifier_iterations.mat', {'iters':iters})

  print "Files Saved!"

def predict(essay, training_set):
  xtrain = np.array(define_features(training_set)).astype(np.int)
  ytrain = np.array(get_targets(training_set)).astype(np.int)

  features = get_essay_features(essay)

  clf = svm.SVR(gamma=1e-5)
  clf.fit(xtrain, ytrain)

  grade = clf.predict(features)
  return grade[0]

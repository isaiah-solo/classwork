import matplotlib
matplotlib.use('Agg')

import matplotlib.pyplot as plt
import numpy as np
import sys
import csv

from matplotlib.colors import ListedColormap
from sklearn import neighbors, datasets, linear_model

import algorithms as alg

# DATA FIELDS
TRAINING = 1000
TESTING = 1500
TOTAL_DATA = 1783

TO_REMOVE = range(27, 6, -1) + [5]

def parse_data(filename):
  with open(filename) as tsv:
    reader = csv.reader(tsv, delimiter='\t')
    parsed_list = list(reader)
    parsed_list.pop(0)

    for element in parsed_list:
      for i in TO_REMOVE:
        element.pop(i)

    del parsed_list[TESTING:]

    return parsed_list[0:TRAINING], parsed_list[TRAINING:TESTING]

def main():
  training_set, testing_set = parse_data('data_set.tsv')
  alg.do_algorithm(training_set, testing_set)

if __name__ == "__main__": main()

import numpy as np
import os.path
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
  if len(sys.argv) != 2:
    print "grader.py: Usage: grader.py <essay text file>"
    sys.exit()

  filename = sys.argv[1]

  if ".txt" not in filename or not os.path.isfile(filename):
    print "grader.py: Invalid file"
    sys.exit()

  essay = ""
  with open(filename) as openfile:
    essay = openfile.read()

  training_set, _ = parse_data('data_set.tsv')
  grade = alg.predict(essay, training_set)

  print "Your grade is " + "%.2f" % (grade) + " out of 12."

if __name__ == "__main__": main()

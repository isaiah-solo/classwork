'''

This file demonstrates at a high-level what your program
should do. It does not support the debug option.

Note that the data structures used in this version are
significantly different than the data structures you will
use in your version. This version uses a dictionary of arrays.
You will use a binary tree of queues.

'''

import argparse

parser = argparse.ArgumentParser(description='''
    Generate word index that lists lines numbers for
    where words appear in a file.
''')
parser.add_argument('-d', action='store_true', help='Show binary tree for debugging')
parser.add_argument('file')
args = parser.parse_args()

if args.d:
    raise Exception('Not supported')

n = 1
index = {}
for line in open(args.file):
    words = line.split()
    for word in words:
        if word not in index:
            index[word] = []
        index[word].append(n)
    n += 1

for word in sorted([w for w in index]):
    linenums = index[word]
    linenums_str = ' '.join([str(s) for s in linenums])
    print('{} : {}'.format(word, linenums_str))

#!/usr/bin/python3

import sys
import re
import argparse

if sys.version_info[0] < 3:
    raise Exception("Must be using Python 3")

parser = argparse.ArgumentParser(description='Generate C header file from binary data')
parser.add_argument('filename', metavar='filename',  nargs=1, help='The binary file to read')
parser.add_argument('--skip', metavar='n', type=int, nargs=1, help='Number of bytes to skip at the beginning of file')
parser.add_argument('--length', metavar='n', type=int, nargs=1, help='The maximum number of bytes to store')
args = parser.parse_args()

filename=args.filename[0]
outfilename=filename+".h"

skip=0
length=-1
if args.skip:
    skip=args.skip[0]
if args.length:
    length=args.length[0]

indent='  '
with open(outfilename, "w") as outf:
    variable=filename
    variable=re.sub('[\. ]', '_', variable);
    header=variable.upper()+'_H_'
    variable=variable+'_data'

    outf.write('#ifndef '+header+'\n')
    outf.write('#define '+header+'\n\n')
    outf.write('unsigned char '+variable+'[] = {\n\n'+indent);
    i=0
    with open(filename, "rb") as inf:
        byte = inf.read(1)    
        while byte:
            if (skip == 0):
                if (i % 256 == 0):
                    outf.write('/* 0x'+'{:04x}'.format(i)+' */\n'+indent)
                i=i+1
                outf.write('0x'+byte.hex()+', ')
                if (i % 16 == 0):
                    outf.write('\n'+indent)
                if length > 0:
                    length=length-1
                if length == 0:
                    break
            else:
                skip=skip-1
            byte = inf.read(1)
    outf.write('\n};\n\n')
    outf.write('#endif /* '+header+' */\n')

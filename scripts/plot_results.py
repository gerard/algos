#!/usr/bin/env python

import subprocess
import tempfile
import glob
import re
import os

for datafile in glob.glob('*.data'):
    pngfile = re.match("(.*)\.data", datafile).group(1) + ".png"
    (f, tmpname) = tempfile.mkstemp()
    os.write(f, "set term png\n")
    os.write(f, "set output 'plots/%s'\n" % pngfile)
    os.write(f, "plot '%s'\n" % datafile)
    os.close(f)
    subprocess.call(["gnuplot", tmpname])
    os.unlink(tmpname)

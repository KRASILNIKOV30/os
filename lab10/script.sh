#!/usr/bin/env python
from ctypes import *

lib = cdll.LoadLibrary("libequationsolver.so")

root1 = c_double(3)
root2 = c_double(0)
hasSolution = c_bool

lib.GetQuadraticEquationRoots.argtypes = [c_double, c_double, c_double, POINTER(c_double), POINTER(c_double)]
hasSolution = lib.GetQuadraticEquationRoots(0, 2, 4, root1, root2)

if hasSolution:
  print(root1, root2)
else:
  print("no solutions")



#!/usr/bin/env python3
from pathlib import Path
from globuild import DependencyGraph

dg = DependencyGraph(Path())
dg.add_static_library("libcdatastructures.a", "vector.o", "llist.o")
dg.add_shared_library("libcdatastructures.so", "vector.o", "llist.o")
dg.add_executable("vectortest", "vector.o", "vectortest.c")
dg.add_executable("llisttest", "llist.o", "llisttest.c")
dg.build()

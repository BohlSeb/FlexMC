from pathlib import Path
import os
from distutils.core import setup, Extension

SRC_FILE = Path(os.path.dirname(__file__)).parents[1] / 'flexmc'

print(SRC_FILE)


example_module = Extension('_flexmc',
                           sources=['build/flexmcPYTHON_wrap.cxx', 'expression_calculator.cpp'],
                           )


setup(name='example',
      version='0.1',
      author="SWIG Docs",
      description="""Simple swig example from docs""",
      ext_modules=[example_module],
      py_modules=["example"])
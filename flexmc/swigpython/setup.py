from distutils.core import setup, Extension


example_module = Extension('_flexmc',
                           sources=['flexmc_wrap.cxx', '../flexmc/lexer.cpp'],
                           )

setup(name='example',
      version='0.1',
      author="SWIG Docs",
      description="""Simple swig example from docs""",
      ext_modules=[example_module],
      py_modules=["example"])
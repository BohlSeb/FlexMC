%module flexmc

%{
#include "expression_calculator.h"
%}

/* Let's just grab the original header file here */
%include <std_string.i>
#include "expression_calculator.h"
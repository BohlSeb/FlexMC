%module flexmc

%{
#include "../flexmc/lexer.h"
%}

/* Let's just grab the original header file here */
%include <std_string.i>
%include "../flexmc/lexer.h"
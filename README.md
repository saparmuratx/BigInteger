# BigInteger

This is a C++ implementation of BigInteger

Compiling and Running:
---------------------
    to compile:
    $ make
    to run:
    $ ./main

Constructors:
-------------
    default - BigInteger a; // a = 0 by default

    int - BigInteger a(123), b = -123;

    string - BigInteger a("999999999999999999999"); 

Exception Throwing - Runtime Errors:
-------------------
    BigInteger a("foobar"); // throws exception : Invalid value: foobar

    BigInteger b("123123s"); // Invalid value: 123123s
    
Increment and Decrement:
------------------------
    BigInteger a = 123;
    a--;
    --a;
    a++;
    ++a;

Comparison Operators:
---------------------    
    >
    <
    ==
    !=
    <=
    >=

Unary +/-:
----------
    BigInteger a = 123;
    a = -a; // a = -123

Arithmetic Operators
--------------------
    + // addition
    - // subtraction
    / // division
    * // multiplication
    % // module

IO Operators:
-------------
    >> // input
    << // output

Other Operators:
----------------
    += // addition assigment
    -= // subtraction assignment

Functions:
----------
    abs() // absolute value
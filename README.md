cppqsolve
=========
----------------------------------------------------------------------------------------
BUILD
-----------------------------------------------------------------------------------------
Building uses cmake.  If you don't have a copy, get it from http://www.cmake.org/
You can check what version (if any) is currently installed with "cmake --version".

Dependencies:
	+CMake
	+Boost.Python
	+Boost.DateTime
	+Boost.ProgramOptions

(It appeared that there was a bug in Boost.Python version 1.53.0, so you might
want to get a different version)

To build (from the root directory):
"mkdir build"
"cd build && cmake .."
(Assuming cmake is able to finish correctly....)
"make"

If you have cmake errors then you probably don't have the boost libraries installed.

------------------------------------------------------------------------------------------
USAGE
------------------------------------------------------------------------------------------
To see the available options: "./qsolve --help"

Note: Anything between square brackets "[]" will name a "type" of input, for example, a file,
a date, an amount of capital, etc.  So if an argument says "--output [FILE]" it means
replace "[FILE]" with the name of file.

The minimum arguments you need to give are a start date and a python script to run:
"./qsolve -start [DATE] [FILE]"

e.g. "./qsolve --start 2014/01/02 PyStrategy.py"

If no end date is given, then it will run from start date until the current date.

To give an end date, use the "--end [DATE]" (alternatively: "-e") option and give a date
in the same format as the start date.

By default, it prints the csv to standard output.  To re-direct the output use the
"--output [FILE]" (alternatively: "-o") 

The Python Script must define the function "strategy(context)".  The context object that
is passed in contains useful information about the state of the system such as the current
date, the current state of the portfolio, the stock data, etc.  Just like any other type
in python you can add member variables to context and they will be saved for the next time
around.

To order some stocks, use "context.order( [SYMBOL], [AMOUNT] )", e.g. "context.order('AAPL', 50)"
to order 50 shares to AAPL.

A really simple example script is:

# PyStrategy.py

# you can provide an initialize function, but you don't have to
# init(context) will be called once before the loop that
# calls strategy for each time period.
def init(context):
    # initialize context here
    # add some state to context that will be passed into my strategy function
    context.aapl = 'AAPL'

def strategy(context):
    context.order(context.aapl, 50);

#end PyStrategy.py

    
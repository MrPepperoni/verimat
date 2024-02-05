Building
--------
cmake -S . -B .build

Testing
-------
ctest --test-dir .build

Usage
-----
verimat program\_file executes the program, input is read from stdin, output is written to stdout.

Todo
----
The bitstream - stdinout conversion in main.cpp is not yet tested.
Additional tests for loop operations in interpreter\_test.cpp would be nice to have.

# Test Instructions

To run these tests, execute the following commands from the repo's root:

`make tests`  
`make test-cases`  
`make test-integrations`  

Alternatively, to run these tests under GDB, execute the following commands from the repo's root:

`make tests-gdb`  
`make test-cases-gdb`  
`make test-integrations-gdb`  

Remember that `make clean` will remove the build artifacts after testing, and `make tests` and `make-tests-gdb` automatically invoke `make clean` before they begin.

## Benchmarks

For testing and comparing different potential solutions. This may be left in an incomplete state, so it might not work out of the box.

## Cases

For testing individual pieces of the source code in isolation. These are essentially the unit tests.

## Integrations

This compiles the source and repl files into a library and executable, then runs each `*.toy` file through the repl to ensure the Toy code works in practice. These are essentially integration tests.

## Mustfails

These have situations which will raise errors of some kind, to ensure that common user errors are handled gracefully. This is not yet implemented.

## Standalone

These are one-file programs that are not intended to test the source directly. Instead, these can cover a number of situations, such as the exact behavior of GitHub's workflow runners, or to generate repetitive code predictably, etc.


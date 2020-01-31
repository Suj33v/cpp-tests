# cpp-tests
Unit tests I write to understand C++ features using
[Catch2](https://github.com/catchorg/Catch2).


## Dependencies
The only dependency is Boost. The boost tests are header-only. But I have
Boost built on my system. So, boost has been included in library form.

## Usage
Go to the build folder and run `cmake .. && make`. This prepares an executable
named "main" in the bin folder. Go to the bin folder and run it with `./main`.
See command-line options available through Catch with `./main -h`.

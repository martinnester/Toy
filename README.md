<p align="center">
  <image src="toylogo.png" />
</p>

[![Running Comprehensive Tests](https://github.com/Ratstail91/Toy/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/Ratstail91/Toy/actions/workflows/c-cpp.yml)

# Preamble

The Toy programming language is a procedural bytecode-intermediate interpreted language. It isn't intended to operate on its own, but rather as part of another program, the "host". This process is intended to allow a decent amount of easy customisation by the host's end user, by exposing logic in script files. Alternatively, binary files in a custom format can be used as well.

The host will provide all of the extensions needed on a case-by-case basis. Script files have the `.toy` file extension, while binary files have the `.tb` file extension.

```
fn makeCounter() { //declare a function like this
	var total: int = 0; //declare a variable with a type like this

	fn counter(): int { //declare a return type like this
		return ++total;
	}

	return counter; //closures are explicitly supported
}

var tally = makeCounter();

print tally(); //1
print tally(); //2
print tally(); //3
```

# Nifty Features

* Simple C-like syntax
* Bytecode intermediate compilation
* Optional, but robust type system (including `opaque` for arbitrary data)
* Functions and types are first-class citizens
* Import external libraries
* Fancy slice notation for strings, arrays and dictionaries
* Can re-direct output, error and assertion failure messages
* Open source under the zlib license

# Getting Started

* [Quick Start Guide](quick-start-guide)
* [Types](types)
* [Standard Library](standard-library)
* [Timer Library](timer-library)
* [Runner Library](runner-library)
* [Game Engine](game-engine)

# Deep Dive

* [Theorizing Toy](theorizing-toy)
* [Embedding Toy](embedding-toy)
* [Using Toy](using-toy)
* [Compiling Toy](compiling-toy)
* [Developing Toy](developing-toy)
* [Testing Toy](testing-toy)
* [Roadmap](roadmap)

# Version Differences

There have been a number of versions of Toy over the years, the current actively developed version is in the `main` branch for the time being. It is recommended that you use the most recent release available.

# Types

When a variable name is declared, you may specify what kind of value can be stored within - this is known as its "type". If you attempt to store a value of a different type, an error will be raised.

```toy
//syntax
var name: type = value;

//example
var answer: int = 42;
```

Specifying a type is optional, in which case, the type is set to `any` by default ([see below](#any)).

```toy
//omit the type and set an integer value
var answer = 42;

print typeof answer; // will print "any"
```

You may access the type of a variable using the `typeof` keyword. Types can also act as values - the type of any type is `type`.

# null

The type `null` is a special case in the language, as it represents the absence of any meaningful value. It can't be specified as a variable's type, only as it's value.

Unlike the other types, the type of `null` is `null`.

# bool

The `bool` type can hold two meaningful values, either `true` or `false`.

# int

The `int` type can hold any whole number between `-2,147,483,648` and`2,147,483,647`, due to being stored as a signed 32-bit integer.

# float

The `float` type can hold a single-precision floating-point value as defined by IEEE 754, which is the most commonly used method for storing real numbers in 32 bits.

What this means in practice is that floating point errors are possible, but this is still the best option for managing decimal numbers.

# string

TODO

# array

TODO

# table

TODO

# function

TODO

# opaque

TODO

# type

TODO

# any

TODO


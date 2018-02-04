# Assignment Two

In this assignment, you will implement a simulation of semi-space garbage
collection.

Semi-space collectors are copy collectors, they work by
partitioning the available memory into two equal halves, known as "to-space" and
"from-space."  New objects are allocated into from-space at the next available
byte (an incrementing "bump pointer" keeps track of this location).

If the next allocation would not fit in the remaining from-space, garbage
collection is triggered. The collector copies all objects reachable from a "root
set" of variables into to-space, updating pointers to point at
the objects' new locations.  Copies, like allocations, are always made at the
next available byte, with a bump pointer tracking the next free byte.  Once
garbage collection is complete, the designations of "from-space" and "to-space"
are swapped, the allocation is made into the new from-space (if there is enough room), and
the program continues.


## Your Assignment

You are provided with an interpreter for a toy language, which
only supports the creation of three types of objects - Foo, Bar and Baz.

Each of the structures has pointers to other structures, which may be `nil`. The language allows the user to instantiate objects, modify their pointer fields, and bind objects to variables.

Here are a couple of examples of programs in the language, which should be very self explanatory.
Available commands are listed below.

```
x = Foo
y = Bar
z = Baz
x.c = Baz
x.d = y
PRINT

w = x.d
y = NULL
PRINT
```

The code above creates an object Foo (which is allocated on our heap), and binds it to `x`.
It then allocates a Bar, and binds it to `y`. Foo, Bar and Baz have member pointers that
can point to other objects, so on the fourth line we assign a new Baz to the field `c` of `x` (`x.c = Baz`). After a sequence of operations, we call `PRINT` to print the current state of the
heap, which contains a list of object IDs and their type.

As a sequence of allocations happen, the heap will run out of space. At this point, garbage collection will take place. Your job is to implement allocate() and collect(), everything else
is implemented for you. The methods look like this:

 - `obj_ptr Heap::allocate(int32_t size)`  
   This method should allocate `size` bytes of space and return an `obj_ptr`
   which contains the offset of the allocated space from the beginning of
   from-space.  If there is not initially enough space, this method should call
   `Heap::collect()` before allocating.  If there is still not enough space,
   it should throw an `OutOfMemoryException`.

 - `void Heap::collect()`  
   This method should implement the actual semi-space garbage collection. Take extra care
   that you update pointers inside objects - if `x.c` is pointing to an object, and the object
   gets moved, the underlying `x.c` pointer must point to the new location.

 - `void Heap::debug()`  
   This method is just a stub, provided for your convenience. It is run whenever
   the program encounters the `DEBUG` command in its input.  This command will
   never occur in the tests we use for grading, so use it to output anything you
   think will be useful when testing.

The language supports the following:
  - `x = Foo`, `x = Baz` instantiates an object of appropriate type, binding it to a variable
  - `x.c = y` allows you to set inner pointers in an object to point to other objects
  - `x.d = NULL` allows setting a pointer to NULL. This can be used to 'lose` reference of an object, making it a candidate for garbage collection.
  - `PRINT` dumps the objects currently in the from space. It prints their unique id and their type.
  - `DEBUG` will call your debug method.
  - `COLLECT` can be used to force garbage collection. This won't be used in tests, we expect that
     allocating until there is not enough space will automatically trigger a garbage collection.

Your assignment will be graded by running a large number of generated programs, and
checking the output against our reference implementation. We expect that you will
test your garbage collector with input programs you write yourself, but take care
to test all possible edge cases.

Finally, to compile the program, run `make`, or manually compile all the `.cpp` files.
You can feed program to your interpreter using `./a.out < example.program`.
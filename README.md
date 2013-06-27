cppapp
======

Private C++ command-line toolbox.  I started out as a collection of reusable
classes I wrote for various shool assignments where I was allowed to use only
my own code.

Compiles to statically-linked library.


Compilation and installation
----------------------------

There are 3 methods:

 * Use the simple `Makefile` in the project's root directory.
 * Use `$ autoreconf --install` to create `configure` script, then use the
   `configure` script to configure a `Makefile` and build/install the library.
 * Use the `init-at` script, which will create `release` and `debug`
   subdirectories with appropriatly configured Makefiles.


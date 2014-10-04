cppapp
======

Private C++ command-line toolbox.  It started out as a collection of reusable
classes I wrote for various shool assignments where I was allowed to use only
my own code.

Compiles to statically-linked library.

[![Build Status](https://travis-ci.org/nnen/cppapp.svg?branch=master)](https://travis-ci.org/nnen/cppapp)


Download
--------

The following is an installable package with `configure` script. To install,
run the `configure` script, then `make` and finally `make install`. The last
command may need administrator priviledges (`sudo make install`).

https://github.com/nnen/cppapp/releases


Compilation and Installation
----------------------------

There are 3 methods:

 * Use the simple `Makefile` in the project's root directory.
 * Use `$ autoreconf --install` to create `configure` script, then use the
   `configure` script to configure a `Makefile` and build/install the library.
 * Use the `init-at` script, which will create `release` and `debug`
   subdirectories with appropriatly configured Makefiles.


Documentation
-------------

The API reference, such as it is, can be found at http://milik.cz/~jan/cppapp/docs .


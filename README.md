# Standalone Toy

Make it convenient to learn llvm and mlir. 

## Build llvm & mlir

This is an example work-flow and configuration to get and build the LLVM source:

* ``cmake -S llvm -B build -G <generator> -DLLVM_ENABLE_PROJECTS='llvm;mlir' [options]``

   Some common build system generators are:

   * ``Ninja`` --- for generating [Ninja](https://ninja-build.org)
     build files. Most llvm developers use Ninja.
   * ``Unix Makefiles`` --- for generating make-compatible parallel makefiles.
   * ``Visual Studio`` --- for generating Visual Studio projects and
     solutions.
   * ``Xcode`` --- for generating Xcode projects.

   Some Common options:

   * ``-DCMAKE_INSTALL_PREFIX=directory`` --- Specify for *directory* the full
     path name of where you want the LLVM tools and libraries to be installed
     (default ``/usr/local``).

   * ``-DCMAKE_BUILD_TYPE=type`` --- Valid options for *type* are Debug,
     Release, RelWithDebInfo, and MinSizeRel. Default is Debug.

   * ``-DLLVM_ENABLE_ASSERTIONS=On`` --- Compile with assertion checks enabled
     (default is Yes for Debug builds, No for all other build types).

 * ``cmake --build build [-- [options] <target>]`` or your build system specified above
   directly.

   * The default target (i.e. ``ninja`` or ``make``) will build all of LLVM.

   * The ``check-all`` target (i.e. ``ninja check-all``) will run the
     regression tests to ensure everything is in working order.

   * CMake will generate targets for each tool and library, and most
     LLVM sub-projects generate their own ``check-<project>`` target.

   * Running a serial build will be **slow**.  To improve speed, try running a
     parallel build.  That's done by default in Ninja; for ``make``, use the option
     ``-j NNN``, where ``NNN`` is the number of parallel jobs, e.g. the number of
     CPUs you have.

 * For more information see [CMake](https://llvm.org/docs/CMake.html)

## Build standaloneToy

`cd standaloneToy && ./build.sh`
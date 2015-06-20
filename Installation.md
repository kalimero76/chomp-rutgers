# Installation #

The installations steps are as follows:

1. **Ensure you have an open-source environment.**

The chomp-rutgers software is an open-source project and as such requires an open-source environment. One has this on Linux by default. On Mac OS X, one needs to install Xcode and make sure to obtain the command line tools. On Windows, one needs to establish an open-source environment via mingw or cygwin.

2. **Download the code from the SVN repository (currently hosted on googlecode).**

This can be achieved by typing the following at the command prompt:
```
svn checkout http://chomp-rutgers.googlecode.com/svn/trunk/ chomp-rutgers
```
3. **Install the Boost libraries.**

You will need to first install [Boost](http://www.boost.org/). Preferably, Boost will be installed in /usr/local. However, this is not always possible (for example you may not have permissions to write to /usr/local). If you install Boost in an alternative location, you will need to [edit the makefile](makefile.md).

4. **Compile and run the tests.**

Change directory to the chomp-rutgers directory and type

```
./TESTS
```

If there are problems, see the troubleshooting section below.


## Possible Problems and Troubleshooting ##

1. You don't have a compiler.

On Mac OS X this can happen if you haven't downloaded the command line tools. They can be downloaded through the menus on Xcode. On other systems, contact your system administrator.

2. The compilation fails.

The two primary reasons the compilation can fail are (a) Boost is not installed, (b) X11 is not installed, or (c) the makefile does not know the correct locations of Boost or X11. (Note that only one of the command-line tools utilizes the X11 package). Check to see that these packages are installed and where they are installed and [[makefile|edit the makefile]] to reflect the correct locations.

If problems remains after this, please contact Shaun Harker at sharker81@gmail.com to submit a bug report.


## Next steps ##
To use the software, you can get started with the [tutorial](Tutorial.md), or dive right into the [Developer Documentation](Developer.md)
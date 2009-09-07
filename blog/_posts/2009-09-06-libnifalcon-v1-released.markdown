---
layout: post
project: libnifalcon
title: libnifalcon v1.0 released
---

After 2 years of work, libnifalcon v1.0 has been released! Here's the changelog since our last release:

* Upgraded build system support to work with new [Compily Buildd system][3]
* Integrated Alastair Barrow's new kinematics core based on the RL Stamper paper (replacing Kevin Ouellet's implementation)
* Added SWIG support with python and java builds and examples
* Removed libftdi support since libusb 1.0 works on all platforms and libftdi is the only library using non-polling updates
* Defaulted comm libraries in FalconDevice (ftd2xx on windows, libusb-1.0 everywhere else).
* Restored falcon_mouse example, since building is more stable
* Documented a whole bunch. Doxygen now puts out something resembling a manual.

The new version is available at

[https://sourceforge.net/projects/libnifalcon/files/libnifalcon/1.0/libnifalcon-1.0.tar.gz/download][1]

The documentation for v1.0 can be accessed at

[http://docs.nonpolynomial.com/libnifalcon/release/doxygen/html/][2]

I'm hoping to have new versions of the Pd/Max externals built using v1 of the library out ASAP, and will post here once that's done. I'm also planning on platform specific releases of python and java libraries, as well as any other language bindings anyone might be interested in.

Not really sure what's in store for the next version yet, as v1 covered quite a bit of things I was planning for post-v1 development. Definitely planning on starting grip hacking soon.

[1]: https://sourceforge.net/projects/libnifalcon/files/libnifalcon/1.0/libnifalcon-1.0.tar.gz/download
[2]: http://docs.nonpolynomial.com/libnifalcon/release/doxygen/html/
[3]: http://www.github.com/qdot/compily_buildd

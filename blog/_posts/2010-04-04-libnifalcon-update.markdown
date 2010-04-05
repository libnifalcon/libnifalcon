---
layout: post
project: libnifalcon
title: libnifalcon update
---

Not a whole lot to say, mostly just letting people know that no, I'm not dead, and yes, the project is still alive.

I've added a new paper to the documentation section, [Characterisation of the Novint Falcon Haptic Device for Application as a Robot Manipulator](http://docs.nonpolynomial.com/libnifalcon/pdf/CharacterizationOfANovintFalconForApplicationAsARobotManipulator.pdf).

In terms of projects using libnifalcon, there was recently a performance by Edgar Berdahl at CCRMA @ Stanford, using 5 novint falcons in a telepresence percussion piece:

<CENTER><object width="480" height="385"><param name="movie" value="http://www.youtube.com/v/R6U40LqIMOU&hl=en_US&fs=1&"></param><param name="allowFullScreen" value="true"></param><param name="allowscriptaccess" value="always"></param><embed src="http://www.youtube.com/v/R6U40LqIMOU&hl=en_US&fs=1&" type="application/x-shockwave-flash" allowscriptaccess="always" allowfullscreen="true" width="480" height="385"></embed></object></CENTER>

There are also pictures of the construction at [my flickr site](http://www.flickr.com/photos/qdot76367/sets/72157623645732117/)

Plans for the near future:

* Finally releasing binary libraries, because apparently building libnifalcon is kinda hard (not disputing that, I need some work on the build system)
* Possibly start looking at grip circuits
* Optimization of communications - We've found that on OS X, at least, the falcon takes around 20% cpu per device running due to the polling calls. This seems kind of high, and means you can't run 4+ on one machine.

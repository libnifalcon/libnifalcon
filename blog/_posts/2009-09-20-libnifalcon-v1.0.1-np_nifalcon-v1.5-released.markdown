---
layout: post
project: libnifalcon
title: libnifalcon v1.0.1 released, np_nifalcon v1.5 released
---

Busy weekend for the libnifalcon team (all one of me)!

First off, libnifalcon v1.0.1 is out. This is mostly bugfixes for using multiple falcons on top of the libusb core.

Change List:

* Fixed bug with connecting to/using multiple falcons using libusb communications
* Added adjusted geometry measurements by Niall Begley
* Fixed bug with reloading firmware through reused FalconDevice object
* Added ability to test multiple falcons to findfalcons
* Added findfalcons_multi example to test multiple falcons with simultanious access

Files are available at

[https://sourceforge.net/projects/libnifalcon/files/libnifalcon/1.0.1/][1]

Next up, np_nifalcon, the external for [Max/MSP][2] and [PureData][3], has been updated to version 1.5. I basically rewrote the external, it's much more stable now, as well as providing more management options for users. 

Source and some binary packages are available at

[https://sourceforge.net/projects/libnifalcon/files/libnifalcon Max_Pd External/1.5/][4]

Finally, to show off some of what's been going on with people using libnifalcon, here's a video of the library being used with [HAPI][5], [Blender Game Engine][6] and [Bullet][7]

<object width="425" height="344"><param name="movie" value="http://www.youtube.com/v/mVm1kYulHnE&hl=en&fs=1&"></param><param name="allowFullScreen" value="true"></param><param name="allowscriptaccess" value="always"></param><embed src="http://www.youtube.com/v/mVm1kYulHnE&hl=en&fs=1&" type="application/x-shockwave-flash" allowscriptaccess="always" allowfullscreen="true" width="425" height="344"></embed></object>

[1]: https://sourceforge.net/projects/libnifalcon/files/libnifalcon/1.0.1/
[2]: http://www.cycling74.com
[3]: http://www.puredata.info
[4]: https://sourceforge.net/projects/libnifalcon/files/libnifalcon%20Max_Pd%20External/1.5/
[5]: http://www.h3d.org
[6]: http://www.blender.org
[7]: http://www.bulletphysics.com

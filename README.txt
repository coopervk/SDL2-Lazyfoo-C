The code in this repository is based upon the work of Lazyfoo productions' SDL2
tutorial.
http://lazyfoo.net/tutorials/SDL/index.php

The programs here within are contained in directories that will not have
identical names to their Lazyfoo equivalents; however, they do use the same
numerical prefix and the title should still be verbose enough. The directories
should have all the necessary files to compile and run, and should run on most,
if not all UNIX flavored operating systems.

I have not tested on *BSD or many Linux distros. I, so far, have been developing
on Fedora. I have (clearly) not created Windows based equivalents. However,
considering this is SDL2, if you alter my code to follow the specifications for
developing for Windows (which Lazyfoo lays out on their website), this code
should be portable enough for you to use.

It should be noted that the coding practices seen here within are in _TERRIBLE_
form. Things such as the typedefs for LTexture and bool _SHOULD_ be in a header,
but they are not. I decided to fail back to this poor form for myself (to help
create muscle memory for C), and to keep mirroring the code on Lazyfoo as
closely as I can.

As of 02/23/2018, the programs which render .png's throw this warning:
	libpng warning: iCCP: known incorrect sRGB profile
If you know what causes this, feel free to help.

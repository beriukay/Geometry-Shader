Date: Tue Nov 26 03:35:48 AKST 2013
Author: Bucky Frost
File: README.txt
Purpose: Thoughts and usage of the project.

#############################
# Project notes on building #
#############################
The directories passThrough and displacement each have a separate make file.
They build that particular aspect of the project. The resulting binary can be
executed by running it with the path to the shaders.

For example:
./displacement ./shader/displacement

#################
# Compatibility #
#################
I had trouble building the project on GNU/Linux systems that were running the
MESA libraries, I think they are only implemented up the versions of 1 and 2
of the OpenGL standard. Thus, the Nvidia Linux drivers were necessary for
building the project. 

Currently using version 3 of OpenGL for the project.

**TODO**
Required hardware

#############
# Structure #
#############
Currently has a directory for each of the aspects of the project. Maybe
instead should have one source file for the project and have multiple
directors for all the different types of the shaders.

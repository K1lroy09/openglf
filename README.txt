Name: Arne Schreuder OpenGL Foundation
Version: 1.0
Description: Opengl foundation for easily creating OpenGL programs
Features: N/A
System requirements (Minimum):
	- Linux x86/x64 distrobution
	- OpenGL 3.0 compatible graphics card
Installation:
	$sudo chmod -x install.sh && sh install.sh
Uninstallation:
	N/A
Configuration:
	$sudo gedit Makefile
	//Change the parameters as required
Operating instructions:
	- Compiling:
		- $make
		//or $make all
		//This cleans and compiles
	- Cleaning:
		- $make clean
	- Compiling:
		- $make compile
	- Running:
		- $make run
	- Install:
		- $make install
		//shortcut to the installation script
Files:
	- The folder structure is set up as follows:
		- bin //contains compiled binaries
		- src //contains all source files
	- The basic file list includes:
		- install.sh //Does the OpenGL installation
		- Makefile
		- README.txt
Credits: Arne Schreuder | arn3@arn3.com
Known bugs: N/A
Change log: N/A

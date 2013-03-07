#!/bin/sh
clear
echo "Arne Schreuder OpenGL foundation"
echo "================================================="
echo "Installing Build-Essential"
echo "================================================="
sudo apt-get install build-essential
echo "================================================="
echo "Installing OpenGL"
echo "================================================="
sudo apt-get install mesa-common-dev
echo "================================================="
echo "Installing GLUT"
echo "================================================="
sudo apt-get install freeglut3
sudo apt-get install freeglut3-dev
echo "================================================="
echo "Installing GLEW"
echo "================================================="
sudo apt-get install libglew1.5
sudo apt-get install libglew1.5-dev
echo "================================================="
echo "Check if all files needed are listed below"
echo "================================================="
ls /usr/include/GL
echo "================================================="
echo "Done"
echo "================================================="

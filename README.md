Based off of [Glitter](https://github.com/Polytonic/Glitter).

Clone repo and grab dependencies:

```bash
git clone --recursive git://github.com/fmenozzi/learn-opengl
cd learn-opengl
mkdir build && cd build
```

Generate project file or Makefile for your platform. If you want to use a particular IDE, make sure it is installed; 
don't forget to set the Start-Up Project in Visual Studio or the Target in Xcode.

```bash
# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 14" ..
cmake -G "Visual Studio 14 Win64" ..
...
```

Project file or Makefile will generate one executable for each tutorial.

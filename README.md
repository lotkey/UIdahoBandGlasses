# UIdahoBandGlasses
Software to control the newly redesigned University of Idaho Band Glasses

## Requirements

1. cmake
2. make
3. Some C++17 compiler (g++, clang, etc.)
4. clang-format

The example commands are written for bash, but there should be a way to do this on a Windows terminal as well. WSL2 is a very convenient option for using Windows systems with a Linux terminal.

## Recommended

If you do not have a preferred IDE, VS Code works well with WSL2 for Windows. Some recommended extensions include:
1. C/C++ and C/C++ Extension Pack
2. Any Clang-Format extension. This will allow you to use clang-format in VS Code (Alt-Shift-F) if you have clang-format installed.

## How to build

1. Install requirements

For Linux:
```bash
sudo apt install make cmake g++ clang-format
```

2. Change directories.
For the transmitter:
```bash
cd transmitter
```
For the receiver:
```bash
cd receiver
```


3. Create a build folder (it will be ignored by git)
```bash
mkdir build
```

4. Change directories into the build folder and build
```bash
cd build
cmake ..
make
```

Optionally, you can run specify the number of jobs for make to run concurrently to speed up the process. This example is for 12 jobs.
```bash
cd build
cmake ..
make -j12
```

## Potential issues

### VS Code, Intellisense, and C++17
If you are using VS Code, you may run into issues with the Intellisense because we are using C++17. By default, the Intellisense standard for C++ is not C++17. Go to Settings, search "standard", and you should see a field called "C_Cpp > Default: Cpp Standard". Select "c++17" in the dropdown menu.
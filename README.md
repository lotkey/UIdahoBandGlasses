# UIdahoBandGlasses
Software to control the newly redesigned University of Idaho Band Glasses

## Requirements

1. WSL2
2. cmake
3. make
4. Some C++17 compiler (g++, clang, etc.)
5. clang-format

The example commands are written for bash, but there should be a way to do this on a Windows terminal as well. WSL2 is a very convenient option for using Windows systems with a Linux terminal.

## Recommended

If you do not have a preferred IDE, VS Code works well with WSL2 for Windows. Some recommended extensions include:
1. C/C++ and C/C++ Extension Pack
2. Any Clang-Format extension. This will allow you to use clang-format in VS Code (Alt-Shift-F) if you have clang-format installed.

## Setup

### WSL2
You can follow this guide by Microsoft to install WSL2: https://docs.microsoft.com/en-us/windows/wsl/install

Be sure to install WSL2 and not WSL1. If you install WSL1, you can upgrade to WSL2.

### Editing

VS Code is highly recommended. If you are using VS Code, start it from WSL2. This will start a VS Code server in WSL2 that can be connected to. It is different than running VS Code on Windows. You can start VS Code from WSL2 like so:
```bash
code .
```
Opening the repository root, receiver folder, or transmitter folder should work best.

### Everything else

Run the setup.sh script at the repo root. This will install requirements and create build directories for the transmitter and the receiver.

## Building

For the transmitter:
```bash
cd transmitter
```
For the receiver:
```bash
cd receiver
```

Change directories into the build folder and build
```bash
cd build
make
```

Optionally, you can run specify the number of jobs for make to run concurrently to speed up the process. This example is for 12 jobs.
```bash
cd build
make -j12
```

## Potential issues

### VS Code, Intellisense, and C++17
If you are using VS Code, you may run into issues with the Intellisense because we are using C++17. By default, the Intellisense standard for C++ is not C++17. Go to Settings, search "standard", and you should see a field called "C_Cpp > Default: Cpp Standard". Select "c++17" in the dropdown menu.

## Coding standards

### Legibility

Team members should be able to read your code and generally understand it. Some guidelines that can help:
- Keep in mind what somebody using your code is interested in. For example, somebody using one of your classes would be interested in the public methods, so declare all the public fields in a class first.
- Expanding on the above guideline, there should be 3 types of includes: files in this project, third-party libraries, and built-in libraries. They should be included in that order (and separated).
- Use descriptive variable/function/class names. This means full words, unless there is a standard or popular abbreviation (img, mph, kB, etc.). 
- Try to keep functions small. If you have a very large function, break it up into multiple if you can.
- Comments. Every function prototype should have a comment unless it's very obvious what the function does ("int max(int, int)" for example). Nobody should have to read the implementation to understand how to use your code. Class declarations should be commented and show what purpose the class serves (not just what it is).
- Classes start with capital letters. Variable names can be camelCase or have underscores_instead. Small variable names may not need either (numrows vs numRows vs num_rows).
- Classes should be split into a header file and source file(s), except for smaller classes.
- Static non-public class member variables should have the "s_" prefix.
- Non-static non-public class member variables should have the "m_" prefix.
- Use clang-format to consistently format your code.

### Pointers

Pointer membership is important. If a pointer is an "owning pointer", then it owns the data that is pointed to. This means it is responsible for freeing the memory.
- Use the STL library for dynamic data structures like vectors, sets, and maps. They clean up their memory when done.
- Use references over pointers when possible.
- Use smart pointers whenever a pointer is needed (unique_ptrs preferably, shared_ptrs only when necessary). Smart pointers automatically free memory when they are destructed.
- If a raw pointer is necessary (deleted move/copy constructors or assignment or something), be sure to document it with comments. Comment whether or not it is an owning pointer and is responsible for cleaning up the memory when done, or if it is a non-owning pointer and the class is not responsible for freeing the memory.
- If you are using raw pointers in a class, carefully define a destructor (if the class is responsible for freeing the memory) and a copy constructor (if you want a deep copy vs. a shallow copy).
- If you define a destructor, define it as virtual. This ensures that the right destructor is called when an object is virtually bound.

### References and Const-ness

Passing objects by reference can increase performance (passing a reference instead of copying an object and passing the copy).
- Pass object arguments by reference whenever possible.
- If the object must not be changed, pass it by a const reference.
- Primitives do not need to be passed by reference (int, bool, etc.). There is likely no performance gain, and the compiler will probably optimize it away anyways.
- There are some cases in which it would be easier or make more sense to pass a modifiable copy. Document it, if needed.
- Declare all class functions that do not modify a class's data as const (this is so that the functions can be used with a const object or reference).

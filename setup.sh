#!/bin/bash

function run_no_output() {
    eval $1 > /dev/null 2>&1
}

echo "Installing requirements..."
run_no_output "apt install make cmake g++ clang-format git code"
echo "Requirements installed."
echo ""

echo "Creating build directory for transmitter..."
run_no_output "cd transmitter"
if [ -d "build" ]
then
    echo "Build directory already exists."
else
    run_no_output "mkdir build"
fi
run_no_output "cd build"
echo "cmaking transmitter..."
run_no_output "cmake .."
run_no_output "cd ../.."
echo "transmitter was cmade."
echo ""

echo "Creating build directory for receiver..."
run_no_output "cd receiver"
if [ -d "build" ]
then
    echo "Build directory for receiver already exists."
else
    run_no_output "mkdir build"
fi
run_no_output "cd build"
echo "cmaking receiver..."
run_no_output "cmake .."
run_no_output "cd ../.."
echo "receiver was cmade."
echo ""

echo "Done."
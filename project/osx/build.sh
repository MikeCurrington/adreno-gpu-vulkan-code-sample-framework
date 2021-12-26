#!/bin/sh
mkdir solution
pushd solution
cmake -G "Unix Makefiles" ..
make
popd


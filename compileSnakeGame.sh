#! /bin/sh

mkdir build
cd build
rm -rd ./**
cmake -DCMAKE_BUILD_TYPE=Release ../src
cmake --build . --target all -- -j $(nproc)
cd ..
ln -sf bin/Release/SnakeGame SnakeGame
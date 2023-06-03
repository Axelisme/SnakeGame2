#! /bin/sh
mkdir allegro_mac/build
cd allegro_mac/build
rm -rf CMakeFiles
cmake --install-prefix $(realpath ..) ..
cmake --build . --target install --clean-first -- -j $(nproc)
cd ../..

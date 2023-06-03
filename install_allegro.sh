#! /bin/sh
mkdir allegro_mac/build
cd allegro_mac/build
cmake --install-prefix $(realpath ..) ..
cmake --build . --target install -- -j $(nproc)
cd ../..

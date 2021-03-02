# Prerequisites

<!--- ubuntu toolchain --->
sudo apt-get install gcc g++ cmake
sudo apt-get install libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi libncurses5-dev build-essential bison flex libssl-dev bc gcc-arm-linux-gnueabi g++-arm-linux-gnueabi

<!--- Dependencies, installs to /usr/local/include/ and /usr/local/lib/ --->
git clone https://github.com/unicorn-engine/unicorn.git
cd unicorn
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DUNICORN_ARCH=arm -DUNICORN_BUILD_SHARED=ON ..
make
sudo make install
cd ../..

git clone https://github.com/serge1/ELFIO.git
cd ELFIO
mkdir build
cd build
wget https://github.com/Kitware/CMake/releases/download/v3.18.6/cmake-3.18.6-Linux-x86_64.sh
sh cmake-3.18.6-Linux-x86_64.sh --skip-license --prefix .
bin/cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
cd ../..

git clone https://github.com/MerryMage/ext-boost dynarmic-boost
git clone https://github.com/MerryMage/dynarmic.git
cd dynarmic
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBoost_INCLUDE_DIRS=${PWD}/../../dynarmic-boost -DDYNARMIC_TESTS=OFF ..
make
sudo find -iname '*.a' -exec cp {} /usr/local/lib/ \;
sudo cp -R ../include/dynarmic /usr/local/include/
cd ../..

# Build

mkdir jit-bench-build
cd jit-bench-build
cmake -DCMAKE_C_COMPILER=arm-linux-gnueabi-gcc -DCMAKE_CXX_COMPILER=arm-linux-gnueabi-g++ -DCMAKE_BUILD_TYPE=Release /mnt/c/Users/vdwje/Desktop/homebrew/3DS/jit-bench
make tester
mv tester tester_arm
rm CMakeCache.txt && cmake -DCMAKE_BUILD_TYPE=Release /mnt/c/Users/vdwje/Desktop/homebrew/3DS/jit-bench
make
mv tester tester_x86

# Run

<!-- should be empty (dynamic symbols) -->
readelf -d tester_arm
readelf -D -s tester_arm
readelf -r tester_arm
<!-- entry points & load -->
readelf -l tester_arm
<!-- symbols -->
readelf -Ws tester_arm

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/usr/local/lib
time ./tester_x86 100000
time ./benchmark_unicorn
time ./benchmark_dynarmic

# Documentation

cross compiler : https://www.acmesystems.it/arm9_toolchain
understanding readelf : https://greek0.net/elf.html

https://linuxhint.com/what-is-a-linux-system-call/
https://chromium.googlesource.com/chromiumos/docs/+/master/constants/syscalls.md
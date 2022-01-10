mkdir -p build
cd build || exit
if [ "$1" == "darwin" ]; then
 cmake -DCMAKE_C_COMPILER=gcc \
      -DCMAKE_CXX_COMPILER=g++ .. 
elif [ "$1" == "windows" ]; then
 cmake -DCMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
      -DCMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ .. 
else
  echo "No target provided. Use \"darwin\" or \"windows\""
  exit 1
fi
make
cd .. || exit

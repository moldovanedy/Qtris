QT_PATH=$HOME/Qt/6.8.0/gcc_64

cd obj
cmake ../src -DCMAKE_PREFIX_PATH=${QT_PATH} -DCMAKE_BUILD_TYPE=Debug
cmake --build .

chmod +x Qtris
# ./Qtris
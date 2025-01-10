set QT_PATH=C:/Qt/Qt-6.8.0

cd obj
cmake ../src -DCMAKE_PREFIX_PATH=%QT_PATH% -DCMAKE_BUILD_TYPE=Debug
cmake --build .
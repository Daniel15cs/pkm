pkill PKM_03_qml

cd ./build/Qt_6_8_2_qt6-Debug/
cmake --build . && ctest -j 4 --output-on-failure --test-dir ./tests/ && ./src/appPKM_03_qml


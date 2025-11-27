# QtAppBase

Base Qt classes to be used within Qt applications.

Qt5 and Qt6 are supported.

To run cmake, make sure path to Qt is provided by providing passing `-DCMAKE_PREFIX_PATH=C:/Qt/6.3.1/mingw_64/lib/cmake_t` when running cmake.

This repo is still a work in progress...

### Build
CMake options needed when building:
```
-DCMAKE_PREFIX_PATH=X:/Toolchains/Qt/6.7.3/mingw_64/lib/cmake 
-DQBASE_LIB_TYPE=STATIC 
```

Can be added in CLion under `File >> Settings >> Build, exec, deploy >> CMake`

### Run
To run in debug mode, make sure working directory points to:
```
X:\Toolchains\Qt\6.7.3\mingw_64\bin
```

It can be done from `Target >> Working directory`
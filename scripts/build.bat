::rd /s /q build32
::rd /s /q build64
mkdir build32
mkdir build64

cd ..
cd build32
cmake -G "Visual Studio 15 2017" ..
msbuild WinImm.sln
cd bin/Debug
ImeInstaller.exe --imePath WinImm.ime --imeName WinImm
cd ../../..

cd build64
cmake -G "Visual Studio 15 2017 Win64" ..
msbuild WinImm.sln
cd bin/Debug
ImeInstaller.exe --imePath WinImm.ime --imeName WinImm
cd ../../..
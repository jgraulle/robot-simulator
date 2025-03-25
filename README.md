Setup
=====

- If you do not have access to epita gitlab, follow the robot-git.pdf common documentation
- For personal Linux:
    - `sudo apt install g++-12 make libjsoncpp-dev libsfml-dev libasio-dev`
- For Windows only:
    - Install VS code for Windows from https://code.visualstudio.com/download
    - In VS code install `C/C++ Extension Pack`
    - from https://www.msys2.org/ install msys2-x86_64-20250221.exe
    - from "MSYS2 UCRT64" console execute
        - `pacman --sync mingw-w64-ucrt-x86_64-gcc`
        - `pacman --sync --needed base-devel mingw-w64-ucrt-x86_64-toolchain` (Press enter to install all)
        - `pacman --sync mingw-w64-ucrt-x86_64-sfml`
        - `pacman --sync mingw-w64-ucrt-x86_64-asio`
        - `pacman --sync mingw-w64-ucrt-x86_64-jsoncpp`
    - Add the path to your MinGW-w64 bin folder to the Windows PATH environment variable by using the following steps:
        - In the Windows search bar, type Settings to open your Windows Settings.
        - Search for Edit environment variables for your account.
        - In your User variables, select the Path variable and then select Edit.
        - Select New and add the MinGW-w64 destination folder you recorded during the installation process to the list. If you used the default settings above, then this will be the path: C:\\msys64\\ucrt64\\bin.
        - Select OK to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available.
    - from https://cmake.org/download/ dowload and install cmake-3.31.6-windows-x86_64.msi, with Add CMake to the PATH for all users
    - open a new Command Prompt (Windows key + "cmd") and type:
        - gcc --version
        - g++ --version
        - gdb --version
        - cmake --version
- From https://gitlab.cri.epita.fr/jeremie.graulle/ssie-s9-robot-simulator create a personal fork
to be able to commit
- Add jeremie.graulle as Maintainer of this project.
- Clone your fork on your computer:
`git clone git@gitlab.cri.epita.fr:<name>/ssie-s9-robot-simulator.git`
- Open VS code from this fork: code . &
- In menu "View" option "Extensions", search and install the extension `C/C++ Extension Pack`
- Build and Run
- If there are some trouble to build from button, try build from console:
    - on Linux with make: `mkdir buildMake && cd buildMake && cmake -G"Unix Makefiles" .. && make`
    - on Linux with ninja: `mkdir buildNinja && cd buildNinja && cmake -GNinja .. && ninja`
    - on Windows from a Command Prompt (Windows key + "cmd") : `mkdir build && cd build && cmake -G "MinGW Makefiles" .. && cmake --build .`

Use
===

You can move the robot using keyboard arrow or keyboard key:

- z and x: to move forward and backward
- s: stop
- q and d: to rotate in place in anti-clockwise and clockwise
- a and e: to turn forward only right wheel (or only left wheel)
- w and c: to turn backward only right wheel (or only left wheel)

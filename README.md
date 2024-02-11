Setup
=====

- If you do not have access to epita gitlab, follow the robot-git.pdf common documentation
- For personal Linux:
    - `sudo apt install g++-12 make libjsoncpp-dev libsfml-dev libasio-dev`
- For Windows only:
    - Install VS code for Windows from https://code.visualstudio.com/download
    - In VS code install `C/C++ Extension Pack`
    - from https://www.msys2.org/ install msys2-x86_64-20231026.exe
    - from msys console execute `pacman -S mingw-w64-ucrt-x86_64-gcc` and `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain` (Press enter to install all)
    - Add the path to your MinGW-w64 bin folder to the Windows PATH environment variable by using the following steps:
    - In the Windows search bar, type Settings to open your Windows Settings.
    - Search for Edit environment variables for your account.
    - In your User variables, select the Path variable and then select Edit.
    - Select New and add the MinGW-w64 destination folder you recorded during the installation process to the list. If you used the default settings above, then this will be the path: C:\\msys64\\ucrt64\\bin.
    - Select OK to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available.
    - open a new Command Prompt and type:
    - gcc --version
    - g++ --version
    - gdb --version
    - from https://cmake.org/download/ dowload and install cmake-3.28.0-rc3-windows-x86_64.msi, with Add CMake to the PATH for all users
    - from msys console execute `pacman -S mingw-w64-ucrt-x86_64-sfml` and `pacman -S mingw-w64-ucrt-x86_64-asio`
- from https://gitlab.cri.epita.fr/jeremie.graulle/ssie-s9-robot-simulator create a personal fork
to be able to commit
- Add jeremie.graulle as Maintainer of this project.
- clone your fork on your computer:
`git clone git@gitlab.cri.epita.fr:<name>/ssie-s9-robot-simulator.git`
- open VS code from this fork: code . &
- In menu "View" option "Extensions", search and install the extension `C/C++ Extension Pack`
- Build and Run
- If there are some trouble to build from button, try build from console:
    - with make: `mkdir buildMake && cd buildMake && cmake -G"Unix Makefiles" .. && make`
    - with ninja: `mkdir buildNinja && cd buildNinja && cmake -GNinja .. && ninja`

Step1
=====

For each step below you can test your modification by moving the robot using keyboard arrow or
keyboard key:

- z and x: to move forward and backward
- s: stop
- q and d: to rotate in place in anti-clockwise and clockwise
- a and e: to turn forward only right wheel (or only left wheel)
- w and c: to turn backward only right wheel (or only left wheel)

Create a branch tp1 in your personal fork, for each following step, you have to do 1 clean commit
in this branch. Do not commit generated files. Look for the following TODO and follow the
associated instructions:

- in Robot::update() search for "TODO Step1"
    - Check angular unit for `setRotation()` and the kinematic model
    - Use reel constant (use cppreference)
    -  If you want to move faster by using coefficient, you have to keep the following behavior: when press
    'a' or 'e' we want only one wheel turn and the other do not move, make the robot turn with the
    rotation center on the side of the robot.
- in LineTrackSensor::update() search for "TODO Step1"
    - Small mistake replace `_value = false;` by `_value = 0;`
- in IrProximitySensor::update() search for "TODO Step1"
    - Before going for complex calcul, try to understand
       `_line[1].position = worldTranform.transformPoint(sf::Vector2f(_distanceDetected, 0.0));`
- in SwitchSensor::update() search for "TODO Step1"
- in Robot::update() search for "TODO Step2"
- in IrProximitySensor::update() search for "TODO Step2"
- in EncoderWheelSensor::update() search for "TODO Step1"

Once done create a merge request from tp1 branch to main.

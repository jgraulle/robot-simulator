Setup
=====

- If you do not have access to epita gitlab, follow the epitaGitlabSshKey.pdf common documentation
- For Windows only:
    - Install VS code for Windows from https://code.visualstudio.com/download
    - In VS code install `C/C++ Extension Pack`
    - from https://www.msys2.org/ install msys2-x86_64-20231026.exe
    - from msys console execute `pacman -S mingw-w64-ucrt-x86_64-gcc` and `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain` (Press enter to install all)
    - Add the path to your MinGW-w64 bin folder to the Windows PATH environment variable by using the following steps:
    - In the Windows search bar, type Settings to open your Windows Settings.
    - Search for Edit environment variables for your account.
    - In your User variables, select the Path variable and then select Edit.
    - Select New and add the MinGW-w64 destination folder you recorded during the installation process to the list. If you used the default settings above, then this will be the path: C:\msys64\ucrt64\bin.
    - Select OK to save the updated PATH. You will need to reopen any console windows for the new PATH location to be available.
    - open a new Command Prompt and type:
    - gcc --version
    - g++ --version
    - gdb --version
    - from https://cmake.org/download/ dowload and install cmake-3.28.0-rc3-windows-x86_64.msi, with Add CMake to the PATH for all users
    - from msys console execute `pacman -S mingw-w64-ucrt-x86_64-sfml` and `pacman -S mingw-w64-ucrt-x86_64-asio`
- from https://gitlab.cri.epita.fr/jeremie.graulle/ssie-s9-robot-simulator create a personal fork
to be able to commit
- clone your fork on your computer:
`git clone git@gitlab.cri.epita.fr:<name>/ssie-s9-robot-simulator.git`
- open VS code from this fork: code . &
- In menu "View" option "Extensions", search and install the extension `C/C++ Extension Pack`
- Build and Run

Step1
=====

For each step below you can test your modification by moving the robot using keyboard arrow or
keyboard key:

- z and x: to move forward and backward
- s: stop
- q and d: to rotate in place in anti-clockwise and clockwise
- a and e: to turn forward only right wheel (or only left wheel)
- w and c: to turn backward only right wheel (or only left wheel)

You have to do a commit in your personal fork after completed each step.

Look for the following TODO and follow the associated instructions:

- in Robot::update() search for "TODO Step1"
- in LineTrackSensor::update() search for "TODO Step1"
- in IrProximitySensor::update() search for "TODO Step1"
- in SwitchSensor::update() search for "TODO Step1"
- in Robot::update() search for "TODO Step2"
- in IrProximitySensor::update() search for "TODO Step2"
- in SpeedSensor::update() search for "TODO Step1"

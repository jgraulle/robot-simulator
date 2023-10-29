Setup
=====

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

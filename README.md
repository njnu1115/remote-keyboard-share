# Remote keyboard share

## Situation

My scenario is: 

- two monitors
	- one with HiDPI
	- the other without
- one workstation with Ubuntu 20.04
- one MacBook Pro
- Most of the time I type on MacBook but occacionally need to type into Ubuntu.
- Don't want to put two keyboards on desk

## Task

Find a solution to do "remote typing"

- xdotool not working in Wayland
- vnc not working in Wayland
- looking into /dev/input/eventX devices and find way to manipulate it

## Action

- write a tool on macOS to capture keyevents. Capturing the letters typed is not enough. What we need is not a clipboard. 
- convert the captured keyCode or keyEvents to bytes based on linux kernel definition
- use ssh to setup a tunnel between macOS and Ubuntu
- send the bytes out from macOS to socket
- write code on Ubuntu to listen the socket and receive bytes
- write the bytes to /dev/input/eventX device

## Result

- it works as expected
- some keyevent not well converted in macOS side, e.g. Up Down Left Right
- simply compile the kbd.c by 
```
clang kbd.c 
```
- sudo is needed to run the binary build from kbd.c in Ubuntu. 
- No sudo needed in macOS side for capturing keyEvents

## Have fun

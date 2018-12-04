# C Snake game

This is a terminal-based snake game I make in 2015.
It uses ncurses for graphics. 
The controls are wasd or arrow keys.

![Screenshot](/../master/screenshot.png?raw=true "Screenshot")

## Building on ubuntu linux
```
sudo apt install build-essential libncursesw5-dev
make
./build/bin/csnek
```

## Notes
The design is a little weird because I was originally testing out
an opaque pointer pattern while writing it, but later I decided against using it.
The gamelogic is built into a dll because I was testing out make.

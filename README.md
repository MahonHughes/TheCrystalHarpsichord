# The Crystal Harpsichord: _Using physical modelling synthesis methods to create a glass harpsichord with diamond strings_

The Crystal Harpsichord is a physical modelling synthesiser emulating a glass harpsichord with diamond strings.

_You can listen to example sounds_ [_here_](Generated%20Wav%20Files/readme.md)

<img width="912" alt="The GUI" src="https://github.com/MahonHughes/TheCrystalHarpsichord/assets/34442699/5bf44602-d610-48a3-886d-732a29a39641">

## Compiling The Crystal Harpsichord

Edit the makefile to include the path to your C++ compiler

The following dependancies should be installed:
 - portaudio
 - libsndfile
 - SDL2

On MacOS, these can be installed with:
```
brew install portaudio libsndfile sdl2
```

Navigate to the project directory.

**The project can be compiled and run using the command** `make run`

To compile without running, use command `make`

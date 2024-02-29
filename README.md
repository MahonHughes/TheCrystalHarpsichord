# The Crystal Harpsichord
### Using physical modelling synthesis methods to create a glass harpsichord with diamond strings

The Crystal Harpsichord is a physical modelling synthesiser emulating a glass harpsichord with diamond strings. 

_You can listen to example sounds_ [_here_](Generated%20Wav%20Files/readme.md)

<img width="912" alt="The GUI" src="https://github.com/MahonHughes/TheCrystalHarpsichord/assets/34442699/5bf44602-d610-48a3-886d-732a29a39641">

**Abstract**


_Music and musical instruments are a core part of what makes us human. With modern computer systems and music creation software, musicians have a virtually endless supply of choice in software instruments and synthesisers. Physical modelling synthesis is a method employed to use a computer to model how an instrument works and sounds. This paper seeks to use physical modelling synthesis methods to model an instrument that is physically possible yet completely impractical to make. The instrument chosen to model was a diamond-strung harpsichord made of glass. A model was developed using C++ using a form of commuted synthesis. The program allows for customisation of the properties of the instrument and is highly adaptable. This was a success and produced the first sound output of The Crystal Harpsichord. The project has wider implications, showing it is possible to model and use a different category of instrument that is not limited by practical constraints._


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

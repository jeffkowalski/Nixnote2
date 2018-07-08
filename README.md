# NixNote2(+)
## Introduction

(Fork/clone of) nixnote2 - An unofficial client of Evernote for Linux.

This version contains the original code, selected changes merged from other forks and my changes.
The aim is:
* Make the application more stable
* Improve status info (sync, error cases)
* Minor usability improvements (mainly focused on my personal needs)
* [TODO list](docs/TODO.md)
* [CHANGELOG](docs/CHANGELOG.md)


## Building from source

Tha app is mainly targeted at Linux, but it should compile quite easily on Windows and
also macOS config is already present (see more detailed info bellow). It could be
that minor adjustments are needed for the non linux builds.

### Linux

* Install development dependencies
  * Install basic dependencies (recipe for Ubuntu, use similar for other distros)
    * ```sudo apt-get install -y git-core qt5-default build-essential```
    * ```sudo apt-get install -y libboost-dev libboost-test-dev libboost-program-options-dev libevent-dev```
  * Install qt5 and poppler
    * ```sudo apt-get install -y libpoppler-qt5-dev```
    * ```sudo apt-get install -y libqt5webkit5-dev```
    * ```sudo apt install -y qt5-qmake```
  * ~Optional
    * ```sudo apt-get install -y libopencv-dev libhunspell-dev```
  * As alternative you can download qt5 directly from [qt.io/download](https://www.qt.io/download).
    Currently Qt supported version is 5.5. Building against 5.6+ needs source changes
    (this is on TODO list).

* Build tidy library
  * clone [source code](https://github.com/htacg/tidy-html5) switch to master branch
  * generate makefile
  ```bash
  cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_DEBUG_SYMBOLS:BOOL=TRUE -DBUILD_SHARED_LIB:BOOL=TRUE -DCMAKE_INSTALL_PREFIX=~/usr
  ```
  * build
  ```bash
  make
  ```
  * install
  ```bash
  make install
  ```
  * library is now copied to ~/usr

* Get latest source from github...
  * I recommend using `master` branch. There maybe feature/* or release/* available, but this may
    not be stable. Anyway there isn't any guarantee for `master` branch either :-)

* Build
```bash
env PATH=/usr/bin:/bin PKG_CONFIG_PATH=$HOME/usr/lib/pkgconfig qmake NixNote2.pro MOC_DIR=./build RCC_DIR=./build OBJECTS_DIR=./build
make
```

If all went well, you should have a ./qmake-build-release/nixnote2 file.

* Optional: create [AppImage package](https://appimage.org/) using [linuxdeployqt](https://github.com/probonopd/linuxdeployqt)
```bash
# Optional second step: if all went well you may try to create AppImage package
# may be a bit tricky - recommended for advanced users
./development/create-AppImage.sh
```

### macOS

Disclaimer: macOS build was added from [RJVB/nixnote2](https://github.com/RJVB/nixnote2).
I can't currently test if it works.  It is quite probable that it will need minor adjustments.

```bash
mkdir build
cd build
qmake ../nixnote2/NixNote2.pro
make
```

Upon successful completion you will have the NixNote2.app bundle in the build directory.

Here, qmake is the one from Qt5. You will need to have Qt5 installed (qtbase, qtdeclarative and qtwebkit),
as well as pkgconfig, poppler-qt5, hunspell and curl; dependencies can come from MacPorts, Fink or HomeBrew (I use MacPorts).
It should be possible to use official Qt5 packages too but I haven't tested that.

The resulting application still depends MacPorts (or Fink or HomeBrew). To turn this into a standalone app bundle that can be
deployed anywhere:

```bash
> cd build
> macdeployqt NixNote2.app [-no-strip]
```

As far as I can tell this will find and copy all required dependencies into the app bundle and modify them so they
can be loaded from inside that bundle (wherever it ends up).

## Binaries
Upcoming...

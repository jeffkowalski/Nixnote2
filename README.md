# NixNote v2.1
## Introduction

Nixnote is Evernote desktop client for Linux.

* [Getting started](https://github.com/robert7/nixnote2/wiki/Getting-started)
* [New features in v2.1](https://github.com/robert7/nixnote2/wiki/New-features-in-v2.1)
* [CHANGELOG](https://github.com/robert7/nixnote2/blob/master/debian/changelog)
* [Howto install & binary downloads](https://github.com/robert7/nixnote2/releases)
  * For Ubuntu users its recommended to install native package from [NixNote PPA](https://github.com/robert7/nixnote2/wiki/NixNote-PPA) (although the AppImage version will also work).
  * For any linux distribution: [download AppImage, mark as executable & run](https://github.com/robert7/nixnote2/wiki/HowTo---Run-AppImage)

## Building from source

This app is mainly targeted at Linux, but it should compile quite easily on Windows and
also macOS config is already present (see more detailed info bellow). As lot of refactoring
has been made and I can't currently try anything else then linux, it is quite probable
that minor adjustments are needed for the all non linux builds.

### Linux - docker build
**Recommended build** for all but advanced users is using docker.
This should work out of the box, no fiddling with any dependencies
is needed. The created binary image should work on all ~recent distributions (at least
in theory).
Basic docker familiarity with docker is helpful.

More info in: [DOCKER README](docs/DOCKER-README.md)

### Linux - manual build
* Install development dependencies - look in content of [this docker file](development/docker/Dockerfile.ubuntu_xenial)
  to see example what is needed for Ubuntu 16.04. If you use another distribution/version,
  you need to find the right packages.
  * Install basic dependencies (recipe for Ubuntu, use similar for other distros)
    * ```sudo apt-get install -y git-core qt5-default build-essential```
    * ```sudo apt-get install -y libboost-dev libboost-test-dev libboost-program-options-dev libevent-dev```
  * Install qt5 and poppler
    * ```sudo apt-get install -y libpoppler-qt5-dev```
    * ```sudo apt-get install -y libqt5webkit5-dev```
    * ```sudo apt-get install -y qttools5-dev-tools```
    * ```sudo apt install -y qt5-qmake```
  * Qt: you can either get Qt packages for your distribution or as alternative you can download qt5 directly
    from [qt.io/download](https://www.qt.io/download).
  * ~Optional
    * ```sudo apt-get install -y libopencv-dev libhunspell-dev```
  * Tidy library
    * Alternative 1: Install from PPA:
      * `sudo apt install tidy-dev`
    * Alternative 2: Build tidy library from source:
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
env PATH=/usr/bin:/bin PKG_CONFIG_PATH=$HOME/usr/lib/pkgconfig qmake NixNote21.pro MOC_DIR=./build RCC_DIR=./build OBJECTS_DIR=./build
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
qmake ../nixnote2/NixNote21.pro
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
> macdeployqt NixNote21.app [-no-strip]
```

As far as I can tell this will find and copy all required dependencies into the app bundle and modify them so they
can be loaded from inside that bundle (wherever it ends up).

### Windows
Should work on Windows, but minor tweaks will be needed to make it run.
I currently have no time for it. Pull request is welcome. No sure its worth the effort, as there is
quite decent official Evernote client for Windows.

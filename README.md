# NixNote v2.1
## Introduction

Nixnote is Evernote desktop client for Linux and macOS.

* [Getting started](https://github.com/robert7/nixnote2/wiki/Getting-started)
* [Features](https://github.com/robert7/nixnote2/wiki/Features)
* [CHANGELOG](https://github.com/robert7/nixnote2/blob/master/debian/changelog)
* [Contributing](CONTRIBUTING.md)

Travis CI [![Build Status](https://travis-ci.com/robert7/nixnote2.svg?branch=master)](https://travis-ci.com/robert7/nixnote2)

## Packages
### Debian, Ubuntu and derivatives official repositories
In case you distribution is based on **Debian 10 (Buster) or Ubuntu 19.04 (Disco) or later distribution
versions**, you can install Nixnote 2.1 from official repositories using:

``` bash
sudo apt update
sudo apt install nixnote2 -y
```
But there maybe newer version in the PPA (see bellow).
Note: on older distributions the "nixnote2" may also be available, but you may get the older [2.0 version](https://github.com/baumgarr/nixnote2).

### Ubuntu
NixNote PPA - reflects the latest [stable release](https://github.com/robert7/nixnote2/wiki/Releases---versions%2C-build-pipeline%2C-branches%2C-tags#stable-releases). More information can be found on [NixNote PPA wiki page](https://github.com/robert7/nixnote2/wiki/NixNote-PPA). Installation commands:

``` bash
sudo add-apt-repository ppa:nixnote/nixnote2-stable -y
sudo apt update
sudo apt install nixnote2 -y
```
PPA packages are available for Ubuntu 16.04 (Xenial) and newer.

Additionally there is a "[development branch PPA](https://code.launchpad.net/~nixnote/+archive/ubuntu/nixnote2-develop)" available.
The usage is same as for "stable PPA", just replace the name "nixnote2-stable" with "nixnote2-develop".
Just please take care, that you don't enable both stable and development PPA.
At most times, the "[development release](https://github.com/robert7/nixnote2/wiki/Releases---versions%2C-build-pipeline%2C-branches%2C-tags#development-releases)"
should be OK for daily use.

### AppImage
This is suitable to **any ~recent linux distribution**.
Installation is trivial - download AppImage file, mark as executable & run.
More information can be found on [NixNote AppImage wiki page](https://github.com/robert7/nixnote2/wiki/HowTo---Run-AppImage).

Two builds are available:
* [Stable build](https://github.com/robert7/nixnote2/releases/tag/continuous) - it reflects the latest [stable release](https://github.com/robert7/nixnote2/wiki/Releases---versions%2C-build-pipeline%2C-branches%2C-tags#stable-releases) - tip of the `master` branch - same as the PPA or AUR version.
* [Development build](https://github.com/robert7/nixnote2/releases/tag/continuous-develop) - it reflects the latest [development release](https://github.com/robert7/nixnote2/wiki/Releases---versions%2C-build-pipeline%2C-branches%2C-tags#development-releases) - tip of the `develop` branch..

### Other
I can't provide support for packaging for other distributions that Ubuntu PPA and the AppImage
but here are links to further community builds:

#### Arch Linux
AUR package [nixnote2-git](https://aur.archlinux.org/packages/nixnote2-git/)
builds the latest [stable release](https://github.com/robert7/nixnote2/wiki/Releases---versions%2C-build-pipeline%2C-branches%2C-tags#stable-releases).

#### Gentoo Linux
NixNote is available via a custom portage overlay (that lives [here][overlay]). It can be installed by running the following commands:
``` bash
layman -o https://raw.githubusercontent.com/bbugyi200/portage-overlay/master/repository.xml -f -a bbugyi200
emerge nixnote:2
```

[overlay]: https://github.com/bbugyi200/portage-overlay

#### Fedora
https://copr.fedorainfracloud.org/coprs/nunodias/nixnote2/

#### OpenSUSE
https://software.opensuse.org/package/nixnote2

## Building from source

This app is mainly targeted at Linux, but it should compile quite easily on Windows and
also macOS config is already present (see more detailed info bellow). As lot of refactoring
has been made and I can't currently try anything else then linux, it is quite probable
that minor adjustments are needed for the all non linux builds.

Application is developed using [Clion](https://www.jetbrains.com/clion/) IDE
using open source licence from [JetBrains](https://www.jetbrains.com/?from=nixnote2).

### Linux - docker build
This should work out of the box, no fiddling with any dependencies
is needed. The created binary image should work on all ~recent distributions (at least
in theory).
Basic familiarity with docker is helpful.

More info in: [DOCKER README](docs/DOCKER-README.md)

### Linux - manual build
* Install development dependencies - look in the content
  of [debian/control](https://github.com/robert7/nixnote2/blob/master/debian/control)
  to see example, what is needed for Ubuntu. If you use another distribution/version,
  you may need adjust packages.
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
      * `sudo apt install libtidy-dev`
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
./development/build-with-qmake.sh
```
`build-with-qmake.sh` is just kind of convenience script. You can also build without it like:
`qmake CONFIG+=debug PREFIX=appdir/usr`, then `make && make install`.

This supposes you installed libtidy in system default location (recommended version is 5.6.0).

In case you installed tidy from nixnote (e.g. using package `nixnote2-tidy` from Nixnote PPA), then
the could command could be `./development/build-with-qmake.sh debug noclean /usr/lib/nixnote2/tidy`.

If all got OK, you should have "qmake-build-debug/nixnote2" binary available now
(and also a deployment copy in appdir).
I suggest running from "appdir" (e.g. `./appdir/usr/appdir/nixnote2`).


* Optional: create [AppImage package](https://appimage.org/) using [linuxdeployqt](https://github.com/probonopd/linuxdeployqt)
```bash
./development/create-AppImage.sh
```

### macOS
Build from source. Basically same as for linux.

```bash
./development/build-with-qmake.sh
```
`build-with-qmake.sh` is just kind of convenience script. You can also build without it like:
`qmake CONFIG+=debug PREFIX=appdir/usr`, then `make && make install`.

Upon successful completion you will have the NixNote2.app bundle in the build directory (e.g. qmake-build-debug/NixNote2.app).

Dependencies can come from MacPorts, Fink or HomeBrew (I use MacPorts).
It should be possible to use official Qt5 packages too but I haven't tested that.
Tested with following macPorts packages: qt5, qt5-qtwebkit, poppler-qt5, hunspell, boost, tidy.

The resulting application still depends MacPorts (or Fink or HomeBrew). To turn this into a standalone app bundle that can be
deployed anywhere:

```bash
> cd build
> macdeployqt NixNote2.app [-no-strip]
```

As far as I can tell this will find and copy all required dependencies into the app bundle and modify them so they
can be loaded from inside that bundle (wherever it ends up).

### Windows (legacy)
Should work on Windows, but minor tweaks will be needed to make it run.
I currently have no time for it. Pull request is welcome. No sure its worth the effort, as there is
quite decent official Evernote client for Windows.

## Donations
If you would like to support the project, you can send me some little amount via paypal: https://paypal.me/nixnote2

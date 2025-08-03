# Cashed

## ABANDONED WILL BE REIMPLEMENTED (as per decissions in task_list)

Cashed is a caching server built entirely in plain c.
It is similar in functionality to Memcached, providing a very simple unsecured (not SSLed) tcp api interface.

you can also directly connect to the server using telnet or netcat.

## Setup instructions
**Install dependencies :**
 * [Cutlery](https://github.com/RohanVDvivedi/Cutlery)
 * [BoomPar](https://github.com/RohanVDvivedi/BoomPar)
 * [ConnMan](https://github.com/RohanVDvivedi/ConnMan)
 * [LockKing](https://github.com/RohanVDvivedi/LockKing)

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/Cashed.git`

**Build from source :**
 * `cd Cashed`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using the library
 * add `-lcashed -lconnman -lboompar -llockking -lpthread -lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<cashed_hashtable.h>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cashed`
 * `sudo make uninstall`

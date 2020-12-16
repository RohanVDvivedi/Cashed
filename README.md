# Cashed

Cashed is a caching server built entirely in plain c.
It is similar in functionality to Memcached, providing a very simple unsecured (not SSLed) tcp api interface.

you can also directly connect to the server using telnet or netcat.

## Setup instructions
**Install dependencies :**
 * [Cutlery](https://github.com/RohanVDvivedi/Cutlery)
 * [BoomPar](https://github.com/RohanVDvivedi/BoomPar)
 * [ConnMan](https://github.com/RohanVDvivedi/ConnMan)
 * [ReaderWriterLock](https://github.com/RohanVDvivedi/ReaderWriterLock)

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/Cashed.git`

**Build from source :**
 * `cd Cashed`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using the library
 * add `-lcashed -lm -lconnman -lboompar -lrwlock -lpthread -lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<cashed_client.h>`     *to use cashed client library for cashed server (tcp/ipv4)*
   * `#include<cashed_hashtable.h>`  *to build an embedded cashed_hashtable, i.e. cashtable*

## Using the binaries
 * To start a cashed server (default tcp port = 6969)
   * `cashed` or `cashed <port>`
 * To start a cashed shell client (default tcp port = 6969, client connections default to 3)
   * `cashed_client` or `cashed_client <port>`

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cashed`
 * `sudo make uninstall`

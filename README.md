# Cashed

Cashed is a caching server built entirely in plain c.
It is similar in functionality to Memcached, providing a very simple unsecured (not SSLed) tcp api interface.

you can also directly connect to the server using telnet or netcat.

## Setup instructions

**Download source code :**
 * `git clone https://github.com/RohanVDvivedi/Cashed.git`

**Build from source :**
 * `cd Cashed`
 * `make clean all`

**Install from the build :**
 * `sudo make install`
 * ***Once you have installed from source, you may discard the build by*** `make clean`

## Using The library
 * add `-lcashed -lconnman -lboompar -lrwlock -lpthread -lcutlery` linker flag, while compiling your application
 * do not forget to include appropriate public api headers as and when needed. this includes
   * `#include<cashed_server.h>`     *to start custom cashed server*
   * `#include<cashed_client.h>`     *to use cashed client library to communicate with cashed_server*
   * `#include<cashed_hashtable.h>`  *to build an embedded cashed_hashtable, or cashtable as I call it*

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cashed`
 * `sudo make uninstall`
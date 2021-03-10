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
   * The cashed_client compiles to the binary cashed_client, you may use thid simplistic implementation to as a "HOW TO" to use the networked API.
 * Cashed uses an ASCII-binary protocol over tcp, this allows us to use telnel and netcat like utility programs to connect to Cashed server.
   * `telnet <host> <port>`
   * `netcat <host> <port>` or `nc <host> <port>`
   	 * host may be localhost.
 * Network API calls
   * Each network call OR a query statement (over netcat or telnet) looks like a function call in c.
     * starts with a function name: `get`, `set` or `del`.
     * then a list of parameters enclosed in `()` and separated by a `,`.
       * each parameter is regarded as a string and params are NOT enclosed in `""`.
       * *Currently the Network API does not support spaces in the parameter*.
     * at the a `;` to denote the end of the statement
   * To get a value
     * `get(<key>);`
   * To set a value
     * `set(<key>,<value>).`
   * To set a value with an expiry (in unsigned integral seconds)
     *  `set(<key>,<value>,<expiry_seconds>);`
   * To delete a value
     * `del(<key>);`

## Instructions for uninstalling library

**Uninstall :**
 * `cd Cashed`
 * `sudo make uninstall`

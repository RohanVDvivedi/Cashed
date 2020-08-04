# Cashed

Cashed is a caching server built entirely in plain c.
It is similar in functionality to Memcached, providing a very simple unsecured (not SSLed) tcp api interface.

you can directly connect to the server using telnet or netcat.

setup instructions
 * git clone https://github.com/RohanVDvivedi/Cashed.git
 * cd Cashed
 * sudo make clean install
 * add "-lcashed -lconnman -lboompar -lrwlock -lpthread -lcutlery" linker flag, while compiling your application

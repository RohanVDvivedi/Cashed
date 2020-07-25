# Cashed

Cashed is a caching serer built entirely in plain c.
It is similar in functionality to Memcached, providing a very simple unsecured (not ssled) tcp api interface.

setup instructions
 * git clone https://github.com/RohanVDvivedi/Cashed.git
 * cd Cashed
 * sudo make clean install
 * add "-lcashed -lconnman -lboompar -lrwlock -lpthread -lcutlery" linker flag, while compiling your application
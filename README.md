dvfs-odroid
===========

A C++ native Android app that does dynamic frequency scaling to achieve a target frames-per-second (FPS). FPS is provided via command line arguments. Has been tested to work only on Nexus 5 and Odroid-XU.

Designed to work with the frontend app [here](https://github.com/yeokm1/dvfsapp/).

Eclipse settings for windows

Project -> Properties -> C/C++ General -> Paths and Symbols

Add the following to include directories

C:\android-ndk-r10d\toolchains\arm-linux-androideabi-4.6\prebuilt\windows-x86_64\lib\gcc\arm-linux-androideabi\4.6\include  
C:\android-ndk-r10d\toolchains\arm-linux-androideabi-4.6\prebuilt\windows-x86_64\lib\gcc\arm-linux-androideabi\4.6\include-fixed

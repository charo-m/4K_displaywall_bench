
# 4K Displaywall Bench

While most benchmarks are intended for the gaming industry, this simple app is for testing fillrate-bound applications such as those that need to drive high resolution displays, or many displays, or both.

#Building

##ISPCTextureCompressor
   git clone https://github.com/GameTechDev/ISPCTextureCompressor.git ../ISPCTextureCompressor
   (follow readme.md)

##Ubuntu 16.04

    apt-get install libxrandr-dev libxinerama-dev libxcursor-dev libglfw3-dev libglm-dev

    cmake .

    make
    
##Windows

    cmake -G "Visual Studio 14 2015" .

    cmake --build .

Use cmake flag -DCMAKE_BUILD_TYPE=Debug to enable OpenGL debug output extension (4.3 and higher)


#Running

    ./4K_displaywall_bench -help

    ./4K_displaywall_bench -width 11520 -height 2160 -swap_interval 0

Use the width and height to define the viewport, which may span multiple displays.

If your driver does not support OpenGL 4.5 (GLXBadFBConfig), run with -gl 3.3 for a version 3.3 context.

#Testing

Requires Python 2.7.  Invokes test application with different parameters.  Press Esc key to quit each invocation of the test application.

##Linux

    ./run_test.py

##Windows

(tested in Git Bash terminal)

    winpty C:/Python27/Python.exe run_win_test.py

#License

4K_displaywall_bench is freely available for download under the terms of the [BSD3](https://github.com/Oblong/4K_displaywall_bench/blob/master/COPYING "COPYING") License.

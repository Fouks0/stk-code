# SuperTuxKart 1.0 TAS Mod

This is a Mod of SuperTuxKart, adding some TAS tools to it in order to perform TASes.

If you landed here, it is assumed that you have advanced knowledge of the STK gameplay/mechanics, and some basic TAS notions. Else, not sure why you are here Lol.

To use these tools, you must also know how to build STK from sources. Revisit the original repository if needed - https://github.com/supertuxkart/stk-code/

Read the game's Help to learn more about TASing STK.

## Building from source

Only binaries for Linux or Windows are possible. No other system is supported.

Go below for execution.

### Common instructions

There is a partial asset folder in Build/bin/assets. You need to download the original 1.0 Assets in order to complete the Build/bin/assets folder: from the original assets, copy the library, music, textures, and tracks folders to Build/bin/assets and you are done. They are not included in the repository in order to avoid issues with having too much stuff here.

### Building SuperTuxKart on Linux

Get the source code with for example Git Clone and the dependencies listed in the original STK instructions. Then,
 
```bash
# Go into the stk-code/Build directory with cd (or open a terminal in the correct folder)
cd /path/to/stk-code/Build
 
# Run cmake to generate the Makefile
cmake ..
 
# Compile (to use more threads for compilation, do for example "make -j 8" to use 8 threads
make
```

### Building SuperTuxKart on Windows

Download this Mod's source code using your preferred way (for example, download it as Zip from the GitHub website).

Install and set up Visual Studio by following the official instructions, Once done, compile in Debug Mode with Cmake and Visual Studio.

## Executing this Mod

In order to do so, the assets folder needs to be next to the binary, which should automatically be the case in Linux. In Windows, Visual Studio might have created the binary somewhere else so you might need to either copy/move the Assets folder there or move the binaries in Build/bin/assets.

Note that the user folders (config, replays,...) are next to the binary and the assets, in order to have everything at the same place and avoid having to looking for these folders.

You can now start TASing. In the main menu, open the Help in order to learn more. I am looking forward to seeing your TASes!

One important thing, reproducibility is unfortunately not always fulfilled. I tested the Mod in 2 computers, one with Windows and one with Linux, and the inputs created from the Linux one fail in the Windows one. I was however able to get them working in some other Linux computers. I will try to find out why there is this divergence and maybe mod a bit the internals in order to enfore reproducibility, at the acceptable cost of actually TASing a game negligibly different than the original.

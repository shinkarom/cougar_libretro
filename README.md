# cougar-libretro

![Bouncingball project](images/bball.jpg)

**Cougar** is a fantasy console, inspired by Nintendo Entertainment System. It is a libretro core.

## Features

* Programming: JavaScript
* Resolution: selectable, from 16x16 to 256x256
* Color: 32 bit
* Sprites: 8 pages by 256 sprites = 2048
* Tilemap: 64x64 tiles
* Sound: OPL3

## Building

To build **Cougar**, you must have [CMake](https://cmake.org) and a build system of your choice.

1. Download the repository.
2. Create a `build` folder there.
3. Move into the `build` folder.
4. Run `cmake .. -G "Ninja"` (CMake supports many different build systems; [Ninja](https://ninja-build.org) is just one of them).
5. When finished, move back to the repository root.
6. Run `cmake --build build`.
7. When finished, you will find `cougar_libretro.dll` or `cougar_libretro.so` in the `bin` folder.

## Running

Run the RetroArch executable with the following command, specifying the path to the **Cougar** library after building, along with the game file:

```
retroarch -L path/to/cougar_libretro.so path/to/game_file
```

For example, if you are on a GNU/Linux system and located in the repository root, you can run:

```
retroarch -L build/cougar_libretro.so build/snake.cart
```

## Installing

### On Linux

Copy the shared object file to the cores directory of RetroArch:

```
cp bin/cougar_libretro.so ~/.config/retroarch/cores/
```

Copy the info file into the same directory:

```
cp bin/cougar_libretro.info ~/.config/retroarch/info/
```

### On macOS

Copy the dynamic library to the cores directory:

```
cp bun/cougar_libretro.dylib ~/Library/Application\ Support/RetroArch/cores/
```

Copy the info file:

```
cp bin/cougar_libretro.info ~/Library/Application\ Support/RetroArch/info/
```

### On Windows

Copy the DLL file to the cores directory of RetroArch:

```
copy bin\cougar_libretro.dll "C:\Program Files\RetroArch\cores\"
```

Copy the info file:

```
copy bin\cougar_libretro.info "C:\Program Files\RetroArch\info\"
```

## Creating Games

Copy and rename the `sdk/template` folder. The `help.txt` file provides documentation.

I use `pnpm` build system, and I will demonstrate on it.

`pnpm install` to install the prerequisites.

`src` folder holds the source code, `tiles` folder the tiles.

`pnpm run build` to create the output cartridge in the `output` folder (you should create the folder manually beforehand). The file name is determined by `name` field  `package.json`.

If at `scripts/run-retroarch.js` you put in the commands to run RetroArch and the path to the core file, `pnpm run run` will run the cartridge from there.

`pnpm run zip` will zip the project folder and place it into `output` folder.

## Credits

Made in 2024 by Roman "shinkarom" Shynkarenko (https://shinkarom.github.io).
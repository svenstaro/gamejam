# Löve2D Project Preset

This is a small helper repository to get you started coding a nice game with Löve2D.

## Requirements

* Makefile and Bash scripts are not good for Windows, don't you try to feed them to the nasty Microsoft stuff. For Linux only!
* You should have Löve2D installed on your system, so you don't have to build the binary package all the time for testing

## Building

The following make targets are available:

    clean:              clears all generated data
    build:              packages the data and sources into ```game.love```
    run:                runs ```love game.love``` for you
    package-all:        creates all binary packages into ```pkg/```
    package-<platform>: generates binary package for one of the supported platforms

Just type ```make``` to perform the **build** and **run** targets.

## Development

Put your data in the ```data/``` folder, and your sources into ```src/```. Make sure to adjust ```src/config.lua```. Add resources to the resource manager in ```src/main.lua```. You can access them via

    resources.images.<image-name>
    resources.fonts.<font-name>
    etc...

from everywhere in your code.

### Class Helper

The class helper can be used like this:

    require("util/helper")

    ClassName = class("ClassName")

    function ClassName:__init(constructor, arguments)
        self.foo = "bar"
    end

    function ClassName:customMethod(parameter)
        -- do stuff
    end

Inheritance works like this:

    require("util/helper")

    SecondClass = class("SecondClass", ClassName)

    -- overwriting a method
    function SecondClass:customMethod(parameter_x)
        -- do other stuff
    end

### Game States

To create a new game state, inherit it from the GameState class (util/gamestate). Then create it in the reset() function in the ```src/main.lua```. To switch to a state, call stack:push(the_state) somewhere in your code. To close a state, run stack:pop(). See the intro state in ```src/intro.lua``` for an example.

## License

This program is free software. It comes without any warranty, to the extent permitted by applicable law. You can redistribute it and/or modify it under the terms of the Do What The Fuck You Want To Public License, Version 2, as published by Sam Hocevar. See http://sam.zoy.org/wtfpl/COPYING for more details.

             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
                       Version 2, December 2004 

    Copyright (C) 2004 Sam Hocevar <sam@hocevar.net> 

    Everyone is permitted to copy and distribute verbatim or modified 
    copies of this license document, and changing it is allowed as long 
    as the name is changed. 

               DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE 
      TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION 

     0. You just DO WHAT THE FUCK YOU WANT TO. 

The following files and directories are not part of this project and were redistributed under their respective licenses:

* data/DejaVuSans.ttf [LICENSE](http://dejavu-fonts.org/wiki/License)
* lib/linux_x86 (license file included)
* lib/linux_x64 (license file included)
* lib/windows_x86 (license file included)
* lib/windows_x64 (license file included)

# PokemonSMS C
A C implementation of PokemonSMS, for stupidity purposes
Built using autoconf

Copyright (C) 2019  Connor Horman

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

## Included Programs & License Exemptions 

`openssl` is released under the terms of the Apache License. It is  Copyright (c) 1998-2018 The OpenSSL Project.

`curl` is released under the terms of a License Derived from the MIT License. Copyright (c) 1996 - 2019, Daniel Stenberg. 

`sqlite` is dedicated by its developers to public domain. This dedication is preserved by the program. See <https://www.sqlite.org/copyright.html> for details on this dedication. 

`lua` is released under the terms of the MIT License. It is Copyright (C) 1994–2019 Lua.org, PUC-Rio.

`zlib` is released under its license. (C) 1995-2017 Jean-loup Gailly and Mark Adler. Permission is granted as an exemption of the GNU General Public License to link against the provided or a system version of zlib.

`PokemonSMS-Core` is licensed under the GNU Lesser General Public License Version 3. (Copyright (C) 2018-2019 Connor Horman)

`random` and `libconinteract` are both licensed under the terms of the MIT License (Copyright (C) 2018-2019 Connor Horman)

PokemonSMS is a fan made project consisting of a specification and a set of Core Libraries, for which an implementation can be built off the specification. The Core Libraries and the Implementation are codependant but perfectly substitutable. Neither PokemonSMS, the PokemonSMS Public Specification Project, nor this program are affiliated with or endorsed by Nintendo, Game Freak, or The Pokemon Company. 
The project and all components are not offered for a fee, and are believed to fall under Fair Dealing in the Canadian Copyright Act. It is simiarily believed to fall under Fair Use in the United States of America, and similar provisions of copyright law applicable in other jurisdictions. Pokemon, all Pokemon games, and trademarks (registered and unregistered) related to Pokemon are owned by Nintendo, Game Freak, or The Pokemon Company.

### Utility Programs

Several programs of varying size are under the `util` and are built for linking purposes. These programs achieve various tasks necessary to execute the game, but not otherwise available. 
The programs include
`io` a LCS 4 (tentative, currently available as Connor Horman LCSD 1) compatible Binary IO Library.

`shadenbt` which is used to save game progress in the ShadeNBT Format described [here](https://chorman0773.github.io/BinarySpecifiations/ShadeNBT). 

`pkmcom` which implements the PkmCom Abstract Protocol and a Number of Base Protocols, including ones not part of the PkmCom Specification. 

`collect` a basic C library that provides various data structures. 


## Building

This is a work-in-progress, however you can build the completed portions of this project. 
To build, change to a separate directory (other than the source directory, `build` is recommended), then execute the `configure` script in the source directory. After the configuration finishes, you can type make to build all completed portions. 
Initial builds will take a considerable ammount of time, due to depedencies on openssl and sqlite (which are packaged as submodules). 
In the future it may be possible to build against system versions of these libraries instead of inline built versions. 
Such versions will not be portable on GNU/Linux, however. 

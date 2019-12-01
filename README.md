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

## Building

This is a work-in-progress, however you can build the completed portions of this project. 
To build, change to a separate directory (other than the source directory, `build` is recommended), then execute the `configure` script in the source directory. After the configuration finishes, you can type make to build all completed portions. 
Initial builds will take a considerable ammount of time, due to depedencies on openssl and sqlite (which are packaged as submodules). 
In the future it may be possible to build against system versions of these libraries instead of inline built versions. 
Such versions will not be portable on GNU/Linux, however. 

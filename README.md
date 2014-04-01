<!---
 PpM - Pong Plus More - A pong clone full of surprises written with C++11.
 Copyright (C) 2013  Luke San Antonio

 You can contact me (Luke San Antonio) at lukesanantonio@gmail.com!

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
-->
'Mo Pong - Pong Plus More (PpM)
====================

A pong clone full of surprises written with C++11

Pong Plus More isn't just any pong clone. It will feature fairly funny easter
eggs and trolls, paddles that have a mind of their own, and a multiplayer mode
which supports all of this. It is released under the GPL so you'll be able to
play PpM for free, though I haven't got an alpha out yet, so you'll just have
to wait.

Thanks for your interest!

I am also managing the development with Trello, so if you want you can find my
board [here][1].

-
Luke, '13

Planned Features
----------------

I hope Pong Plus More could someday be the go-to pong game for everyone! I plan
to add:

 - Scripting to control arbitrary paddles and a "Level Editor" to go with that!
   - The maps and scripts can be bundled into a package and distributed as a
   gamemode.
   - Don't know about any particular scripting language... Most likely Python.
 - Complete Network Transparency. The scripting API will do the best job it can
 in allowing the developer to avoid special cases involved when a player
 abstraction isn't that great...
 - Some other stuff that I can't remember at the moment.
 - Awesome Portability, Compatibility, and Configurability.
   - I at least want Pong Plus More to run on my Raspberry Pi so I can get it
   on my TV.
   - It would be pretty cool to work with the Wii remote tilt function.
   - Joystick/Controller support is a must.
   - What the hell, why not have it run in a console, while playing against a
   friend, I don't discriminate.

Roadmap
-------

##### Alpha v0.1 "Pong for the masses."
This will be the first alpha, including a Single player game mode with an AI.

*Expected Release*: April 3st, 2014.

##### Alpha v0.2 "The Plugin Update."
This update will bring a plugin system similar to that of the new fangled
[neovim][2].

That is, a control system based on asynchronous JSON-RPC communication between
the program and an arbitrary executable (interpreter!) and PpM. All this can
go through stdout and stdin using the async functions in Boost.Asio.

This idea starts to get *really* interesting when you think of the options you
have in printing to stdout. You could write a plugin in a language from Linux
assembly and C to [Piet][4], Java and Python. On that note I will be providing
an API for Python so that there is no need to learn the protocol.

##### Alpha v0.3 "Socially Viable!"
This update will introduce a multiplayer mode!

##### Beta v1.0 "'Dat Beta."
This will just bring general stability and hopefully some useful and exciting
plugins.

Also included will be an extension to my idea of "Awesome Portability,
Compatibility, and Configurability." That is, this update will bring the
crazy stuff like Wiimote/Joystick/Controller integration and Raspberry Pi
support. That being said, if your lucky I'll even add a terminal interface
(ncurses), we'll see how that turns out.

Building
--------
Probably the biggest thing that might be a problem when building is the C++14
support required. (I am trying to cut down in the interest of portability
across compilers).

Here's how you do it:

#### Prerequisites
On Arch:

    $ sudo pacman -S --needed base-devel cmake sdl2 git

Arch should have no problem with the (really) recent C++14 code as this is the
system I do development on.

These distributions may have a problem, just because I can't test building on
them. Eventually, once I cut down on the C++14 code I'll set up Travis for
continuous integration, so the support for building on these systems should
get better.

On Dpkg-Based systems (Debian, Ubuntu) - Debian Squeeze (oldstable) and Wheezy
(stable) may or may not work as the clang is old and gcc is at 4.7, which I'm
pretty sure won't work. Ubuntu should work fine though.

    $ sudo apt-get install build-essential cmake libsdl2-dev git

On Yum-Based systems (Fedora) - Fedora should compile it fine, but I'm not
sure about Red Hat.

    $ sudo yum install gcc gcc-c++ make cmake SDL2-devel git

#### Compiling

##### Linux:
Get the source:

    $ mkdir -p ~/build/PpM
    $ cd ~/build/PpM
    $ git clone https://github.com/thee-l/PongPlusMore

Build it:

    $ mkdir build && cd build
    $ cmake ../PpM/
    $ make -j4

Enjoy it:

    $ ./src/client/ppm

##### Mac OS X:
Soon, I'll add a [Homebrew][3] formula for PpM so you can install it right
font your terminal. For now though, just use it to install dependencies and
follow the Linux compiling constructions above.

##### Windows:
Pfft. Have fun man!

No, that's not it. When the game is actually worth downloading I'll have a
prebuilt binary uploaded. The only reason it can't be built in Visual Studio
is because of the compiler and its lack of support for C++11/14. Soon enough,
I will try to minimize the C++14 usage and hopefully support compilation on
this compiler.

It might be possible to use MinGW since they have g++ 4.8.1 ported. But no
guarantees.

Prebuilt Binaries
-----------------
Coming Soon. - Luke

FAQ
---

#### Why Pong, Luke? It's Pong! Who cares about Pong!
This game is basically a learning experience. If I happen to make a great pong
clone in the process, then awesome!

#### Luke, you've been working on this game since March, what's taking so long?
Eh, procrastination mostly. Development *has* picked up recently though, which
is nice!

#### Why the GPL?
I thought about the MIT license and the BSD license, but my conclusion was
that even though those license are great for libraries, for PpM I really
do think that any changes to should benefit everyone. That's why I picked the
GPL!

[1]: <https://trello.com/b/6kkfz2kJ/pong-plus-more>
[2]: <https://github.com/neovim/neovim#new-plugin-architecture>
[3]: <http://brew.sh/>
[4]: <http://www.dangermouse.net/esoteric/piet.html>

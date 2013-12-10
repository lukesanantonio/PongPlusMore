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
Pong Plus More (PpM)
====================

A pong clone full of surprises written with C++11

Pong Plus More isn't just any pong clone. It will feature fairly funny easter
eggs and trolls, paddles that have a mind of their own, and a multiplayer mode
which supports all of this. It is released under the GPL so you'll be able to
play PpM for free, though I haven't got an alpha out yet, so you'll just have
to wait... Thanks for your interest! I am *sort of* managing the development
with Trello, you can find my board, in all its glory, [here][1]. I can't
imagine it very interesting, but it should soon be a solid snapshot of my
progress thus far, though there are no guarantees! =D

-
Luke, '13

Planned Features
----------------

I hope Pong Plus More could someday be the go-to pong game for everyone! I plan
to add:

 - Scripting to control arbitrary paddles and a "Level Editor" to go with that!
   - The maps and scripts can be bundled into a package and distributed as a
   gamemode.
   - Don't know about any particular scripting language... Possibly Javascript,
   Python, or [any of these][2].
 - Complete Network Transparency. The scripting API will do the best job it can
 in allowing the developer to avoid special cases involved when a player
 abstraction isn't that great...
 - Configurable. It'd be nice if a player could change the resolution of their
 pong game. So why not!
 - Some other stuff that I can't remember at the moment.


Building
--------
Probably the biggest thing that might be a problem when building is the C++11
support. It seems like most Linux distributions come with a fairly recent GCC
compiler version these days, so it might not be a problem for you. I have GCC
4.8.2 and Clang 3.3 on Arch Linux and it compiles fine.

Here's how you do it:

#### Prerequisites
On Arch:

    $ sudo pacman -S --needed base-devel cmake sdl2 git

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
    $ git clone https://github.com/thee-l/PpM.git

Build it:

    $ mkdir build && cd build
    $ cmake ../PpM/
    $ make -j4

Enjoy it:

    $ ./src/client/PpM

##### Mac OS X:
The steps should work similar to how it's built on Linux. The hard part is
getting the dependencies.

##### Windows
Pfft. Have fun, man! Hint: It is possible.

Prebuilt Binaries
-----------------
Coming Soon - Luke

FAQ
---

#### Why Pong, Luke? It's Pong! Who cares about Pong!
This game is basically a learning experience. If I happen to make a great game
in the process, then great!

#### Why is the trello page so out of date
You mean [this one][1]? Yeah, it's pretty bad now, but I am working on
organizing it! Just sit tight and check back every so often...

#### Luke, you've been working on this game since March, what's taking so long?
I don't know, I am getting a bit too worried over design that I barely make any
progress... However, recently I have been able to code and produce fairly
alright code.

Also I have set an unofficial deadline of March 4th, 2014 for an
early alpha. An early alpha will include multiplayer support with flying
paddles, at the least.

#### Why the GPL, you seem like a reasonable guy, why not the MIT License?
I thought about the MIT license or something similar (like the Zlib license).
My conclusion was that I do believe in a more liberal license generally,
however in this case, I really do think that any changes to PpM should benefit
everyone. That's why I picked the GPL!

[1]: <https://trello.com/b/6kkfz2kJ/pong-plus-more>
[2]: <http://codeplea.com/game-scripting-languages>

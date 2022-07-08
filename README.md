# ModShot-Core

[![ModShot CI](https://github.com/Astrabit-ST/ModShot-Core/actions/workflows/ci.yml/badge.svg)](https://github.com/Astrabit-ST/ModShot-Core/actions/workflows/ci.yml)

**ModShot** is a even more MORE specialized fork of a specialized fork of ([mkxp by Ancurio](https://github.com/Ancurio/mkxp) designed for [OneShot](http://oneshot-game.com/)) for OneShot mods.

Thanks to [hunternet93](https://github.com/hunternet93) for starting the reimplementation of the journal program!

Thanks to [rkevin-arch](https://github.com/rkevin-arch) for the docker build!

Thanks to [somedevfox](https://github.com/somedevfox) for helping ModShot ditch conan!

Thanks to [hat_kid](https://github.com/thehatkid) for fixing a lot such as wallpaper bindings, unicode Win32 username issue, and introducing
[Win32 tray ballons](https://docs.microsoft.com/en-us/windows/win32/shell/notification-area) and
[GTK notifications](https://developer-old.gnome.org/GNotification/)!

> mkxp is a project that seeks to provide a fully open source implementation of the Ruby Game Scripting System (RGSS) interface used in the popular game creation software "RPG Maker XP", "RPG Maker VX" and "RPG Maker VX Ace" (trademark by Enterbrain, Inc.), with focus on Linux. The goal is to be able to run games created with the above software natively without changing a single file.
>
> It is licensed under the GNU General Public License v2+.

*ModShot* also makes use of [steamshim](https://hg.icculus.org/icculus/steamshim/) for GPL compliance while making use of Steamworks features. See LICENSE.steamshim.txt for details.
You can compile ModShot with steam without compiling steamshim, but you will need to source the steamshim binary yourself.

## Building

Conan is no longer supported. Long live conan.
Instead, ModShot now makes use of meson, bash, and make, in a fairly simple setup. You only need to compile dependencies once.

Because of this, compiling with Visual Studio as was standard before is no longer supported, and instead compiling using the msys2 toolset is required.
The main upshot of this, of course, is remaining on par with ruby in terms of gem support. (at least for windows)

Previously, C extensions were very jank with ModShot, **however** now you can use a C extension right from your own Ruby install!
(Provided the version is the same, and the msys2 evironment is the same. I'll get back to this later.)

## Using clang

//TODO

# FMOD

FMOD support is very... interesting. It involves a lot of licensing, legalese, etc.
If you want to enable FMOD support, download FMOD and extract the zip into a folder named `fmod`.
You will then need to pass `-Dfmod=true` as an option.

ModShot should handle everything from there, it's up to you to follow the FMOD license.

FMOD bindings also do not supply the `Audio` module. You may create wrapper functions for that if you wish.
AL Effect bndings are not supplied as well for obvious reasons.

Functions will always return an array of values (usually two) in the order of `[result, values...]`.
This means you can do `result, value = FMOD.some_func()`, which is pretty neat, right? If a result is not `FMOD_OK` (0) there will be no return value. Keep that in mind!

The FMOD bindings won't hold your hands either- You will need to clean up after yourself.
Because of the way the bindings work as well calling the same function twice will **NOT** return the "same" object. Fundamentally, it is the same object, as the C++ side object is the same, but it is a brand new object as far as ruby is concerned.

Because of this behavior, you can quite easily cause a memory leak by repeatedly storing an object returned in an array somewhere constantly loaded such that ruby will not garbage collect it.
i.e
```rb
array = []
100000.times do
    array << bank.get_event_list # Bad will memory leak!
end
```
So, be mindful of what you write! Luckily instances of objects from these bindings are very small so it's not a big deal if your code isn't perfect, but **PLEASE** do be mindful of this! RMXP already has a similar problem with Bitmaps, so if you've dealt with them you should know what to do.
There is an `==` operator provided (TODO) that will check if an object is the same for you as well. You can usually assign a value `nil` to get it garbage collected.

A feature is provided to automatically release **some** FMOD objects (Like System, Bank, etc) but it is disabled by default.
This is because some FMOD functions will allow you to "get back" an object after it has been garbage collected from ruby, and this feature messes with that.
I would not suggest using it because if you are, you're likely doing something wrong and are relying on a bandaid fix to avoid it being an issue.

The bindings should generally line up with what's documented in the latest FMOD docs- although things like `FMOD_Studio_System_Create` are hidden away under `FMOD::Studio::System.new` instead.

I hope that's enough info to get you started! There will be some direct conversions of FMOD examples in the `scripts` folder when I get to it. (TODO)

### Options

```
RUBY_VER && -Dmri_version (default 3.1) sets the ruby version.
-Dsteam (default false) sets the build to use steam.
--build-type (default Release) sets the build type.
-Dbuild_static (default true) sets the build to be static. (True is faster, but with longer startup times.)
-Dfmod (default false) toggles FMOD instead of OpenAL.
-Dauto_clean_fmod (default false) toggles auto releasing SOME FMOD objects when garbage collected.
```

## Building on Windows

First, you'll need to download [msys2](https://www.msys2.org/) and install it.
Then, you'll want to determine what Ruby version you're using, as this will determine what build environment you'll be using.
As is, ModShot is set up to use Ruby 3.1, so keep that in mind. Please refer to this table to determine the environment.
(You *can* use the wrong environment and it will work fine, just not with C extensions.)

```
Ruby >3.1 UCRT64 (Default)
Ruby 3.0-2.0 MINGW64
Ruby <1.9 MINGW32 (NOT SUPPORTED!)
```

Once you've figured out the environment you need to use, pull up an msys2 shell of it.
(You can search the environment name in the Windows search box to open it)
From there, it's pretty much identical to the Linux setup.

```sh
# Install dependencies from package manager
sh setup.sh

# Build extra dependencies (like ruby)
cd windows; make
source vars.sh

cd ..; meson build --prefix="$PWD/build.out" --bindir=.
cd build && ninja install
```

## Building on Linux

Building on Linux is fairly easy, as long as you're using one of the supported distros. (Manjaro, Debian/Ubuntu, Fedora/Red Hat)
Unlike Windows, you don't have to worry about msys2 environments. Just use gcc and you'll be good to go.
If you're not, fear not, as you can usually just install all the dependencies right from your package manager. See `setup.sh`.

```sh
# Install dependencies from package manager
sh setup.sh

# Build extra dependencies (like ruby)
cd linux; make
source vars.sh

cd ..; meson build --prefix="$PWD/build.out" --bindir=.
cd build && ninja install
```

This should create a folder called `out` with your build of ModShot all ready to go!

## Configuration

*ModShot* reads configuration data from the file "oneshot.conf". The format is ini-style. Do *not* use quotes around file paths (spaces won't break). Lines starting with '#' are comments. See 'oneshot.conf.sample' for a list of accepted entries.

All option entries can alternatively be specified as command line options. Any options that are not arrays (eg. preloaded scripts) specified as command line options will override entries in oneshot.conf. Note that you will have to wrap values containing spaces in quotes (unlike in oneshot.conf).

The syntax is: `--<option>=<value>`

Example: `./oneshot --gameFolder="oneshot" --vsync=true`

## External gems

Modshot builds come pre-packaged with the ruby standard library in `/lib/ruby/`. You can require gems from this folder at any point by using `require '<gem>'`.

You can ship your own gems by finding the gem install location (Typically `C:\Ruby27-x64\lib\ruby\gems\2.7.0\gems`), going inside the gem, and copying over all the files inside lib.

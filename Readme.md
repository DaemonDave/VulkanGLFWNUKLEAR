# Nuklear-TEMPLATES

# INTRODUCTION

Dear visitor,

This repository, issue tracker, etc. forks https://github.com/Immediate-Mode-UI/Nuklear, https://github.com/m0ppers/nuklear-glfw-vulkan
into a separate repository that allows people to use various build systems like cmake, gmake, and automake/autoconf 
together by augmenting the orignal repositories as one locus for people to check into

TO STUDENTS, SELF-TAUGHT, The CURIOUS, and CONTRACTORS;

I used to coach science fair students, and supervise competitors at the national science fair. Top winner I coached was awarded $35,000 in scholarships.

Adding to this project is a good one to demonstrate knowledge, outline the value of work potential to understand science, and develop parts of solutions.

Rather than logging stuff to a log book, make a window that logs data in edit windows and saves, then make chart and graph subwindows to show the data.
You can add octave functionality as a C function to run.  Store your example workng here and drag out the templates ( edit label, two check 
marks and a save button for experiment daily logging, as an off-the-cuff example).

The more you demonstrate as competence to make science including coding you expand what could be a very simple add-on that gets you the extra marks. Or the job.

Think about integrating data science like interpreted / quasi MATLAB performance of GNU Octave: https://octave.org/download

Think about adding windows to projects like Octave: https://github.com/iambillmccann-zz/Octave-Data-Science/tree/master

Here's the thing: If you never plan to curate and maintain a source repository then if you donate working code back here you
achieve the objective employers and science fair judges want to see without taking on a responsibility. You score EXTRA POINTS on both business and science fair fronts.

You had a succession plan for your code, you didn't throw it in the memory hole (from George Orwell's 1984: https://gutenberg.net.au/ebooks01/0100021h.html).

This is an important thing to point out to potential bosses and science fair judges: I wrote a book about my horror story trying to extract the exact needs for 
graphics software for an underfunded science project; it details over 1000 source code tarballs, github repos, Source Forge folders, and on and on and how hard it is
to find the things you are searching for in barely working and rushed design software bundles. 


The book's called Untrapped Value.

It explains the serious problems, and wraps concepts about how widespread it is; 
and knowing about that will make you more sophisticated in your software skills.

https://leanpub.com/untrappedvalue101
https://leanpub.com/untrappedvalue
https://books.google.ca/books/about?id=Pqu1EAAAQBAJ

You have no idea, as I did, just how many well-meaning scientists and engineers offload source code that is full of 
things that are valuable if there had been a software reuse plan or at least better code documentation that would lead to 
rapid adoption and success.  And a lot of rushed crap that obscures the things you are trying to find. Even Google. 
Even NASA. Even Universities full of professors that sound like they know what they are doing.

My book shows you where to find value in software and how to mine it.

So you are looking at best a 40% savings in development time if you start from reusing someone else's code. Because it takes that long to see into enough
code to understand what's salvageable and what's not. 

If you want an introduction to the things you need to be successful you will find them in the yearly updates to Untrapped Value.

This repository is an attempt to weld the software reuse ideas into a specific functional library that goes into a very flexible and 
powerful immediate mode GUI system that will make it's way into moble systems and IoT devices that allow users to have a minimal interface
with a minimal compiler, memory, and processor environments. Everyone shares and helps each others in, say the 15% range and 
they pass it along down the years to people that spend much less time.

Every stop to fix corners of this REPOSITORY will pay dividends for your followers. 

IMAGINE THAT YOU AREN'T WRITING SOFTWARE FOR THE NEXT PRODUCT, BUT YOU ARE BUILDING A LEGACY OF CODE THAT
LASTS A THOUSAND YEARS. THAT'S REAL IMPACT.

Sincerely,

Github: DaemonDave GETTR: @DaemonDave TWITTER/X: @DaemonDave

## OBJECTVE:

To coalesce solo developers attempts at successful layouts, menu and widget pairings, and other learned templates
that should allow others to see the gist of the template and use their application .

This repository is at the kludge stage - the folders are lumped together under their orignal license layouts - licences
that reflect the orignal developers wishes. Once there is a major change to code or format that will meld
into what the next developers decide is the future licence. 
The autoconfiguration will work all over, but no inserts have been made into the originals. 

The idea being, git clone and poke around, get some things working, the way you want, and then come back to
a proposed redesign and then the storming, forming and norming begins.

*Nuklear community*

[![Build Status](https://travis-ci.org/vurtun/nuklear.svg)](https://travis-ci.org/vurtun/nuklear)

This is a minimal state immediate mode graphical user interface toolkit
written in ANSI C and licensed under public domain. It was designed as a simple
embeddable user interface for application and does not have any dependencies,
a default render backend or OS window and input handling but instead provides a very modular
library approach by using simple input state for input and draw
commands describing primitive shapes as output. So instead of providing a
layered library that tries to abstract over a number of platform and
render backends it only focuses on the actual UI.

## OUTLINE

THIS REPO MELDS MAKE SYSTEMS TOGETHER AND THE ORIGINAL MAKE FILES GET ADDED TO THE AUTOCONF/AUTOMAKE SYSTEM
YOU CAN MAKE 

CMAKE
AUTOCONF/AUTOMAKE/GMAKE
GMAKE

 WITH THE SAME CODE.
 
 The GNU Autoconf / Aclocal / Automake / Libtool configure system works with:
 
 autoreconf -fvi  
 - this recreates the configure files for your system.
 
 ./configure
 - this configures to compile in this directory. You can do like RTEMS or cmake and make a build/ directory
 but this makes for silly noncompartment. You can control anything within configure including building into anywhere
 and changing the prefix for all compiled byproducts.
 
 then when no errors or library missings are thrown then:
 
 make / make check / sudo make install
 
 make check - makes test programs which are not intended to be installed
 make - makes anything
 sudo make install - installs with superuser admin permission for /usr/local or other desired locations.
 
 cmake build system (this one I do the least with nor care to learn): https://cmake.org/
 
 normally:
 
 cmake . 
 
 or 
 
 cmake -G "Unix Makefiles" 
 
 (https://cmake.org/cmake/help/latest/generator/Unix%20Makefiles.html#generator:Unix%20Makefiles)
 and then:
 
 make
 
 NOTE: cmake is only in one folder - nuklear-glfw-vulkan which is what the original has. It would be a nice add-on by a 
 knowledgeable dev.

## CONFIGURE WITH CONFIG.H

 If you want to use the new autoconf and configure, then you can get the benefits of knowing what's available in
 system if you include
 
 #include "config.h"
 
 The autoconf/automake system automagically allows your code in subfolders to find that config.h in the base directory.
 This is important when you want to make complicated software and especially when you want kernel / kmod and other base functions.
 
 
## Features

- Immediate mode graphical user interface toolkit
- Single header library
- Written in C89 (ANSI C)
- Small codebase (~18kLOC)
- Focus on portability, efficiency and simplicity
- No dependencies (not even the standard library if not wanted)
- Fully skinnable and customizable
- Low memory footprint with total memory control if needed or wanted
- UTF-8 support
- No global or hidden state
- Customizable library modules (you can compile and use only what you need)
- Optional font baker and vertex buffer output
- [Documentation](https://cdn.statically.io/gh/vurtun/nuklear/master/doc/nuklear.html)

## Building

This library is self contained in one single header file and can be used either
in header only mode or in implementation mode. The header only mode is used
by default when included and allows including this header in other headers
and does not contain the actual implementation.

The implementation mode requires to define  the preprocessor macro
`NK_IMPLEMENTATION` in *one* .c/.cpp file before `#include`ing this file, e.g.:
```c
#define NK_IMPLEMENTATION
#include "nuklear.h"
```
IMPORTANT: Every time you include "nuklear.h" you have to define the same optional flags.
This is very important not doing it either leads to compiler errors or even worse stack corruptions.

## Gallery

![screenshot](https://cloud.githubusercontent.com/assets/8057201/11761525/ae06f0ca-a0c6-11e5-819d-5610b25f6ef4.gif)
![screen](https://cloud.githubusercontent.com/assets/8057201/13538240/acd96876-e249-11e5-9547-5ac0b19667a0.png)
![screen2](https://cloud.githubusercontent.com/assets/8057201/13538243/b04acd4c-e249-11e5-8fd2-ad7744a5b446.png)
![node](https://cloud.githubusercontent.com/assets/8057201/9976995/e81ac04a-5ef7-11e5-872b-acd54fbeee03.gif)
![skinning](https://cloud.githubusercontent.com/assets/8057201/15991632/76494854-30b8-11e6-9555-a69840d0d50b.png)
![gamepad](https://cloud.githubusercontent.com/assets/8057201/14902576/339926a8-0d9c-11e6-9fee-a8b73af04473.png)

## Example

```c
/* init gui state */
struct nk_context ctx;
nk_init_fixed(&ctx, calloc(1, MAX_MEMORY), MAX_MEMORY, &font);

enum {EASY, HARD};
static int op = EASY;
static float value = 0.6f;
static int i =  20;

if (nk_begin(&ctx, "Show", nk_rect(50, 50, 220, 220),
    NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_CLOSABLE)) {
    /* fixed widget pixel width */
    nk_layout_row_static(&ctx, 30, 80, 1);
    if (nk_button_label(&ctx, "button")) {
        /* event handling */
    }

    /* fixed widget window ratio width */
    nk_layout_row_dynamic(&ctx, 30, 2);
    if (nk_option_label(&ctx, "easy", op == EASY)) op = EASY;
    if (nk_option_label(&ctx, "hard", op == HARD)) op = HARD;

    /* custom widget pixel width */
    nk_layout_row_begin(&ctx, NK_STATIC, 30, 2);
    {
        nk_layout_row_push(&ctx, 50);
        nk_label(&ctx, "Volume:", NK_TEXT_LEFT);
        nk_layout_row_push(&ctx, 110);
        nk_slider_float(&ctx, 0, &value, 1.0f, 0.1f);
    }
    nk_layout_row_end(&ctx);
}
nk_end(&ctx);
```
![example](https://cloud.githubusercontent.com/assets/8057201/10187981/584ecd68-675c-11e5-897c-822ef534a876.png)

## Bindings
There are a number of nuklear bindings for different languges created by other authors.
I cannot atest for their quality since I am not necessarily proficient in either of these
languages. Furthermore there are no guarantee that all bindings will always be kept up to date:

- [Java](https://github.com/glegris/nuklear4j) by Guillaume Legris
- [D](https://github.com/Timu5/bindbc-nuklear) by Mateusz Muszyński
- [Golang](https://github.com/golang-ui/nuklear) by golang-ui@github.com
- [Rust](https://github.com/snuk182/nuklear-rust) by snuk182@github.com
- [Chicken](https://github.com/wasamasa/nuklear) by wasamasa@github.com
- [Nim](https://github.com/zacharycarter/nuklear-nim) by zacharycarter@github.com
- Lua
  - [LÖVE-Nuklear](https://github.com/keharriso/love-nuklear) by Kevin Harrison
  - [MoonNuklear](https://github.com/stetre/moonnuklear) by Stefano Trettel
- Python
  - [pyNuklear](https://github.com/billsix/pyNuklear) by William Emerison Six (ctypes-based wrapper)
  - [pynk](https://github.com/nathanrw/nuklear-cffi) by nathanrw@github.com (cffi binding)
- [CSharp/.NET](https://github.com/cartman300/NuklearDotNet) by cartman300@github.com

## Credits
Developed by Micha Mettke and every direct or indirect contributor to the GitHub.


Embeds `stb_texedit`, `stb_truetype` and `stb_rectpack` by Sean Barrett (public domain)
Embeds `ProggyClean.ttf` font by Tristan Grimmer (MIT license).


Big thank you to Omar Cornut (ocornut@github) for his [imgui](https://github.com/ocornut/imgui) library and
giving me the inspiration for this library, Casey Muratori for handmade hero
and his original immediate mode graphical user interface idea and Sean
Barrett for his amazing single header [libraries](https://github.com/nothings/stb) which restored my faith
in libraries and brought me to create some of my own. Finally Apoorva Joshi for his singe-header [file packer](http://apoorvaj.io/single-header-packer.html).

## License
```
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Micha Mettke
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------
```

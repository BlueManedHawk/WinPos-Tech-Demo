
# WinPos Tech Demo

**IMPORTANT:  I don't really plan to develop this any further.  It's a silly
little gimmick that I made because I noticed that I could.  I'm making the
source available in the hopes that this will be useful to others, _not_ for
collaboration.**

Video here:  https://youtu.be/mHYmOOpxsQE

This is a silly gimmick I made because I noticed that I could.  It uses a
window as a portal "past" the desktop.  I don't know what this is useful for,
but it might be useful for something.

I made this using C and SDL.  I used C because I like it and because developing
with any other language would take up too much storage on my Chromebook (except
maybe C++, but that language is a pile of trash anyway, and I sure as hell
ain't using it), and I used SDL because it seemed convenient (though honestly,
it may not have been the best choice…).

For some reason, I restricted myself to keeping all my lines at less than 80
characters.  I have genuinely no idea why I did this, since any modern text
has word wrap anyway.  I'm going to blame it on delirium from working on this
with a lack of sleep.

This isn't quite what I was aiming for originally—I would have much preferred
for the image used to be centered and scaled to fit the usable bounds of the
screen.  However, I'm happy with this as it is, so I think I can be done with
it.

Licensing details are in `LICENSE.md`.

To compile the program, make sure you have whatever your distro's SDL
development package is installed, then just compile `Main.c` with `clang-13
\`sdl2-config --cflags --libs\` Main.c`, and you should be good.  (You'll need
to run the file from the root directory of the repository—if you're getting an
error about the image failing to load, that's why.)

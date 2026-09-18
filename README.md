
Readme · MD
# Total Party Kill — Nintendo Switch port (Stencyl / OpenFL / Lime wrapper)
 
This is a native wrapper / loader that runs the original ARM64 Android build of *Total Party Kill* on Switch homebrew. It contains **no game code and no game assets** — it loads the game's own libraries and recreates, natively, the Android layer underneath them: bionic's C library, SDL, OpenGL ES, audio, input, and the Java/JNI side the Lime runtime expects.
 
## Install & run
 
You need files from the Total Party Kill Android v1.0.3 (arm64-v8a).
 
Put the `.nro` in any folder under `sdmc:/switch/` and place your game files next to it — the loader finds its folder at runtime, so the name is up to you:
 
```
sdmc:/switch/totalpartykill_nx
├── totalpartykill.nro
├── liblime.so
├── libApplicationMain.so
├── cursor.png                              <- optional
└── assets
```
 

```

 Launch via title override (hold R while starting an installed game).
 
Optionally drop a `cursor.png` (up to 64×64, transparency respected) in the same folder to replace the on-screen cursor with your own.
 
## Controls
 
The Android build of this game reads a touch screen and nothing else — its Stencyl input layer has no controller support on any code path. So the pad presses the game's own on-screen controls for you, at the positions where the game draws them.
 
| Input | Action |
|---|---|
| Touchscreen | Direct multi-touch — the game as designed (handheld) |
| Left stick ←/→, D-pad ←/→ | The on-screen left and right arrows |
| A | The jump button, bottom right |
| B | The sword button |
| Y | The swap button, bottom middle |
| + | Pause, top right |
| − | Back, top left |
| ZL + ZR | Toggle the on-screen cursor |
| Left stick (cursor up) | Move the cursor |
| A (cursor up) | Tap at the cursor |
 

 The cursor is off by default and works docked as well as handheld. It's modal: while it's up the button mappings go silent, because the left stick can't drive both the cursor and the arrows, and a held button would otherwise press whatever sits at its HUD position in a menu. That's also why A can be both jump and tap — the two never overlap.
 
## Remapping — `config.txt`
 
```
a = jump
b = sword
x = none
y = swap
l = none
r = none
zl = none
zr = none
plus = pause
minus = back
 
dpad_left = left
dpad_right = right
dpad_up = none
dpad_down = none
 
stick_left = left
stick_right = right
stick_up = none
stick_down = none
```
 
Assignable controls are `jump`, `sword`, `swap`, `left`, `right`, `pause`,
`back` and `none`. Several inputs may share one control — the D-pad and the
stick both drive the arrows by default. Delete the file to restore the
defaults.
 
## Building
 
Requires devkitPro with the `switch-dev` group plus these portlibs:
 
```
pacman -S switch-dev
pacman -S switch-mesa switch-libdrm_nouveau switch-sdl2 \
          switch-libpng switch-zlib
 
export DEVKITPRO=/opt/devkitpro
make                        # -> totalpartykill.nro
```

## Credits
 
The loader/shim infrastructure (`so_util`, the bionic shims, `jni_env`/`jni_classes`, `dl_bridge`) derives from the open-source Switch `.so`-loader lineage — Andy Nguyen and fgsfds, building on TheOfficialFloW's Vita/Switch loader tradition — with the Bloons Pop, Mulmash, PvZ Ultimate and MBHaxe Switch ports as references. The on-screen cursor (`nx_pointer`) comes from the Happy Wheels Switch port, with its cursor.png loader and GL overlay intact. All MIT-licensed.
 
`source/sdl_procs.h` is generated from the function list in SDL 2.0.12's `src/dynapi/SDL_dynapi_procs.h` (zlib licensed, © 1997-2020 Sam Lantinga) — function names only, no SDL code.
 
Thanks to everyone in that lineage for making this approach possible.

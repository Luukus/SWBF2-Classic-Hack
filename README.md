﻿# Star Wars Battlefront 2 Classic (2005) Hack - Steam Edition
#### An internal hack created for Star Wars Battlefront 2 Classic, Steam edition. If you like it please give it a star! ⭐ 

*Note: This is for educational purposes only. Do not use it for any commercial or illegal purposes. I will not be responsible for any unauthorized usage of this software.*

![img](https://github.com/Luukus/SWBF2-Classic-Hack/blob/ab49908afa953252c397649c67184e043de73b4f/img/in-game%20menu.png?raw=true)

## Features
- Aimbot
  - Configurable smoothing
  - Configurable FOV
  - Configurable lead scaling
- Team ESP
- Enemy ESP

*If there's interest I'll continue to add and build out more features, so please star to show it!*

## Run
You will need to either download the built DLL from this repository's releases, from UnknownCheats ([at this link](https://www.unknowncheats.me/forum/other-fps-games/598340-star-wars-battlefront-2-classic-2005-esp-aimbot-internal.html)), or build it yourself. Build details will be below.
After getting your hands on the DLL, you will need to run SWBF2 from Steam and then use an injector (like GuidedHacking's Injector) to inject it into your game. You can inject it 
at anytime. To uninject it press the `End` key.

Once injected the menu will appear. Use the up and down arrow keys to navigate the options, and the left and right arrow keys to enable/disable options. When you want to hide the menu
just press the `Insert` key.

***NOTE:** Make sure your injector is running the x86 version, not the x64 version*

## Build 
Building it is straightforward. Open it in Visual Studio (Im using VS2019 but any should work) and make sure you're building the Release x86 version. Building the solution will generate a DLL in the `build` folder within the root directory.

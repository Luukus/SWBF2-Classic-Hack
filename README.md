# Star Wars Battlefront 2 Classic (2005) Hack - Steam Edition
#### An internal hack created for Star Wars Battlefront 2 Classic, Steam edition. If you like it please give it a star! ⭐ 

*Note: This is for educational purposes only. Do not use it for any commercial or illegal purposes. I will not be responsible for any unauthorized usage of this software.*

![img](https://github.com/Luukus/SWBF2-Classic-Hack/blob/440280259a330621f5c1628a4a09a0837ca655fe/img/in-game%20menu.png)

## Features
- Aimbot
  - Configurable smoothing
  - Configurable FOV
  - Configurable lead scaling
- Team ESP
- Enemy ESP
- *If there's interest I'll continue to add and build out more features, some please star to show it!*

## Run
You will need to either download the built DLL from the Github Releases section, or build it yourself. Build details will be below.
After getting your hands on the DLL, you will need to run SWBF2 from Steam and then use an injector (like GuidedHacking's Injector) to inject it into your game. You can inject it 
at anytime. To uninject it press the `End` key.

Once injected the menu will appear. Use the up and down arrow keys to navigate the options, and the left and right arrow keys to enable/disable options. When you want to hide the menu
just press the `Insert` key.

***NOTE:** Make sure your injector is running the x86 version, not the x64 version*

## Build 
Building it is straightforward. Open it in Visual Studio (Im using VS2019 but any should work) and make sure you're building the Release x86 version. Building the solution will generate a DLL in the `build` folder within the root directory.
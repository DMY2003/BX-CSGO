!! THIS IS NOW OUTDATED !!

# BLAXED INTERNAL CS:GO CHEAT
[![C++](https://img.shields.io/badge/language-C%2B%2B-%23f34b7d.svg?style=plastic)](https://en.wikipedia.org/wiki/C%2B%2B) 
[![CS:GO](https://img.shields.io/badge/game-CS%3AGO-yellow.svg?style=plastic)](https://store.steampowered.com/app/730/CounterStrike_Global_Offensive/) 
[![License](https://img.shields.io/github/license/dmy2003/BX-CSGO.svg?style=plastic)](LICENSE)

Free open-source cross-platform cheat software for **Counter-Strike: Global Offensive** game. Designed as an internal cheat - [Dynamic-link library](https://en.wikipedia.org/wiki/Dynamic-link_library) (DLL) loadable into game process. Compatible with the Steam version of the game. Available for Windows systems.

<img src="https://raw.githubusercontent.com/DMY2003/BX-CSGO/master/Images/pic1.png">
<img src="https://raw.githubusercontent.com/DMY2003/BX-CSGO/master/Images/pic2.jpg">

## Getting started

### Prerequisites
Microsoft Visual Studio 2015 (or newer), platform toolset and Windows SDK 10 are required in order to compile BX-CSGO. If you don't have ones, you can download VS [here](https://visualstudio.microsoft.com/) (Windows SDK is installed during Visual Studio Setup).

### Downloading

There are two options of downloading the source code:

#### Without [git](https://git-scm.com)

Choose this option if you want pure source and you're not going to contribute to the repo. Download size ~600 kB.

To download source code this way [click here](https://github.com/DMY2003/BX-CSGO/archive/master.zip).

#### With [git](https://git-scm.com)

Choose this option if you're going to contribute to the repo or you want to use version control system. Download size ~4 MB. Git is required to step further, if not installed download it [here](https://git-scm.com).

Open git command prompt and enter following command:

    git clone --depth=1 https://github.com/DMY2003/BX-CSGO.git

`BLAXED` folder should have been successfully created, containing all the source files.

### Compiling from source

<details>

<summary>Windows</summary>

When you have equipped a copy of the source code, next step is opening **BLAXED.sln** in Microsoft Visual Studio 2015.

Then change build configuration to `Release | x86` and simply press **Build solution**.

If everything went right you should receive a DLL binary file.

</details>

### Loading / Injecting into game process

<details>

<summary>Windows</summary>

Open your favorite [DLL injector](https://en.wikipedia.org/wiki/DLL_injection) and just inject the DLL file into `csgo.exe` process.

When injected, menu is openable under `INSERT` key.

</details>

### Further optimizations
If your CPU supports AVX / AVX2 / AVX-512 instruction set, you can enable it in project settings. This should result in more performant code, optimized for your CPU. Currently SSE2 instructions are selected in project settings.

## FAQ

### How do I open menu?
Press <kbd>INSERT</kbd> while focused on CS:GO window.

This cheat was used on **Blaxed** and **Luminus** Communities between *2016-2017*.

## See also
*   [Osiris](https://github.com/danielkrupinski/Osiris)
*   [Gamerfood_CSGO](https://github.com/A5-/Gamerfood_CSGO)

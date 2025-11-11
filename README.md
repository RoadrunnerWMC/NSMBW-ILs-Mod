# NSMBW ILs Mod

A New Super Mario Bros. Wii mod that adds quality-of-life improvements for speedrunning individual levels (ILs).

This mod is legal for [the speedrun.com IL leaderboards](https://www.speedrun.com/nsmbw).

## Features

- Instant Level Reset
  - Controls: Press Minus (-)
- Attempt Counter
  - Increases by 1 with every level entry or reset
  - Resets when entering a different level
- Precise Time
  - Becomes visible after level clear
- Star Coin Tracker
  - Shows star coins collected during this run-through of the level
  - Becomes visible after level clear

<p align="center"><img src="screenshot.png?raw=true" width="512px" alt="Screenshot of the mod"/></p>

## Setup

- [Download the zip file for the latest release](https://github.com/RoadrunnerWMC/NSMBW-ILs-Mod/releases/latest) (for example, "nsmbw_ils_mod_v1.0.0.zip"). This mod is a Riivolution patch, so install the Homebrew Channel and Riivolution first, and then install the mod to your SD card. There are many tutorials on how to set up Riivolution patches online.
- This mod works best with 4:3 aspect ratio, so please put your Wii or Wii U into that mode.

## Building

1. Install [Python 3](https://www.python.org/), [Ninja](https://ninja-build.org/), and [Wine](https://www.winehq.org/) (if not on Windows). Also download a build of [Kamek](https://github.com/Treeki/Kamek), and the right version of CodeWarrior (see the Kamek readme for more details).
2. In the `code` folder, run `python3 configure.py --kamek=<path to Kamek binary> --kstdlib=<path to k_stdlib folder> --cw=<path to mwcceppc.exe>`.
3. In the same folder, run `ninja`. This will build the code files and put them in `code/bin`.
4. Copy them into `sd/nsmbw_ils_mod/Code`. Alternatively, set up symlinks from the Code folder to the bin folder, so it'll always reflect the latest versions whenever you rebuild.

The `sd` folder is now a working Riivolution mod you can test in Dolphin or on hardware.

To create a release:

1. (Optional) Install [advzip](https://github.com/amadvance/advancecomp). If available, this is used to perform better compression on the zip.
2. Run `./create_release.sh`.
3. Rename the newly created `NEW_RELEASE.zip` to a more proper filename, like `nsmbw_ils_mod_1.0.0.zip`.

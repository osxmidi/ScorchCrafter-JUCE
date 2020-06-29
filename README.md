# ScorchCrafter-Juce


ScorchCrafter Amp Sim for JUCE (short demo at https://soundcloud.com/aaabii/test)

Quad core (or more) systems recommended.

Does not include cabinets.

Some possible IR loaders https://github.com/osxmidi/KlangFalter and http://lsp-plug.in/ (Impulsantworten)

9 Amp Channels with various tube stage configurations.

Channels 1, 2, 3
Uses 2 simulated tube overdrive passes
Clean and low gain / crunch

Channels 4, 5, 6
Uses 3 simulated tube overdrive passes
Mid-range gain, moderate overdrive

Channels 7, 8, 9
Uses 4 simulated tube overdrive passes
High-gain overdrive

-------

There is also ScorchCrafter C120 at the ScorchC120-JUCE branch https://github.com/osxmidi/ScorchCrafter-JUCE/tree/ScorchC120-JUCE

--------

To make

Rename the unzipped JUCE folder to JUCE and move it to the home folder

Unzip this repository/clone inside the JUCE folder

Some libraries need to be preinstalled

sudo apt-get -y install git pkg-config libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev libcurl4-gnutls-dev

(also webkit2gtk-4.0 if using webkit)

cd into Builds/LinuxMakefile and run make CONFIG=Release

--------

The lv2 version needs JUCE lv2 from the lv2 branch at https://github.com/lv2-porting-project/JUCE

sudo apt-get install lv2-dev

Copy/replace the JUCE modules folder to the JuceLibraryCode folder

Unzip the lv2.zip file in the Builds/LinuxMakefile folder and copy the Makefile to the Builds/LinuxMakefile folder

cd into the Builds/LinuxMakefile folder

make CONFIG=Release

cd build

copy lv2_ttl_generator and lvmake and makelv2 (might need a chmod +x to make them executable) from the unzipped lv2.zip to the build folder

./makelv2



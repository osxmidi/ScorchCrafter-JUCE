# ScorchC120-Juce


ScorchC120 Amp Sim for JUCE.

Dual Core (or more) systems recommended.

Does not include cabinets.

Some possible IR loaders https://github.com/osxmidi/KlangFalter and http://lsp-plug.in/ (Impulsantworten)

----------

To make Linux version

Rename the unzipped JUCE folder to JUCE and move it to the home folder

Unzip this repository/clone inside the JUCE folder

Some libraries need to be installed

sudo apt-get -y install git pkg-config libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev libcurl4-gnutls-dev

cd into Builds/LinuxMakefile and run Makelibs (might require a chmod +x Makelibs)

cd into Builds/LinuxMakefile and run make CONFIG=Release

----------

The lv2 version needs JUCE lv2 from the lv2 branch at https://github.com/lv2-porting-project/JUCE

sudo apt-get install lv2-dev

Copy/replace the JUCE modules folder to the JuceLibraryCode folder

Unzip the lv2.zip file in the Builds/LinuxMakefile folder and copy the Makefile to the Builds/LinuxMakefile folder

cd into the Builds/LinuxMakefile folder

make CONFIG=Release

cd build

copy lv2_ttl_generator and lvmake and makelv2 (might need a chmod +x to make them executable) from the unzipped lv2.zip to the build folder

./makelv2



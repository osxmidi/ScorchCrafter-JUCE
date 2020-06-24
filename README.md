# ScorchC120-Juce


ScorchC120 Amp Sim for JUCE.

Dual Core (or more) systems recommended.

Does not include cabinets.

Some possible IR loaders https://github.com/osxmidi/KlangFalter and http://lsp-plug.in/ (Impulsantworten)

----------

To make Linux version

Some libraries need to be installed

sudo apt-get -y install git pkg-config libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev libcurl4-gnutls-dev

cd into common/eq and run ./makelib (might require a chmod +x makelib)

cd into common/dist and run ./makelib

cd into common and run ./makelib

cd into Builds/LinuxMakefile and run make CONFIG=Release


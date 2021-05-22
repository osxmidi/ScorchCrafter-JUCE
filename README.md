# ScorchC120-Juce


ScorchC120 Amp Sim for JUCE (short demo at https://soundcloud.com/aaabii/test)

Dual Core (or more) systems recommended.

Does not include cabinets.

Some possible IR loaders https://github.com/osxmidi/KlangFalter and http://lsp-plug.in/ (Impulsantworten)

----------

Linux make instructions

Rename the unzipped JUCE folder to JUCE and move it to the home folder

Unzip this repository/clone inside the JUCE folder

To make the Projucer, change into ~/JUCE/extras/Projucer/Builds/LinuxMakefile.
Edit the Makefile and add "-DJUCER_ENABLE_GPL_MODE=1" to both the JUCE_CPPFLAGS lines.
Then enter into the Terminal, 
make CONFIG=Release

Run the Projucer and load the ScorchC120 jucer file in the unzipped ScorchCrafter-JUCE-ScorchC120-JUCE folder and save the project (disable JUCE_VST3_CAN_REPLACE_VST2 in the juce_audio_plugin_client module options before saving).

---------
 
Some libraries need to be preinstalled

sudo apt-get -y install git pkg-config libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev libcurl4-gnutls-dev

(also webkit2gtk-4.0 if using webkit)

cd into Builds/LinuxMakefile and run make CONFIG=Release

vst3 is installed into ~/.vst3

--------

The lv2 version needs JUCE lv2 from the lv2 branch at https://github.com/lv2-porting-project/JUCE

Install the lv2 development files.
sudo apt-get install lv2-dev or sudo pacman -Sy lv2 for Manjaro

Make the Projucer as above and save the project as a vst2 (change from vst3 to vst2 in the Projucer settings).

Unzip the lv2.zip file in the Builds/LinuxMakefile folder and copy the Makefile to the Builds/LinuxMakefile folder

cd into the Builds/LinuxMakefile folder

make CONFIG=Release

then

cd build

copy lv2_ttl_generator and lvmake and makelv2 (might need a chmod +x to make them executable) from the unzipped lv2.zip file to the build folder

./makelv2

----------

The lv2 version needs JUCE lv2 from the lv2 branch at https://github.com/lv2-porting-project/JUCE

Automation needs a few mods in the /JUCE/modules/juce_audio_plugin_client/LV2/juce_LV2_Wrapper.cpp file (before copying the /JUCE/modules folder to the JuceLibraryCode folder)

search for ~JuceLv2Wrapper

Insert the following just above the ~JuceLv2Wrapper () line

```

    void setParameter (int32 index, float value)
    {
    if (auto* param = filter->getParameters()[index])	
    {
    param->setValue(value);
    param->sendValueChangedMessageToListeners (value);    
	}
    }  
    
then search for filter->setParameter and replace it with setParameter (leave the rest of the line as it is)

```


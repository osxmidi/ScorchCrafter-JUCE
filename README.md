# ScorchC120-Juce


ScorchC120 Amp Sim for JUCE (short demo at https://soundcloud.com/aaabii/test)

Dual Core (or more) systems recommended.

Does not include cabinets.

Some possible IR loaders https://github.com/osxmidi/KlangFalter and http://lsp-plug.in/ (Impulsantworten)

----------

To make

Rename the unzipped JUCE folder to JUCE and move it to the home folder

Unzip this repository/clone inside the JUCE folder

To make the Projucer change into ~/JUCE/extras/Projucer/Builds/LinuxMakefile then make CONFIG=Release

Run the Projucer and load the jucer file in the unzipped folder and save the project

Some libraries need to be preinstalled

sudo apt-get -y install git pkg-config libfreetype6-dev libx11-dev libxinerama-dev libxrandr-dev libxcursor-dev mesa-common-dev libasound2-dev freeglut3-dev libxcomposite-dev libcurl4-gnutls-dev

(also webkit2gtk-4.0 if using webkit)

cd into Builds/LinuxMakefile and run Makelibs (might require a chmod +x Makelibs)

cd into Builds/LinuxMakefile and run make CONFIG=Release

----------

The lv2 version needs JUCE lv2 from the lv2 branch at https://github.com/lv2-porting-project/JUCE

Automation needs a few mods in the /JUCE/modules/juce_audio_plugin_client/LV2/juce_LV2_Wrapper.cpp file (before copying the /JUCE/modules folder to the JuceLibraryCode folder)

search for ~JuceLv2Wrapper

Insert the following just above the ~JuceLv2Wrapper () line

    void setParameter (int32 index, float value)
    {
    if (auto* param = filter->getParameters()[index])	
    {
    param->setValue(value);
    param->sendValueChangedMessageToListeners (value);    
	}
    }  
    
then search for filter->setParameter and replace it with setParameter (leave the rest of the line as it is)

...

sudo apt-get install lv2-dev

Make the Projucer and save the project as above.

Add

#ifndef  JucePlugin_MaxNumInputChannels
 #define JucePlugin_MaxNumInputChannels    2
#endif
#ifndef  JucePlugin_MaxNumOutputChannels
 #define JucePlugin_MaxNumOutputChannels   2
#endif 

to the bottom of the JucePluginDefines.h file in the JuceLibraryCode folder

Copy/replace the JUCE modules folder to the JuceLibraryCode folder

Unzip the lv2.zip file in the Builds/LinuxMakefile folder and copy the Makefile to the Builds/LinuxMakefile folder

cd into the Builds/LinuxMakefile folder

make CONFIG=Release

cd build

copy lv2_ttl_generator and lvmake and makelv2 (might need a chmod +x to make them executable) from the unzipped lv2.zip to the build folder

./makelv2



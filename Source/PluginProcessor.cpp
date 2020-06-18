
#include "PluginProcessor.h"
#include "PluginEditor.h"

ScorchAmpAudioProcessor::ScorchAmpAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
	    mState(*this, nullptr, Identifier("ScorchCrafter"),
		{
			  std::make_unique<AudioParameterFloat>("master",
													"Master",
										             0.0, 
													 1.0,
													 0.5
													 ),
			  std::make_unique<AudioParameterFloat>("gain",
													"Gain",
								            		 0.0, 
													 1.0,
													 0.5
													 ),
			  std::make_unique<AudioParameterFloat>("contour",
													"Contour",
					              					 0.0, 
													 1.0,
													 0.5
													),
			  std::make_unique<AudioParameterFloat>("presence",
													"Presence",
									            	 0.0, 
													 1.0,
													 0.5
													),
			  std::make_unique<AudioParameterFloat>("treble",
													"Treble",
								            		 0.0, 
													 1.0,
													 0.5
													),
			 std::make_unique<AudioParameterFloat>("mid",
													 "Mid",
									        		 0.0, 
													 1.0,
													 0.5
												     ),
			 std::make_unique<AudioParameterFloat>("bass",
													 "Bass",
													 0.0, 
													 1.0,
													 0.5
												     ),
			 std::make_unique<AudioParameterFloat>("noisegate",
													 "NoiseGate",
										             0.0, 
													 1.0,
													 0.6
                                                     ),                                                    
			 std::make_unique<AudioParameterFloat>("ampstages",
													 "AmpStages",													 
										             0.0, 
													 10.0,
													 0.5
                                                     ), 
			 std::make_unique<AudioParameterBool>("noiseon",
													 "NoiseOn",
													 false),                                                           
			 std::make_unique<AudioParameterBool>("boost",
													 "Boost",
													 false),                                                                                                           
			 std::make_unique<AudioParameterBool>("bright",
													 "Bright",
													 false)                                                     
		}),
mScorch()

#endif
{ 
	
	mScorch = new Scorch();
	
    mState.addParameterListener ("master", this);
    mState.addParameterListener ("gain", this);
    mState.addParameterListener ("contour", this);
    mState.addParameterListener ("presence", this);
    mState.addParameterListener ("treble", this);
    mState.addParameterListener ("mid", this);
    mState.addParameterListener ("bass", this);
    mState.addParameterListener ("noisegate", this);
    mState.addParameterListener ("bright", this);
    mState.addParameterListener ("noiseon", this);
    mState.addParameterListener ("boost", this); 
    mState.addParameterListener ("ampstages", this);
 
    mScorch->loadProgram(0); 
 
    float setpar;	
	
    setpar = mScorch->getParameterValue(2); 	
    mState.getParameter("master")->setValue(setpar);	
 
    setpar = mScorch->getParameterValue(4); 	
    mState.getParameter("gain")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(11); 	
    mState.getParameter("contour")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(10); 	
    mState.getParameter("presence")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(9); 	
    mState.getParameter("treble")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(8); 	
    mState.getParameter("mid")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(7); 	
    mState.getParameter("bass")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(13); 	
    mState.getParameter("noisegate")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(12); 	
    mState.getParameter("noiseon")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(5); 	
    mState.getParameter("bright")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(3); 	
    mState.getParameter("boost")->setValue(setpar);
 
    setpar = mScorch->getParameterValue(6); 	
    mState.getParameter("ampstages")->setValue(setpar);
}

ScorchAmpAudioProcessor::~ScorchAmpAudioProcessor()
{
	delete mScorch;
}

const String ScorchAmpAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ScorchAmpAudioProcessor::acceptsMidi() const
{
    #if JucePlugin_WantsMidiInput
    return true;
    #else
    return false;
    #endif
}

bool ScorchAmpAudioProcessor::producesMidi() const
{
    #if JucePlugin_ProducesMidiOutput
    return true;
    #else
    return false;
    #endif
}

bool ScorchAmpAudioProcessor::isMidiEffect() const
{
    #if JucePlugin_IsMidiEffect
    return true;
    #else
    return false;
    #endif
}

double ScorchAmpAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ScorchAmpAudioProcessor::getNumPrograms()
{	
    return mScorch->numprograms; 
}

int ScorchAmpAudioProcessor::getCurrentProgram()
{	
    return mScorch->getProgramx(); 
}

void ScorchAmpAudioProcessor::setCurrentProgram (int index)
{
    if(index < 0)
    return;
    	
	mScorch->loadProgram(index); 
	
    float setpar;	
	
    setpar = mScorch->getParameterValue(2); 	
    mState.getParameter("master")->setValue(setpar);	 
    sendParamChangeMessageToListeners(0, setpar);
 
    setpar = mScorch->getParameterValue(4); 	
    mState.getParameter("gain")->setValue(setpar);
    sendParamChangeMessageToListeners(1, setpar);  
  
    setpar = mScorch->getParameterValue(11); 	
    mState.getParameter("contour")->setValue(setpar);
    sendParamChangeMessageToListeners(2, setpar);
 
    setpar = mScorch->getParameterValue(10); 	
    mState.getParameter("presence")->setValue(setpar);
    sendParamChangeMessageToListeners(3, setpar);
 
    setpar = mScorch->getParameterValue(9); 	
    mState.getParameter("treble")->setValue(setpar);
    sendParamChangeMessageToListeners(4, setpar);
 
    setpar = mScorch->getParameterValue(8); 	
    mState.getParameter("mid")->setValue(setpar);
    sendParamChangeMessageToListeners(5, setpar);
 
    setpar = mScorch->getParameterValue(7); 	
    mState.getParameter("bass")->setValue(setpar);
    sendParamChangeMessageToListeners(6, setpar);
 
    setpar = mScorch->getParameterValue(13); 	
    mState.getParameter("noisegate")->setValue(setpar);
    sendParamChangeMessageToListeners(7, setpar);	
 
    setpar = mScorch->getParameterValue(12); 	
    mState.getParameter("noiseon")->setValue(setpar);
    sendParamChangeMessageToListeners(9, setpar);
 
    setpar = mScorch->getParameterValue(5); 	
    mState.getParameter("bright")->setValue(setpar);
    sendParamChangeMessageToListeners(11, setpar);
 
    setpar = mScorch->getParameterValue(3); 	
    mState.getParameter("boost")->setValue(setpar);
    sendParamChangeMessageToListeners(10, setpar); 
 
    setpar = mScorch->getParameterValue(6); 	
    mState.getParameter("ampstages")->setValue(setpar);
    sendParamChangeMessageToListeners(8, setpar);	
}

const String ScorchAmpAudioProcessor::getProgramName (int index)
{
	char name[2048];
	
	memset(name, 0, 2048);
	
    mScorch->getProgramNameIndexedx (0, index, name); 

    retval = name;
	
    return retval;
}

void ScorchAmpAudioProcessor::changeProgramName (int index, const String& newName)
{
}

void ScorchAmpAudioProcessor::parameterChanged(const String& parmID, float newValue)
{
	float newValue2;
		
    if (parmID == "master")
    {
        mScorch->setParameterValue(2, newValue);
    }
    else if (parmID == "gain")
    {
        mScorch->setParameterValue(4, newValue);
    }
    else if (parmID == "contour")
    {
        mScorch->setParameterValue(11, newValue);
    }
    else if (parmID == "presence")
    {
        mScorch->setParameterValue(10, newValue);
    }
    else if (parmID == "treble")
    {
        mScorch->setParameterValue(9, newValue);
    }    
    else if (parmID == "mid")
    {
        mScorch->setParameterValue(8, newValue);
    } 
    else if (parmID == "bass")
    {
        mScorch->setParameterValue(7, newValue);
    }
    else if (parmID == "noisegate")
    {
        mScorch->setParameterValue(13, newValue);
    }     
    else if (parmID == "noiseon")
    {
        mScorch->setParameterValue(12, newValue);
    }               
    else if (parmID == "bright")
    {
        mScorch->setParameterValue(5, newValue);
    }           
    else if (parmID == "boost")
    {
        mScorch->setParameterValue(3, newValue);
    }           
    else if (parmID == "ampstages")
    {
		newValue2 = newValue / 10.0;
        mScorch->setParameterValue(6, newValue2);
    }  
                                   
}

void ScorchAmpAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{	
	mScorch->sampleRateChanged(sampleRate);
}

void ScorchAmpAudioProcessor::releaseResources()
{

}

#ifndef JucePlugin_PreferredChannelConfigurations
bool ScorchAmpAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void ScorchAmpAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
        
    const float *inBuffers[2] = { buffer.getReadPointer(0), buffer.getReadPointer(1) };
    float* outBuffers[2] = { buffer.getWritePointer(0), buffer.getWritePointer(1) };

     mScorch->run(inBuffers, outBuffers, buffer.getNumSamples());
}

bool ScorchAmpAudioProcessor::hasEditor() const
{
    return true; 
}

AudioProcessorEditor* ScorchAmpAudioProcessor::createEditor()
{
    return new ScorchAmpAudioProcessorEditor (*this);
}

void ScorchAmpAudioProcessor::getStateInformation (MemoryBlock& destData)
{
	auto state = mState.copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void ScorchAmpAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
	std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

	if (xmlState.get() != nullptr)
		if (xmlState->hasTagName(mState.state.getType()))
			mState.replaceState(ValueTree::fromXml(*xmlState));
}

AudioProcessorValueTreeState & ScorchAmpAudioProcessor::getState()
{
	return mState;
}

AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	ScorchAmpAudioProcessor *sp;
	sp = new ScorchAmpAudioProcessor();
	
	if(sp->mScorch == nullptr)
        {
	// printf("ScorchInitErr\n");
        return nullptr;
        }
	
 //   return new ScorchAmpAudioProcessor();
}

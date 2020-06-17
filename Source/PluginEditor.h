
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::ButtonAttachment ButtonAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboBoxAttachment;

//==============================================================================

class ScorchAmpAudioProcessorEditor  : public AudioProcessorEditor, public Button::Listener, public Slider::Listener, public ComboBox::Listener
{
public:
    ScorchAmpAudioProcessorEditor (ScorchAmpAudioProcessor&);
    ~ScorchAmpAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	void initialiseGUI();	
	void updateToggleState (Button* button, String name);
	void updateToggleState2 (Button* button, String name);
	void updateToggleState3 (Button* button, String name);
	
	ToggleButton brightButton { "Bright" };	
	std::unique_ptr<ButtonAttachment> brightAttachment;
	  
	ToggleButton boostButton { "Boost" };	
    std::unique_ptr<ButtonAttachment> boostAttachment;
	  
	ToggleButton noiseonButton { "NoiseOn" };
	std::unique_ptr<ButtonAttachment> noiseonAttachment;	  	  
	
	void buttonClicked (Button* button) override;
	   
    void AmpStageMenuChanged();	
      
    std::unique_ptr<ComboBoxAttachment > AmpStageMenuAttachment;   
      	  
    void comboBoxChanged (ComboBox* comboBoxThatHasChanged) override;

    ScorchAmpAudioProcessor& processor;
	AudioProcessorValueTreeState& mState;

	const int mSliderWidth = 80;
	const int mSliderHeight = 80;
	const int mTextBoxWidth = mSliderWidth;
	const int mTextBoxHeight = 14;

	typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

	Slider ScorchMasterSlider;
	std::unique_ptr<SliderAttachment> ScorchMasterSliderAttachment;

	Slider ScorchGainSlider;
	std::unique_ptr<SliderAttachment> ScorchGainSliderAttachment;

	Slider ScorchContourSlider;
	std::unique_ptr<SliderAttachment> ScorchContourSliderAttachment;
	
	Slider ScorchPresenceSlider;
	std::unique_ptr<SliderAttachment> ScorchPresenceSliderAttachment;

	Slider ScorchTrebleSlider;
	std::unique_ptr<SliderAttachment> ScorchTrebleSliderAttachment;
	
	Slider ScorchMidSlider;
	std::unique_ptr<SliderAttachment> ScorchMidSliderAttachment;
	
	Slider ScorchBassSlider;
	std::unique_ptr<SliderAttachment> ScorchBassSliderAttachment;

	Slider ScorchNoiseGateSlider;
	std::unique_ptr<SliderAttachment> ScorchNoiseGateSliderAttachment;
	
    void sliderValueChanged (Slider* slider) override;
    
    ComboBox AmpStageMenu;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScorchAmpAudioProcessorEditor)
};

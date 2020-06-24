

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ScorchAmpAudioProcessorEditor::ScorchAmpAudioProcessorEditor (ScorchAmpAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), mState(p.getState())
{
    setSize (480, 360);
	initialiseGUI();
}

ScorchAmpAudioProcessorEditor::~ScorchAmpAudioProcessorEditor()
{

}

void ScorchAmpAudioProcessorEditor::sliderValueChanged (Slider* slider)
{
	
	if(slider == &ScorchMasterSlider)
	{
	      mState.getParameter("master")->beginChangeGesture();
          mState.getParameter("master")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("master")->endChangeGesture();
	}
	else if(slider == &ScorchGainSlider)
	{
	      mState.getParameter("gain")->beginChangeGesture();
          mState.getParameter("gain")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("gain")->endChangeGesture();
	}
	else if(slider == &ScorchContourSlider)
	{
	      mState.getParameter("contour")->beginChangeGesture();
          mState.getParameter("contour")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("contour")->endChangeGesture();
	}		
	else if(slider == &ScorchPresenceSlider)
	{
	      mState.getParameter("presence")->beginChangeGesture();
          mState.getParameter("presence")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("presence")->endChangeGesture();
	}	
	else if(slider == &ScorchTrebleSlider)
	{
	      mState.getParameter("treble")->beginChangeGesture();
          mState.getParameter("treble")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("treble")->endChangeGesture();
	}	
	else if(slider == &ScorchMidSlider)
	{
	      mState.getParameter("mid")->beginChangeGesture();
          mState.getParameter("mid")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("mid")->endChangeGesture();
	}	
	else if(slider == &ScorchBassSlider)
	{
	      mState.getParameter("bass")->beginChangeGesture();
          mState.getParameter("bass")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("bass")->endChangeGesture();
	}	

}

void ScorchAmpAudioProcessorEditor::comboBoxChanged (ComboBox* comboBoxThatHasChanged)
{

}

void ScorchAmpAudioProcessorEditor::buttonClicked (Button* button)
{

}	   

void ScorchAmpAudioProcessorEditor::updateToggleState (Button* button, String name)
{
		  
	if (button == &brightButton) 
    {
	auto state = button->getToggleState();
		
	mState.getParameter("bright")->setValue(state);	
	mState.getParameter("bright")->beginChangeGesture();
    mState.getParameter("bright")->setValueNotifyingHost (button->getToggleState() ? 1.0f : 0.0f);
    mState.getParameter("bright")->endChangeGesture();	  
    }		

}
    
void ScorchAmpAudioProcessorEditor::updateToggleState2 (Button* button, String name)
{

    if (button == &boostButton) 
	{
	auto state = button->getToggleState();
		
    mState.getParameter("boost")->setValue(state);	
    mState.getParameter("boost")->beginChangeGesture();
    mState.getParameter("boost")->setValueNotifyingHost (button->getToggleState() ? 1.0f : 0.0f);
    mState.getParameter("boost")->endChangeGesture();	  
	}		

}   
    
void ScorchAmpAudioProcessorEditor::updateToggleState3 (Button* button, String name)
    {
	
	if (button == &heavyButton) 
	{				
	auto state = button->getToggleState();
			
	mState.getParameter("heavy")->setValue(state);	
	mState.getParameter("heavy")->beginChangeGesture();
    mState.getParameter("heavy")->setValueNotifyingHost (button->getToggleState() ? 1.0f : 0.0f);
    mState.getParameter("heavy")->endChangeGesture();	  
 
    if(mState.getParameter("clean")->getValue()	== 1.0)
    {  
	cleanButton.setToggleState (false, dontSendNotification); 
	
    mState.getParameter("clean")->setValue(0.0);	
    mState.getParameter("clean")->beginChangeGesture();
    mState.getParameter("clean")->setValueNotifyingHost (0.0f);
    mState.getParameter("clean")->endChangeGesture();	           			
	 }
	
    }		

} 

void ScorchAmpAudioProcessorEditor::updateToggleState4 (Button* button, String name)
{

    if (button == &cleanButton) 
	{			
	auto state = button->getToggleState();
		
	mState.getParameter("clean")->setValue(state);	
	mState.getParameter("clean")->beginChangeGesture();
    mState.getParameter("clean")->setValueNotifyingHost (button->getToggleState() ? 1.0f : 0.0f);
    mState.getParameter("clean")->endChangeGesture();	  
    
    if(mState.getParameter("heavy")->getValue()	== 1.0)
    {    	
    heavyButton.setToggleState (false, dontSendNotification);

    mState.getParameter("heavy")->setValue(0.0);	
    mState.getParameter("heavy")->beginChangeGesture();
    mState.getParameter("heavy")->setValueNotifyingHost (0.0f);
    mState.getParameter("heavy")->endChangeGesture();	
    }      
      
	}		

}       
   
void ScorchAmpAudioProcessorEditor::paint (Graphics& g)
{

Rectangle<int> rect = {125, 0, 300, 80};

    g.fillAll (Colours::darkblue.brighter());
    g.setColour (Colours::black);     
    g.setFont (40.0f);    
    g.drawText ("Scorch   C120", rect, Justification::left, true);   

}

void ScorchAmpAudioProcessorEditor::resized()
{
	
	auto sliderLeft = 50;
	
    ScorchMasterSlider.setBounds (sliderLeft, 80, 100, 100);
    ScorchGainSlider.setBounds (sliderLeft + 100, 80, 100, 100);
    ScorchContourSlider.setBounds (sliderLeft + 200, 80, 100, 100);   
    ScorchPresenceSlider.setBounds (sliderLeft + 300, 80, 100 , 100);
    ScorchTrebleSlider.setBounds (sliderLeft + 50, 180, 100 , 100);
    ScorchMidSlider.setBounds (sliderLeft + 150, 180 , 100 , 100);
    ScorchBassSlider.setBounds (sliderLeft + 250, 180 , 100 , 100);             
                
    brightButton.setBounds(sliderLeft + 40, 300, 70, 20);
    boostButton.setBounds(sliderLeft + 120, 300, 70, 20);
    heavyButton.setBounds(sliderLeft + 200, 300, 70, 20);
    cleanButton.setBounds(sliderLeft + 280, 300, 70, 20);
       
}

void ScorchAmpAudioProcessorEditor::initialiseGUI()
{
    addAndMakeVisible(ScorchMasterSlider);
	ScorchMasterSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchMasterSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchMasterSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchMasterSlider.setTextValueSuffix(" Master");
	ScorchMasterSliderAttachment.reset(new SliderAttachment(mState, "master", ScorchMasterSlider));	
	ScorchMasterSlider.addListener(this);

    addAndMakeVisible(ScorchGainSlider);
	ScorchGainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchGainSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchGainSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchGainSlider.setTextValueSuffix(" Gain");	
	ScorchGainSliderAttachment.reset(new SliderAttachment(mState, "gain", ScorchGainSlider));
	ScorchGainSlider.addListener(this);
		
	addAndMakeVisible(ScorchContourSlider);
	ScorchContourSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchContourSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchContourSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchContourSlider.setTextValueSuffix(" Contour");	
	ScorchContourSliderAttachment.reset(new SliderAttachment(mState, "contour", ScorchContourSlider));
	ScorchContourSlider.addListener(this);

    addAndMakeVisible(ScorchPresenceSlider);
	ScorchPresenceSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchPresenceSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchPresenceSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchPresenceSlider.setTextValueSuffix(" Presence");	
	ScorchPresenceSliderAttachment.reset(new SliderAttachment(mState, "presence", ScorchPresenceSlider));
	ScorchPresenceSlider.addListener(this);

	addAndMakeVisible(ScorchTrebleSlider);	
	ScorchTrebleSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchTrebleSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchTrebleSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchTrebleSlider.setTextValueSuffix(" Treble");
	ScorchTrebleSliderAttachment.reset(new SliderAttachment(mState, "treble", ScorchTrebleSlider));
	ScorchTrebleSlider.addListener(this);

	addAndMakeVisible(ScorchMidSlider);	
	ScorchMidSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchMidSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchMidSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchMidSlider.setTextValueSuffix(" Mid");
	ScorchMidSliderAttachment.reset(new SliderAttachment(mState, "mid", ScorchMidSlider));
	ScorchMidSlider.addListener(this);

	addAndMakeVisible(ScorchBassSlider);
	ScorchBassSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchBassSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchBassSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchBassSlider.setTextValueSuffix(" Bass");
	ScorchBassSliderAttachment.reset(new SliderAttachment(mState, "bass", ScorchBassSlider));
	ScorchBassSlider.addListener(this);

	addAndMakeVisible (brightButton);
	brightAttachment.reset (new ButtonAttachment (mState, "bright", brightButton));
	brightButton.onClick = [this] { updateToggleState (&brightButton, "Bright"); };		
	brightButton.addListener (this);

	addAndMakeVisible (boostButton);
	boostAttachment.reset (new ButtonAttachment (mState, "boost", boostButton));	
	boostButton.onClick = [this] { updateToggleState2 (&boostButton, "Boost"); };		
	boostButton.addListener (this);

	addAndMakeVisible (heavyButton);	
    heavyAttachment.reset (new ButtonAttachment (mState, "heavy", heavyButton));	
	heavyButton.onClick = [this] { updateToggleState3 (&heavyButton, "Heavy"); };		
	heavyButton.addListener (this);
	
	addAndMakeVisible (cleanButton);	
    cleanAttachment.reset (new ButtonAttachment (mState, "clean", cleanButton));	
	cleanButton.onClick = [this] { updateToggleState4 (&cleanButton, "Clean"); };		
	cleanButton.addListener (this);	
	
}

   



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
	else if(slider == &ScorchNoiseGateSlider)
	{
	      mState.getParameter("noisegate")->beginChangeGesture();
          mState.getParameter("noisegate")->setValueNotifyingHost ((float) slider->getValue());
          mState.getParameter("noisegate")->endChangeGesture();
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

	if (button == &noiseonButton) 
	{
	auto state = button->getToggleState();
		
	mState.getParameter("noiseon")->setValue(state);	
	mState.getParameter("noiseon")->beginChangeGesture();
    mState.getParameter("noiseon")->setValueNotifyingHost (button->getToggleState() ? 1.0f : 0.0f);
    mState.getParameter("noiseon")->endChangeGesture();	  			
	}		

}     
    
void ScorchAmpAudioProcessorEditor::AmpStageMenuChanged()
{

    switch (AmpStageMenu.getSelectedId())
    {
        case 1: 
        mState.getParameter("ampstages")->setValue(0.0); 
        AmpStageMenu.setSelectedId(1); 
        break;
        case 2: 
        mState.getParameter("ampstages")->setValue(0.1); 
        AmpStageMenu.setSelectedId(2); 
        break;
        case 3: 
        mState.getParameter("ampstages")->setValue(0.2); 
        AmpStageMenu.setSelectedId(3); 
        break;
        case 4: 
        mState.getParameter("ampstages")->setValue(0.3); 
        AmpStageMenu.setSelectedId(4); 
        break; 
        case 5: 
        mState.getParameter("ampstages")->setValue(0.4); 
        AmpStageMenu.setSelectedId(5); 
        break; 
        case 6: 
        mState.getParameter("ampstages")->setValue(0.5); 
        AmpStageMenu.setSelectedId(6); 
        break; 
        case 7: 
        mState.getParameter("ampstages")->setValue(0.6); 
        AmpStageMenu.setSelectedId(7); 
        break; 
        case 8:  
        mState.getParameter("ampstages")->setValue(0.7); 
        AmpStageMenu.setSelectedId(8); 
        break;     
        case 9:  
        mState.getParameter("ampstages")->setValue(0.8); 
        AmpStageMenu.setSelectedId(9); 
        break;          
        case 10: 
        mState.getParameter("ampstages")->setValue(0.9); 
        AmpStageMenu.setSelectedId(10); 
        break; 
        case 11: 
        mState.getParameter("ampstages")->setValue(1.0); 
        AmpStageMenu.setSelectedId(11); 
        break;                                                                                            
    }

}    

void ScorchAmpAudioProcessorEditor::paint (Graphics& g)
{

Rectangle<int> rect = {125, 0, 300, 80};

    g.fillAll (Colours::darkblue.brighter());
    g.setColour (Colours::black);     
    g.setFont (40.0f);    
    g.drawText ("ScorchCrafter", rect, Justification::left, true);   

}

void ScorchAmpAudioProcessorEditor::resized()
{
	
	auto sliderLeft = 50;
	
    ScorchMasterSlider.setBounds (sliderLeft, 80, 100, 100);
    ScorchGainSlider.setBounds (sliderLeft + 100, 80, 100, 100);
    ScorchContourSlider.setBounds (sliderLeft + 200, 80, 100, 100);   
    ScorchPresenceSlider.setBounds (sliderLeft + 300, 80, 100 , 100);
    ScorchTrebleSlider.setBounds (sliderLeft, 180, 100 , 100);
    ScorchMidSlider.setBounds (sliderLeft + 100, 180 , 100 , 100);
    ScorchBassSlider.setBounds (sliderLeft + 200, 180 , 100 , 100);   
    ScorchNoiseGateSlider.setBounds (sliderLeft + 300, 180 , 100 , 100);              
                
    brightButton.setBounds(sliderLeft + 150, 300, 70, 20);
    boostButton.setBounds(sliderLeft + 230, 300, 70, 20);
    noiseonButton.setBounds(sliderLeft + 310, 300, 70, 20);
    
    AmpStageMenu.setBounds(sliderLeft, 300, 130, 20);      
     
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

	addAndMakeVisible(ScorchNoiseGateSlider);	
	ScorchNoiseGateSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	ScorchNoiseGateSlider.setSize(mSliderWidth, mSliderHeight);
	ScorchNoiseGateSlider.setTextBoxStyle(Slider::TextBoxBelow, false, mTextBoxWidth, mTextBoxHeight);
	ScorchNoiseGateSlider.setTextValueSuffix(" NoiseGate");
	ScorchNoiseGateSliderAttachment.reset(new SliderAttachment(mState, "noisegate", ScorchNoiseGateSlider));
	ScorchNoiseGateSlider.addListener(this);
		
	addAndMakeVisible (brightButton);
	brightAttachment.reset (new ButtonAttachment (mState, "bright", brightButton));
	brightButton.onClick = [this] { updateToggleState (&brightButton, "Bright"); };		
	brightButton.addListener (this);

	addAndMakeVisible (boostButton);
	boostAttachment.reset (new ButtonAttachment (mState, "boost", boostButton));	
	boostButton.onClick = [this] { updateToggleState2 (&boostButton, "Boost"); };		
	boostButton.addListener (this);

	addAndMakeVisible (noiseonButton);	
        noiseonAttachment.reset (new ButtonAttachment (mState, "noiseon", noiseonButton));	
	noiseonButton.onClick = [this] { updateToggleState3 (&noiseonButton, "NoiseOn"); };		
	noiseonButton.addListener (this);
		
        addAndMakeVisible(AmpStageMenu);        
        AmpStageMenu.onChange = [this] { AmpStageMenuChanged(); };        
        AmpStageMenu.addItem("AmpStages 1", 1);
        AmpStageMenu.addItem("AmpStages 2", 2);
        AmpStageMenu.addItem("AmpStages 3", 3);	
        AmpStageMenu.addItem("AmpStages 4", 4);		
        AmpStageMenu.addItem("AmpStages 5", 5);		
        AmpStageMenu.addItem("AmpStages 6", 6);
        AmpStageMenu.addItem("AmpStages 7", 7);       
        AmpStageMenu.addItem("AmpStages 8", 8);
        AmpStageMenu.addItem("AmpStages 9", 9);
        AmpStageMenu.addItem("AmpStages 10", 10); 
        AmpStageMenu.addItem("AmpStages 11", 11);               
                      
        AmpStageMenu.setSelectedId(6);  
        AmpStageMenuAttachment.reset (new ComboBoxAttachment (mState, "ampstages", AmpStageMenu));  
	AmpStageMenu.addListener (this);  

}

   

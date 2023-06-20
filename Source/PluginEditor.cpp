/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoiseGateBasicAudioProcessorEditor::NoiseGateBasicAudioProcessorEditor (NoiseGateBasicAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 400);
    
    addAndMakeVisible(sliderThreshold);
    addAndMakeVisible(sliderRatio);
    addAndMakeVisible(sliderAttack);
    addAndMakeVisible(sliderRelease);
    
    sliderThreshold.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderThreshold.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderThreshold.setDoubleClickReturnValue(true, 0.0f);
    sliderThreshold.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderThreshold.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderThreshold.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderThreshold.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderThreshold.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    
    labelThreshold.attachToComponent(&sliderThreshold, false);
    labelThreshold.setText("Thresh (dB)", juce::dontSendNotification);
    
    sliderRatio.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderRatio.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderRatio.setDoubleClickReturnValue(true, 0.0f);
    sliderRatio.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderRatio.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderRatio.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderRatio.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderRatio.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    
    labelRatio.attachToComponent(&sliderRatio, false);
    labelRatio.setText("Ratio", juce::dontSendNotification);
    
    sliderAttack.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderAttack.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderAttack.setDoubleClickReturnValue(true, 0.0f);
    sliderAttack.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderAttack.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderAttack.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderAttack.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderAttack.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    
    labelAttack.attachToComponent(&sliderAttack, false);
    labelAttack.setText("Attack (ms)", juce::dontSendNotification);
    
    sliderRelease.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    sliderRelease.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 76, 38);
    sliderRelease.setDoubleClickReturnValue(true, 0.0f);
    sliderRelease.setColour(juce::Slider::ColourIds::thumbColourId, juce::Colours::black.withAlpha(0.0f));
    sliderRelease.setColour(juce::Slider::ColourIds::rotarySliderFillColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderRelease.setColour(juce::Slider::ColourIds::rotarySliderOutlineColourId, juce::Colours::black.withAlpha(0.25f));
    sliderRelease.setColour(juce::Slider::ColourIds::textBoxTextColourId, juce::Colours::whitesmoke.withAlpha(0.25f));
    sliderRelease.setColour(juce::Slider::ColourIds::textBoxOutlineColourId, juce::Colours::black.withAlpha(0.0f));
    
    labelRelease.attachToComponent(&sliderRelease, false);
    labelRelease.setText("Release (ms)", juce::dontSendNotification);
    
    
    
    sliderAttachmentThreshold = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "THRESHOLD", sliderThreshold);
    
    sliderAttachmentRatio = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RATIO", sliderRatio);
    
    sliderAttachmentAttack = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "ATTACK", sliderAttack);
    
    sliderAttachmentRelease = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "RELEASE", sliderRelease);
}

NoiseGateBasicAudioProcessorEditor::~NoiseGateBasicAudioProcessorEditor()
{
}

//==============================================================================
void NoiseGateBasicAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void NoiseGateBasicAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    /*sliderThreshold.setBounds(getWidth()/2 - 200, getHeight()/2 - 100, 100, 100);
    sliderRatio.setBounds(getWidth()/2 - 100, getHeight()/2 - 100, 100, 100);
    sliderAttack.setBounds(getWidth()/2, getHeight()/2 - 100, 100, 100);
    sliderRelease.setBounds(getWidth()/2 + 100, getHeight()/2 - 100, 100, 100);*/
    int size = 150;
    int xOffset = 30;
    int yOffset = 40;
    
    sliderThreshold.setBounds(xOffset, yOffset, size, size);
    sliderRatio.setBounds(getWidth() / 2 + xOffset, yOffset, size, size);
    sliderAttack.setBounds(xOffset, getHeight() / 2 + yOffset, size, size);
    sliderRelease.setBounds(getWidth() / 2 + xOffset, getHeight() / 2 + yOffset, size, size);
}

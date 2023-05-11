/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NoiseGateBasicAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    NoiseGateBasicAudioProcessorEditor (NoiseGateBasicAudioProcessor&);
    ~NoiseGateBasicAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider sliderThreshold;
    juce::Slider sliderRatio;
    juce::Slider sliderAttack;
    juce::Slider sliderRelease;
    
    
    juce::Label labelThreshold; 
    juce::Label labelRatio;
    juce::Label labelAttack;
    juce::Label labelRelease;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentThreshold;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentRatio;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentAttack;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sliderAttachmentRelease;
    
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NoiseGateBasicAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NoiseGateBasicAudioProcessorEditor)
};

/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
NoiseGateBasicAudioProcessor::NoiseGateBasicAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters() )//Constructor for apvts
#endif
{
    //noiseGate = juce::dsp::NoiseGate<float>();
    apvts.state.addListener(this);
}

NoiseGateBasicAudioProcessor::~NoiseGateBasicAudioProcessor()
{
}

//==============================================================================
const juce::String NoiseGateBasicAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool NoiseGateBasicAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool NoiseGateBasicAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool NoiseGateBasicAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double NoiseGateBasicAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int NoiseGateBasicAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int NoiseGateBasicAudioProcessor::getCurrentProgram()
{
    return 0;
}

void NoiseGateBasicAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String NoiseGateBasicAudioProcessor::getProgramName (int index)
{
    return {};
}

void NoiseGateBasicAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void NoiseGateBasicAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();
    
    noiseGate.prepare(spec);
    
}

void NoiseGateBasicAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool NoiseGateBasicAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void NoiseGateBasicAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
    juce::dsp::AudioBlock<float> block (buffer);
    
    noiseGate.setThreshold(apvts.getRawParameterValue("THRESHOLD")->load());
    noiseGate.setRatio(apvts.getRawParameterValue("RATIO")->load());
    noiseGate.setAttack(apvts.getRawParameterValue("ATTACK")->load());
    noiseGate.setRelease(apvts.getRawParameterValue("RELEASE")->load());

    
    //updateParameters();
    noiseGate.process(juce::dsp::ProcessContextReplacing<float>(block));
    
    
    /*for(int i = 0; i < totalNumOutputChannels; ++i)
    {
      for(int j = 0; j < buffer.getNumSamples(); ++j)
      {
          noiseGate.processSample(i, buffer.getSample(i, j));
      }
    }*/
          
          
          

}

//==============================================================================
bool NoiseGateBasicAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* NoiseGateBasicAudioProcessor::createEditor()
{
    return new NoiseGateBasicAudioProcessorEditor (*this);
}


juce::AudioProcessorValueTreeState::ParameterLayout NoiseGateBasicAudioProcessor::createParameters()
{
    juce::AudioProcessorValueTreeState::ParameterLayout params;
    
    params.add(std::make_unique<juce::AudioParameterFloat>("THRESHOLD", "Threshold", -96.0f, 6.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RATIO", "Ratio", 1.0f, 10.0f, 1.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 1.0f, 300.0f, 20.0f));
    params.add(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 1.0f, 700.0f, 20.0f));
    
    return params;
}

//==============================================================================
void NoiseGateBasicAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void NoiseGateBasicAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (apvts.state.getType()))
          apvts.replaceState (juce::ValueTree::fromXml (*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new NoiseGateBasicAudioProcessor();
}

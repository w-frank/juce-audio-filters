
#include "PluginProcessor.h"
#include "PluginEditor.h"


DSPModuleAudioProcessor::DSPModuleAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), valueTree(*this, nullptr, "Parameters", createParameters())
#endif
{
}

DSPModuleAudioProcessor::~DSPModuleAudioProcessor()
{
}

const juce::String DSPModuleAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DSPModuleAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DSPModuleAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DSPModuleAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DSPModuleAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DSPModuleAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DSPModuleAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DSPModuleAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DSPModuleAudioProcessor::getProgramName (int index)
{
    return {};
}

void DSPModuleAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void DSPModuleAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    lastSampleRate = sampleRate;

    juce::dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumOutputChannels();

    stateVariableFilter.reset();
    stateVariableFilter.prepare(spec);

}

void DSPModuleAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

void DSPModuleAudioProcessor::updateFilter()
{
    auto filterTypeValue = valueTree.getRawParameterValue("FILTER_TYPE");
    auto filterCutoffValue = valueTree.getRawParameterValue("FILTER_CUTOFF");
    auto filterResonanceValue = valueTree.getRawParameterValue("FILTER_RESONANCE");

    int filterType = int(filterTypeValue->load());
    float filterCutoff = filterCutoffValue->load();
    float filterResonance = filterResonanceValue->load();

    switch(filterType)
    {
        case 0: 
            stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>
                ::Type::lowPass;
            stateVariableFilter.state->setCutOffFrequency(lastSampleRate, filterCutoff, filterResonance);
            break;
        case 1:
            stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>
                ::Type::bandPass;
            stateVariableFilter.state->setCutOffFrequency(lastSampleRate, filterCutoff, filterResonance);
            break;
        case 2:
            stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>
                ::Type::highPass;
            stateVariableFilter.state->setCutOffFrequency(lastSampleRate, filterCutoff, filterResonance);
            break;
        default:
            stateVariableFilter.state->type = juce::dsp::StateVariableFilter::Parameters<float>
                ::Type::lowPass;
            stateVariableFilter.state->setCutOffFrequency(lastSampleRate, filterCutoff, filterResonance);
            break;
    }
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DSPModuleAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
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

void DSPModuleAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    juce::dsp::AudioBlock<float> block (buffer);

    updateFilter();

    stateVariableFilter.process(juce::dsp::ProcessContextReplacing<float> (block));

}

bool DSPModuleAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DSPModuleAudioProcessor::createEditor()
{
    return new DSPModuleProcessorEditor (*this);
}

void DSPModuleAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DSPModuleAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DSPModuleAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DSPModuleAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_CUTOFF", "FilterCutoff", 20.0f, 20000.0f, 600.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FILTER_RESONANCE", "FilterResonance", 1.0f, 5.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterInt>("FILTER_TYPE", "FilterType", 0, 2, 0));

    return {params.begin(), params.end()};
}



#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


class DSPModuleProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    DSPModuleProcessorEditor (DSPModuleAudioProcessor&);
    ~DSPModuleProcessorEditor() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:

    juce::Slider filterCutoffDial;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;

    juce::Slider filterResDial;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResAttachment;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DSPModuleAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSPModuleProcessorEditor)
};

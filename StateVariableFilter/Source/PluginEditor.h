
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
    juce::Slider filterResDial;
    juce::ComboBox filterSelector;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterCutoffAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> filterResonanceAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> filterSelectorAttachment;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DSPModuleAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DSPModuleProcessorEditor)
};

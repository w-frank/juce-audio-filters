/**
 * @file PluginEditor.cpp
 * 
 * @brief 
 * 
 * @author
 */

#include "PluginProcessor.h"
#include "PluginEditor.h"


DSPModuleProcessorEditor::DSPModuleProcessorEditor (DSPModuleAudioProcessor& p) :
AudioProcessorEditor (&p), processor (p)
{
    setSize (200, 200);

    filterSelector.setJustificationType(juce::Justification::centred);
    filterSelector.addItem("Low Pass", 1);
    filterSelector.addItem("Band Pass", 2);
    filterSelector.addItem("High Pass", 3);
    addAndMakeVisible(&filterSelector);
    filterSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(processor.valueTree, "FILTER_TYPE", filterSelector);

    
    filterCutoffDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(20.0f, 20000.0f);
    filterCutoffDial.setValue(600.0f);
    filterCutoffDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "FILTER_CUTOFF", filterCutoffDial);
    
    filterResDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterResDial.setRange(1.0f, 5.0f);
    filterResDial.setValue(2.0f);
    filterResDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterResDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterResDial);
    filterResonanceAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "FILTER_RESONANCE", filterResDial);

}

DSPModuleProcessorEditor::~DSPModuleProcessorEditor()
{
}

void DSPModuleProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("Filter", titleArea, juce::Justification::centredTop);
    g.drawText("Cutoff", 46, 70, 50, 25, juce::Justification::centredLeft);
    g.drawText("Resonance", 107, 70, 70, 25, juce::Justification::centredLeft);
    
    juce::Rectangle <float> area (25, 25, 150, 150);
    
    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void DSPModuleProcessorEditor::resized()
{

    //need to come back and dynamically set these...ok for now
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    
    filterSelector.setBounds(area.removeFromTop(20));
    filterCutoffDial.setBounds (30, 90, 70, 70);
    filterResDial.setBounds (100, 90, 70, 70);

}
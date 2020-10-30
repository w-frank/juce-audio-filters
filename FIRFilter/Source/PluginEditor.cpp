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

    filterCutoffDial.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(20.0f, 20000.0f);
    filterCutoffDial.setValue(600.0f);
    filterCutoffDial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterCutoffDial);
    filterCutoffAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.valueTree, "FILTER_CUTOFF", filterCutoffDial);

    filterCutoffDial.setSkewFactorFromMidPoint(1000.0f);
}

DSPModuleProcessorEditor::~DSPModuleProcessorEditor()
{
}

void DSPModuleProcessorEditor::paint (juce::Graphics& g)
{
    juce::Rectangle<int> titleArea(0, 10, getWidth(), 20);
    
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.drawText("FIR Low Pass Filter", titleArea, juce::Justification::centredTop);

    juce::Rectangle <float> area (25, 25, 150, 150);
    
    g.setColour(juce::Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);
}

void DSPModuleProcessorEditor::resized()
{

    //need to come back and dynamically set these...ok for now
    juce::Rectangle<int> area = getLocalBounds().reduced(40);
    
    filterCutoffDial.setBounds (area.getWidth() - 90, area.getHeight() - 90, 140, 140);

}
# JUCE Audio Filters

Collection of digital audio filters using the JUCE DSP module. Built using the [JUCE](https://github.com/juce-framework/JUCE) 
audio application framework.

* Finite impulse response (FIR) filter (lowpass, cutoff control)
* Infinite impulse reponse (IIR) filter (lowpass, cutoff/resonance control)
* State variable (SV) filter (lowpass/bandpass/highpass, cutoff/resonance control)

Based on the excellent tutorials by [The Audio Programmer](https://github.com/TheAudioProgrammer/).

## Build Environment
* JUCE v6.0.4
* Ubuntu 18.04.5 LTS

## Build
```bash
cd <filter_type>/Builds/LinuxMakefile
make
```

## Run
```bash
cd <filter_type>/Builds/LinuxMakefile/build
./<filter_type>
```
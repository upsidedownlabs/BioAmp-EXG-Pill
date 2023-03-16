# EEG FILTERING

## Introduction
Filtering of the raw EEG signal is an important part before cutting it into short segments and taking the event code of interest because the long segment of data is required for accurate estimation and also to remove low fequencies. *Digital filtering* is a common method for preprocessing EEG signal.There are various toolbox present to preprocess raw EEG signal and that uses digital filters to do the filtering. One such is *EEG Toolbox in MATLAB*. BioAmp-Exg-Pill uses an *Biquad IIR digital filter*. This is a recurssive filter whose output of the filter is depends on the *current,previous inputs and outputs*. As it uses the  previous values of the output,so the *feedback of the output* is present. It is a second order filter that has *two poles and two zeros* but in practical term it can act like a *lowpass, hipass, bandpass, notch, shelf*, etc.depending on the values of the coefficients in the filtering equation.Also,biquad filters can be cascadded for filtering. Previous research suggest that IIR filter are best suited filters for transcranial signal processing. 

**Block diagram of IIR filter**

![Capture](https://user-images.githubusercontent.com/122666785/224307805-3c4e67ec-bc96-42e2-8335-df99cb492c3c.PNG)

### Specifications of the implemented filter

1. *Passband frequency*= 0.5Hz to 29Hz
2. Biquad filter is used as a *bandpass filter*.

**Response of the filter**

![filter](https://user-images.githubusercontent.com/122666785/224324132-cd281ce0-e568-4528-b4c0-ef10c13de032.PNG)

### Advantage of using IIR filter

1) *Low implementation cost*: Requires less coefficients and memory than FIR filters in order to satisfy a similar set of specifications 
2) *Low latency*: Suitable for real-time control and very high-speed RF applications by virtue of the low number of coefficients
3) *Analog equivalent*: May be used for mimicking the characteristics of analog filters using s-z plane mapping transforms


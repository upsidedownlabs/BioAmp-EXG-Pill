# Introduction
<br>**Digital filtering** is a common preprocessing step when analysing EEG to remove noise and low frequency.BioAmp-Exg-Pill uses an **Biquad IIR digital filter**,which is used after connecting it to the development board. This is a recurssive filter whose output of the filter is depends on the **current,previous inputs and outputs**. As it uses the  previous values of the output,so the **feedback of the output** is present. It has **two poles and two zeros** to be BIBO stable but in practical term it can act like a lowpass, hipass, bandpass, notch, shelf, etc.depending on the values of the coefficients in the filtering equation.Also,biquad filters can be cascadded for filtering.Previous research suggest that IIR filter are best suited filters for transcranial signal processing. 
To filter the EEG data, this biquad IIR digital filter can be used as a bandpass filter and whose passband frequence is from **0.5Hz to 29Hz**.</br>
<br><p align="center">"![Capture](https://user-images.githubusercontent.com/122666785/224307805-3c4e67ec-bc96-42e2-8335-df99cb492c3c.PNG)"</p>
<p align="center">Fig 1:Block diagram of IIR filter</p></br>

## Response of the filter
<br><p align="center"> ![filter](https://user-images.githubusercontent.com/122666785/224324132-cd281ce0-e568-4528-b4c0-ef10c13de032.PNG)</p>
<p align="center">Fig 2: response of the filter</p></br>

## Advantage of using IIR filter
<br>1) **Low implementation cost**: Requires less coefficients and memory than FIR filters in order to satisfy a similar set of specifications 
<br>2) **Low latency**: Suitable for real-time control and very high-speed RF applications by virtue of the low number of coefficients
<br>3) **Analog equivalent**: May be used for mimicking the characteristics of analog filters using s-z plane mapping transforms</br> 

## References
1)Introduction to Digital Signal Processing
<br>2)MaxCookbook Biquad filter
<br>3)Mikroke IIR filter
<br>4)M.Farrok,K. Dunlop,P.Giacobbe,J.Donwar and J.Zarrifa;Front. Neurosci.,Sec. Neural Technolog,volume 11,"A Fast EEG Forecasting Algorithm for Phase-Locked Transcranial Electrical Stimulation of the Human Brain".doi-10.3389/fnins.2017.00401</br>

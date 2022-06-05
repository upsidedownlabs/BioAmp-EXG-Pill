<blockquote>

# Electroencephalography (EEG) &#129504;

</blockquote>

 <a href="https://en.wikipedia.org/wiki/Electroencephalography" style="color: #ff3385; text-decoration: none;"> Electroencephalography (EEG) </a>  is an electrophysiological monitoring method to record the brain's electrical activity on the scalp. During the procedure, electrodes consisting of small metal discs with thin wires are pasted onto the scalp. The electrodes detect <a href="https://www.hopkinsmedicine.org/health/treatment-tests-and-therapies/electroencephalogram-eeg" style="color: #ff3385; text-decoration: none;">tiny electrical charges</a>  that result from brain-cell activity. Those charges are then amplified to appear on a computer screen. It is typically non-invasive, with the electrodes placed along the scalp. The resulting signal is called an electroencephalogram, an example of which is shown below:
<br></br>
<p style="text-align: center;"> <b> Resulting EEG Signal Wave </b></p>
<div style="text-align:center;">
<img src=" images/EEG/bioamp-exg-pill-electroencephalography-wave.jpg" alt="eeg sketch" style="height: 285px; width:560px;"/>
</div>

## How to Create an Electroencephalograph With BioAmp EXG Pill?

An EEG is performed using an instrument called an electroencephalograph to produce a record called an electroencephalogram. Electrodes are commonly placed on the forehead, as shown in the diagram below, when recording frontal-cortex EEG signals.
<br></br>
<p style="text-align: center;"> <b> Frontal-cortex EEG Signal Sketch </b></p>
<div style="text-align:center;">
<img src=" images/EEG/bioamp-exg-pill-electroencephalography.jpg" alt="eeg sketch" style="height: 315px; width:560px;"/>
</div>
<br>
</br>
To record nice clean EEG signals with BioAmp EXG Pill, all you need is the Analog Serial Out sample Arduino sketch. If you want some more control we also have a <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/tree/main/software/FixedSampling/FixedSampling.ino" style="color:  #ff3385; text-decoration: none;"> Fixed Sampling example sketch</a> for recording EEG and other biopotential signals at a specific sampling rate. You can also use the <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/blob/main/software/EEGFilter/EEGFilter.ino" style="color:  #ff3385; text-decoration: none;"> EEG Filter example sketch </a>  for recording EEG at a sampling rate of 256.0 Hz and a frequency of 0.5 - 29.5 Hz. The image below shows the fourth-order Butterworth IIR digital bandpass filter used in the EEG Filter example sketch.
<br></br>
<p style="text-align: center;"> <b> EEG Filter Sketch </b></p>
<div style="text-align:center;">
<img src=" images/EEG/bioamp-exg-pill-eegfilter.jpg" alt="eeg sketch" style="height: 315px; width:560px;"/>
</div>

## Frontal EEG Recording

The video below shows a frontal electroencephalography (EEG) recording for both open- and closed-eye positions. The transition between the signals is also very clearly visible. To get an even better EEG signal, it's recommended to use a dedicated ADC like the Texas Instruments <a href="https://www.ti.com/product/ADS1115" style="color: #ff3385; text-decoration: none;"> ADS115 (16-bit) </a>  or <a href="https://www.ti.com/product/ADS131M08" style="color: #ff3385; text-decoration: none;"> ADS131M08 (24-bit) </a>. On the new BioAmp EXG Pill v1.0, we recommend creating a solder bridge on the back side of the PCB to set the narrow bandpass option.

<p style="text-align: center;"> <b> Frontal EEG Recording </b></p>
<div style="text-align:center;">
<iframe title="vimeo-player" src="https://player.vimeo.com/video/617188926?h=809ae189ee" width="560" height="315" frameborder="0" allowfullscreen></iframe>
</div>

_**Note**: BioAmp EXG Pill is not a certified medical device and should not be treated like one._

## References

1. <a href="https://en.wikipedia.org/wiki/Electroencephalography" style="color: #ff3385; text-decoration: none;">https://en.wikipedia.org/wiki/Electroencephalography </a>
2. <a href="https://www.hopkinsmedicine.org/health/treatment-tests-and-therapies/electroencephalogram-eeg" style="color: #ff3385; text-decoration: none;"> https://www.hopkinsmedicine.org/health/treatment-tests-and-therapies/electroencephalogram-eeg </a>

 
BioAmp-EXG-Pill has an inbuilt EEG filter, which is a **IIR digital bandpass butterworth filter**,and is .Its passband frequency is from 0.5Hz and the bandstop frequency is 29Hz. 
Specifications of the filters are:
1) **Fourth order filter** and implemented as a **biquads** (second order section)
2) Sampling rate of the filter is **256Hz**
3) The rate at which the information is transferred is 115200 baud.
Advantages of using the biquad bandpass filter, it will allow to custom low-pass, high-pass, or any other desired frequency shaping of the signal and the frequency response of a Biquad Filter is flat gain of 0 dB.

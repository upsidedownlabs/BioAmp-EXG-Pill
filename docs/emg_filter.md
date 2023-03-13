# **EMG FILTERING**

## **Introduction**

The surface electromyographic (sEMG) signal contains the signal that originates in the muscle and various noise components
which are endemic and unavoidable. These noise components contaminate the sEMG signal and may lead to an erroneous
interpretation of the signal making filtering an importance processes before cutting the long segment into short ones.

<br> There are several toolboxes and methods for filtering electromyography (EMG) signals. Some of the commonly used toolboxes are:
<ol>
<li> Butterworth filter: It is a widely used low-pass filter that can be used to remove high-frequency noise from EMG signals. The Butterworth filter is a type of infinite impulse response (IIR) filter that can be implemented in Matlab or Python.
<li> Wavelet transform: The wavelet transform is a time-frequency analysis tool that can be used to filter EMG signals. The wavelet transform can be used to decompose EMG signals into different frequency bands, which can then be filtered to remove noise.
<li> Bandpass filter: A bandpass filter can be used to isolate the frequency band of interest in EMG signals. The bandpass filter can be designed to remove low and high-frequency noise while retaining the EMG signal of interest.
<li> Adaptive filter: An adaptive filter is a type of digital filter that can be used to remove noise from EMG signals. The adaptive filter can be trained to learn the noise characteristics of the EMG signal and remove them in real-time.
<li> Independent component analysis (ICA): ICA is a signal processing technique that can be used to separate independent sources of noise from EMG signals. ICA can be used to remove artifacts such as movement artifacts or electrical noise.
<li> Kalman filter: The Kalman filter is a statistical method that can be used to estimate the underlying EMG signal while removing noise. The Kalman filter can be used to track the EMG signal in real-time and estimate the noise characteristics of the signal. and that uses digital filters to do the filtering.</li></ol></br>


**Block diagram of IIR filter** 

<br><p align="center">
![Capture](https://user-images.githubusercontent.com/122666785/224307805-3c4e67ec-bc96-42e2-8335-df99cb492c3c.PNG)</p>

### <br>**Specifications of the implemented filter**</br>

1. *Passband frequency*= 74.5 Hz to 149.5 Hz
2. Also the sampling freqquency is 500 Hz
3. Biquad filter is used as a *bandpass filter*.
4. Response of the filter

<br><p align="center"> ![EEG FILTER](https://user-images.githubusercontent.com/122666785/224680965-c9a5a4f8-8efb-4990-b88e-cb9054818f78.PNG) </p>

### **Filtered EMG** 
<br> <p align="center"> ![EMG FILTER](https://user-images.githubusercontent.com/122666785/224681471-6ee6942c-bb29-49b9-9a27-4bd459a01e6d.PNG)</p>

### **References**

1) Introduction to Digital Signal Processing 
2) MaxCookbook Biquad filter
3) Mikroke IIR filter
4) M.Farrok,K. Dunlop,P.Giacobbe,J.Donwar and J.Zarrifa;Front. Neurosci.,Sec. Neural Technolog,volume 11,"A Fast EEG Forecasting Algorithm for Phase-Locked Transcranial Electrical Stimulation of the Human Brain".doi-10.3389/fnins.2017.00401
5) Carlo J. De Luca, et.,Journal of Biomechanics 43 (2010) 1573–1579,"Filtering the surface EMG signal: Movement artifact and
baseline noise contamination"


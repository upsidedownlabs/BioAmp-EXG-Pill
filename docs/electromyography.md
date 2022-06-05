># Electromyography (EMG) &#128170;


<a href="https://en.wikipedia.org/wiki/Electromyography" style="color: #b38600; text-decoration: none;"> Electomyography (EMG) </a> is a technique for evaluating and recording the electrical activity produced by skeletal muscles. EMG is also used as a diagnostic procedure to assess the health of muscles and the nerve cells that control them (motor neurons). EMG results can reveal nerve dysfunction, muscle dysfunction, or problems with nerve-to-muscle signal transmission. The image below shows an EMG wave recorded with BioAmp EXG Pill.
<br></br>
<p style="text-align: center;"> <b> Resulting EMG Signal Wave </b></p>
<div style="text-align:center;">
<img src=" images/EMG/bioamp-exg-pill-emg-wave.jpg" alt="eeg sketch" style="max-width:70%;border:6px " style="height: 315px; width:560px;"/>
</div>
<br></br>

## How to Create an Electromyograph with BioAmp EXG Pill?

EMG is performed using an instrument called an electromyograph to produce a record called an electromyogram. Common electrode placement for recording good EMG signal for hand movement is near the ulnar nerve, as shown in the diagram below.
<br></br>
<p style="text-align: center;"> <b> Hand Movement EMG Signal Sketch </b></p>
<div style="text-align:center;">
<img src=" images/EMG/bioamp-exg-pill-electromyograph.jpg" alt="eeg sketch" style="height: 315px; width:560px;"/>
</div>
<br></br>

To record nice clean EMG signals with BioAmp EXG Pill, all you need is the Analog Serial Out Arduino sketch. If you want more control we also have a <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/tree/main/software/FixedSampling/FixedSampling.ino" style="color: #b38600; text-decoration: none;"> Fixed Sampling Arduino sketch</a> that allows you to record EMG and other Biopotential signals at a specific sampling rate. You can also use the <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/blob/main/software/EMGFilter/EMGFilter.ino" style="color: #b38600; text-decoration: none;"> EMG Filter Arduino sketch</a> for recording EMG at a 500 Hz sampling rate and a 74.5 - 149.5 Hz frequency. The image below shows the 4th order bandpass Butterworth IIR digital filter used in the EMG Filter sample sketch:
<br></br>
<p style="text-align: center;"> <b> EMG Filter Sample Sketch </b></p>
<div style="text-align:center;">
<img src=" images/EMG/bioamp-exg-pill-emgfilter.jpg" alt="eeg sketch" style="max-width:70%;border:6px " style="height: 315px; width:560px;"/>
</div>
<br></br>
For practical use cases, we need a smooth signal, but the raw EMG signal does not come out as a smooth curve, which is why we offer the <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/tree/main/software/EMGEnvelop/EMGEnvelop.ino" style="color: #b38600; text-decoration: none;"> envelope-detection Arduino sketch</a> for BioAmp EXG Pill. The video below shows envelope detection at work:

<p style="text-align: center;"> <b> Envelope Detection Recording </b></p>
<div style="text-align:center;">
<iframe title="vimeo-player" src="https://player.vimeo.com/video/593798539?h=ef34fc28c3" width="640" height="284" frameborder="0" allowfullscreen></iframe>
</div>

## Example EMG Projects with BioAmp EXG Pill

BioAmp EXG Pill allows you to do a lot with muscle power. We show two examples below: a servo controller and an LED bar graph. Both demonstrations use code that is derived from the <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/tree/main/software/EMGEnvelop/EMGEnvelop.ino" style="color: #b38600; text-decoration: none;"> envelope-detection Arduino sketch</a>. The image below shows how envelope detection works on the Raw EMG signal.
<br></br>
<p style="text-align: center;"> <b> EMG Envelope Wave </b></p>
<div style="text-align:center;">
<img src=" images/EMG/bioamp-exg-pill-emgenvelope.jpg" alt="eeg sketch" style="max-width:70%;border:6px " style="height: 315px; width:560px;"/>
</div>
<br></br>


- ### Servo Controller

Controlling a servo motor with BioAmp EXG Pill is pretty easy, as shown in the video below. To create the same project, all you have to do is load the <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/tree/main/software/ServoControl/ServoControl.ino" style="color: #b38600; text-decoration: none;"> servo-control Arduino sketch</a> onto your Arduino-compatible board, and you're good to go.

<p style="text-align: center;"> <b> Servo-Control using BioAmp EXG pill</b></p>
<div style="text-align:center;">
<iframe width="560" height="315" src="https://www.youtube.com/embed/ZePE1umyzFI" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
</div>

- ### LED Bar Graph

The same envelope-detection concept is used for the LED bar graph project. You can use as many LEDs you want with this project. To make a cool LED bar graph like the one shown in the video below, just edit the <a href="https://github.com/upsidedownlabs/BioAmp-EXG-Pill/tree/main/software/LEDBarGraph/LEDBarGraph.ino" style="color: #b38600; text-decoration: none;"> LED bar graph Arduino sketch </a> , upload it to the Arduino board, then connect the LEDs according to the specified pin numbers.

<p style="text-align: center;"> <b> LED Bar Graph Project using BioAmp EXG Pill </b></p>
<div style="text-align:center;">
<iframe width="560" height="315" src="https://www.youtube.com/embed/MzLAFVXk7_M" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
</div>

## References

1. <a href="https://www.mayoclinic.org/tests-procedures/emg/about/pac-20393913" style="color: #b38600; text-decoration: none;"> Mayo Clinic article on EMG</a>
2. <a href="https://en.wikipedia.org/wiki/Electromyography" style="color: #b38600; text-decoration: none;"> Wikipedia Electromyography page</a>
3. <a href="https://www.instructables.com/Claw-Controller-Using-BIOAMP-EXG-PILL/" style="color: #b38600; text-decoration: none;"> PR ROBOTICS _Instructables_ article on _Claw Controller Using BioAmp EXG PILL_</a>

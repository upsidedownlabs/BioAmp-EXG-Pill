# BioAmp EXG Pill


![Project Category](https://img.shields.io/badge/Category-Bioelectronics-gold) ![BioAmp EXG Pill Revision ](https://img.shields.io/badge/Version-0.9-success) [![BioAmp EXG Pill intro video ](https://img.shields.io/badge/Intro-YouTube-red)](https://www.youtube.com/watch?v=-G3z9fvQnuw) [![BioAmp EXG Pill Crowd Supply page ](https://img.shields.io/badge/Support_Project-Crowd_Supply-00B5AC)](https://www.crowdsupply.com/upside-down-labs/bioamp-exg-pill) [![BioAmp EXG Pill Hackaday Page ](https://img.shields.io/badge/Documentation-hackaday.io-yellowgreen)](https://hackaday.io/project/178997-bioamp-exg-pill) [![BioAmp EXG Pill PCB ](https://img.shields.io/badge/PCB-OSH_Park-purple)](https://oshpark.com/shared_projects/OPELY7qK)

[![Down Labs BioAmp EXG Pill Crowd Supply banner](images/Crowd-Supply-Banner.gif)](https://www.crowdsupply.com/upside-down-labs/bioamp-exg-pill)

BioAmp EXG Pill is a small (2.54 X 1.00 cm) and elegant Analog Front End (AFE) board for BioPotential signal acquisition that you can use with any 5v Micro Controller Unit (MCU) with an ADC. It is capable of recording publication grade BioPotential signals like ECG, EMG, EOG, and EEG without the inclusion of any dedicated Hardware/Software filter, see [BioAmp EXG Pill v0.7 intro video](https://www.youtube.com/watch?v=-G3z9fvQnuw) for more info.

#### <strong style="color:red">Note: graphics and images shown are for v0.7 only. The hardware files has been updated for v0.9 with more functionalities. Graphics, images, and documentation will be updated directly for the final v1.0 revision of the board!</strong>


![Upside Down Labs BioAmp EXG Pill Assembled](images/BioAmp-EXG-Pill-Assembled.png)

![Magic of Upside Down Labs BioAmp EXG Pill Assembled](images/Magic-of-BioAmp-EXG-Pill.png)

## Software
BioAmp EXG Pill works with any 5V microcontroller with an ADC like Arduino UNO/Nano or you can use dedicated 5v compatible ADC like ADS1115. To help with signal processing and cleaning you can use the included Arduino example sketches listed below.

| No. | Program| Description |
| ---- | ---- | ---- |
|1 | [FixedSampling](software/FixedSampling)| Sample from ADC at a fixed rate for easy processing of signal.|
|2 | [EMGFilter](software/EMGFilter)| A 74.5 - 149.5 Hz band pass filter sketch for clean Electromyography.|
|3 | [ECGFilter](software/ECGFilter)| A 0.5 - 44.5 Hz band-pass filter sketch for clean Electrocardiography.|
|4 | [EOGFilter](software/EOGFilter)| A 0.5 - 19.5 Hz band-pass filter sketch for clean Electrooculography.|
|5 | [EEGFilter](software/EEGFilter)| A 0.5 - 29.5 Hz band-pass filter sketch for clean Electroencephalography.|
|6 | [EMGEnvelop](software/EMGEnvelop)| EMG signal envelop detection for robotics and biomedical applications.|
|7 | [LEDBarGraph](software/LEDBarGraph)| LED bar graph showing EMG amplitude.|
|8 | [ServoControl](software/ServoControl)| Servo motor control with EMG.|
|9 | [HeartBeatDetection](software/HeartBeatDetection)| Standard deviation based heart beat detection algorithm.|

To-Do:

| No. | Program| Description |
| ---- | ---- | ---- |
|1 | HeartRate | Heart rate calculation with ECG.|
|2 | BrainWaves | Brain waves amplitude with FFT.|
|3 | EOGController | EOG based eye movement detection (left/right) to create a game controller.|
|4 | BlinkDetection | EOG based eye blink detection.|



`Note: All sketches has been tested on Arduino Nano only.` 


## Hardware

BioAmp EXG Pill has been created using KiCad and all the design files can be found under [hardware](hardware/) folder, including production [Gerber](hardware/gerbers) files. Images below shows a quick overview of the hardware design.

| Front (v0.7)              |  Back (v0.7) |
| :-------------------------: | :-------------------------: |
| ![Upside Down Labs BioAmp EXG Pill front](images/BioAmp-EXG-Pill-v0.7-front-darkpurple.png)  | ![Upside Down Labs BioAmp EXG Pill back](images/BioAmp-EXG-Pill-v0.7-back-darkpurple.png) |

| Dimensions (v0.7)             |  Schematic (v0.7)  |
| :-------------------------: | :-------------------------: |
| ![Upside Down Labs BioAmp EXG Pill dimensions](images/BioAmp-EXG-Pill_dimensions.png)  | ![Upside Down Labs BioAmp EXG Pill schematic](images/BioAmp-EXG-Pill_schematic.png) |

| Licenses Facts              |  OSHWA Certification |
| :-------------------------: | :-------------------------: |
| <a href="LICENSE.md"><img src="images/Licenses_facts.svg" width="400" alt="Open Source Licenses Facts"/></a>  | <a href="https://certification.oshwa.org/in000026.html"><img src="images/OSHW_mark_IN000026.png" width="300" alt="Open Source Hardware Certification mark"/></a> |


## License

#### Hardware
CERN Open Hardware License Version 2 - Strongly Reciprocal ([CERN-OHL-S-2.0](https://spdx.org/licenses/CERN-OHL-S-2.0.html)).

#### Software
MIT open source [license](http://opensource.org/licenses/MIT).

#### Documentation:
<a rel="license" href="http://creativecommons.org/licenses/by/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by/4.0/">Creative Commons Attribution 4.0 International License</a>.

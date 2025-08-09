# FIR-FiltersBank
FIR Filters Bank with C for DSK6713 development board

It is coded in C language in the Code Composer Studio IDE and implemented on the DSK6713 development board as a bank of FIR filters to analyze its behavior in audio playback.
## Design and implementation
To design the FIR filter bank, the block diagram in the following figure is taken into account.
![Block Diagram FIR Filter Bank](img/SystemBlockDiagram.png)

The design specifications as follow
| Filtro |	fc1 |	fc2 |
| ------ | ---- | --- |
| 1	| 250 Hz	| 500 Hz |
| 2	| 500 Hz	| 875 Hz |
| 3	| 875 Hz	| 1150 Hz |
| 4	| 1150 Hz	| 1450 Hz |
|5	| 1450 Hz |	2000 Hz |
|6	| 2000 Hz	| 2600 Hz |
|7	| 2600 Hz	| 3800 Hz |
|8	| 3800 Hz	| 6800 Hz |

Some considerations:
1. Use the dip switch on the development card to select the filter through which the input signal will pass, and the filtered signal will be obtained.
2. Design a filter bank for each window: (Hamming, Hanning, Kaiser)
3. The number of coefficients is determined by N=251
4. The calculated filter coefficients are stored in a C header file (.h).
5. Use the MATLAB Script to coefficients file
6. Consider the FIR Filter algorithm

       FIR Algorithm
       Variables
         integer N
         float x[N], h[N], yn
       Start
         Repeat For k <- 0 to N
           x[i] <- x[i-1]
         End For
         x[0] <- xn
         yn <- 0
         Repeat For k <- 0 to N-1
           yn <- yn + h[k] * x[k]
         End For
         Return yn
       End

## Results
Srarting with filter responses
* Hamming FIR Filter
<img width="591" height="488" alt="image" src="https://github.com/user-attachments/assets/3df31375-d16b-4297-b515-99d5a466800b" />

* Hanning FIR Filter
<img width="591" height="488" alt="image" src="https://github.com/user-attachments/assets/f79ef216-4b2d-4ace-8ca7-9977311e5aea" />

* Kaiser FIR Filter
<img width="591" height="488" alt="image" src="https://github.com/user-attachments/assets/eef73569-43ed-4ce7-b884-4956202da439" />


Results of some actual measurements on an oscilloscope

| Window | Filter | Rejetion band | Pass band |
| ------- | ------ | ---------------- | ------------- |
| Hamming | 250 - 500 Hz | <img width="355" height="185" alt="image" src="https://github.com/user-attachments/assets/cb832b32-f7f9-415f-8aaa-d4d82cc262d3" /> Input: 60 Hz | <img width="383" height="200" alt="image" src="https://github.com/user-attachments/assets/83f5deb3-75e9-47cf-8f8c-454d81bec74d" /> Input: 300 Hz |
| Hanning | 1150 - 1450 Hz | <img width="354" height="179" alt="image" src="https://github.com/user-attachments/assets/42168bfc-602c-49a3-97f0-d0d424c8f17d" /> Input: 1000 Hz | <img width="389" height="203" alt="image" src="https://github.com/user-attachments/assets/96b60acd-8745-4e42-bcfc-45668a624f4d" /> Input: 1300 Hz |
| Kaiser | 3800 - 6800 Hz | <img width="349" height="183" alt="image" src="https://github.com/user-attachments/assets/eb614cf4-978b-4df6-8674-2c8bcf82ab28" /> Input: 3500 Hz | <img width="388" height="202" alt="image" src="https://github.com/user-attachments/assets/32a81dea-4bfb-43a0-abf1-00e7d5779234" /> Input: 5500 Hz |

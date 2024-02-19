### TODO:

- modify your sequencer to use the PIO as its primary I/O engine, including the ability to R/W any register 





### Peripheral used

- GPIO
- PIO

### Operation 

In this demonstration, which mirrors the setup described in Part 4, I showcased two cycles of capturing and replaying the BOOT button press sequence, with an exploration into varying the replay speeds.
Upon launching the program, the console will prompt the user to press 'c' to initiate a 10-second recording phase for capturing the BOOT button press sequence. Following the recording, the sequence will be replayed at a speed specified by the user. The PIO is employed both for monitoring the BOOT button presses and for controlling the light to provide visual feedback during the replay phase.

Video: 

![](IMG_3325.gif)
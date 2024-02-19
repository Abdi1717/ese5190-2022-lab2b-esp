### TODO:

Update your sequencer to be able to slow down and speed up recordings/replay. On the input side, the goal is ultimately to be able to handle the full 4 Gbps "firehose" from the PIO logic analyzer example in the SDK manual, which would fill up 256Kb of RAM in only 31 µs at a system clock speed of 125000 MHz if captured 'raw'! On the output side, the goal is to be able to output precisely timed sequences from the PIO at system clock resolution based on a handful of control points.

- update your sequencer to be able to record just the timestamped transitions between input values, and to be able to play data in this format
- give your sequencer the ability to select a range of output rates for both live and recorded input


### Video

When the C code is executed on the RP2040 and the Python script is run on the CPU, the console will prompt the user to choose between initiating a 10-second recording of BOOT button presses or replaying a previously recorded sequence at a user-defined speed. The Python script is responsible for handling user input. In this demonstration, I completed two cycles of recording and replaying the BOOT button press sequence, experimenting with various replay speeds.

![](IMG_3321.gif)




### TODO:

Create a REPL to let you read and write RP2040 registers from a console. You should be able to:
- select any 32-bit address to read/write (even if not a valid RP2020 address)
- read/write any 32-bit value to this address
- read/write using any of the atomic bit-setting aliases and a 32-bit mask


### Peripheral used

- GPIO


## How the prototype works

Upon launching the program, the console will prompt the user to enter the register address and select either a read or write operation. Following the user's input, the program will execute the specified action—either reading from or writing to the register—and subsequently display a confirmation message on the console.



### Overview:


![](lab2Output.PNG)
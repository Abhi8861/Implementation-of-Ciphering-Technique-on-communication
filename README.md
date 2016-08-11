# Implementation-of-Ciphering-Technique-on-communication
Already done 1.5 years back as a requirement for my min-Project.

PC1 (Encryption software)
We have developed a graphical user interface which handles the message to be sent to the microcontroller. This code has been developed by using Platform Independent Programming language and it has to be sent to the controller using native c embedded in JAVA code.(through the serial communication bus).

Serial Communication Bus 1:
The data is sent through this Bus(a cable) to the microcontroller from PC1’s
Communication port.

Microcontroller:
We chose 2 microcontrollers-pic16f886 which has an I2C port and a UART port. A program written in embed C is flashed onto the controller through USB. This program handles the transmit and receive of the 2 serial communication port.

PC2 (Decryption software)
The platform independent software is used in PC2, the message coming at Communication port is read and by only providing a Valid key, the message contents is revealed .

This was done to understand data security and implement a basic security algorithm with also the aim to make a simple embedded system to specifically work on our needs.

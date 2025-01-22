# Donut
A native C program for the PIC18F2420 microcontroller, displaying the iconic "spinning donut" animation on a 2x16 ASCII LCD screen!

# Info
* Clock Speed: The system uses an 8 MHz crystal oscillator, which is internally multiplied by 4, resulting in a 32 MHz clock speed.
* LCD Interface: The LCD is controlled using a 4-pin data connection.
* Computation: The program primarily utilizes 4-byte floating-point precision for calculations. In specific cases, 1-byte fixed-point values are employed to optimize performance and address memory constraints, as the system has only 768 bytes of RAM.
* Performance: The animation achieves a frame rate of approximately 1 frame per second (1 FPS).

# Example

![test board](https://i.imgur.com/Yonw7XK.jpeg)

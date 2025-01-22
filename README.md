# Donut
Native C for PIC18F2420, displaying the famous "spinning donut" on an ASCII LCD 2x16 screen!

# Info
The crystal oscillator that was used is a 8MHz and since it is multiplied by 4 we get a 32MHz clock speed! The LCD is driven using 4-pin data connection and the computation is done mostly using 4byte floating point precission except in some cases where we use fixed point 1 byte values to save some computation and resolve memory limitations given the 768B of RAM. The animation runs at around 1fps.

# Example

![test board](https://i.imgur.com/Yonw7XK.jpeg)

# Hulk Jump Pixel Game Project
A superhero black-white-pixel game controlled by buttons and joystick about Hulk and Loki
inspired by GameBoy, Street Fighter, and Super Mario Bros. Object-oriented programming is utilized to construct the modular and reusable game design using development board LPC 1768 for the game display shown on the LCD screen.

# Release/Demo Version
## ***Tutorial Option***
https://youtube.com/shorts/W21ZPGGsOWk?feature=share
## ***Game Demo***

# Game Features
|Interaction of the characters|
|---|
|![20210429_130628000_iOS](https://user-images.githubusercontent.com/71925079/192747160-522360d8-5e4a-484b-a2c2-807e4f0f6267.jpg) |
-	Hulk and Loki use fireball and bubble to attack each other respectively
-	Ejection of bubbles from Loki used ticker function of mbed, upper and bottom bubbles take turns to be ejected by Loki, which pass the timer flags from main.cpp to the Loki.cpp

|Collision of attacks|
|---|
|![20210429_130703000_iOS](https://user-images.githubusercontent.com/71925079/192748700-0117f526-dfd1-4d6f-9769-9ed319d64a79.jpg)|
-	The collisions for the attacks used a general bool function to detect, simply inputting the coordinates of two objects and the detected range
-	Different pixel paints are drawn to illustrate the collision motions
-	The sequence of the interaction of them in coding throughout the boss stage is crucial as it affects which object covers another object

|Consistency of pixel frames|
|---|
|![20210429_130817000_iOS](https://user-images.githubusercontent.com/71925079/192749233-32bf881d-8b28-46eb-bbff-5c0f98e2ecab.jpg)|
-	In order to interrupt a consistent motion, I used pass-by-reference functions of returning flags and set or get positions of different objects throughout the files and initialize the motion 
-	Counter is used to replace the function of for and while loop as a counter of if statement can be incremented by the frame running on the main.cpp, any for or while loop outside the main.cpp will interrupt the smoothness of the game








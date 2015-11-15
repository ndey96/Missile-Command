# Missile-Command

This project is a one dimensional adaption of the classic arcade game "Missile Command"

The original game: https://www.youtube.com/watch?v=8eC_6QzvLrE

A demo of this project: https://youtu.be/K2Q93NSWim4

![img_5223](https://cloud.githubusercontent.com/assets/10405248/11170485/e72a53a2-8ba3-11e5-8f9e-18459ed40207.jpg)

##The Rules:
- The game consists of a 3 x 8 array of LEDs and 3 buttons
- "Bombs" fall from the top of each column
- Pressing a button will send a "missile" shooting up one of the columns
- When a missile meets a bomb, both the missile and the bomb are removed from play
- The speed at which bombs are generated increases as the game goes on
- If a bomb to reaches the bottom of a column, the column is removed from play
- When all columns are removed from play, the game ends
- The object of the game is to play as long as possible

##How it works:

###The LED Array:
![fa21jg0gk0yutkn large](https://cloud.githubusercontent.com/assets/10405248/11170613/acb83a78-8ba7-11e5-9a7f-b8a868398372.jpg)

- Each row is hooked up to one pin
- Each column hooked up to one pin
- The program iterates through the columns fast enough that it seems constant

###The Buttons:

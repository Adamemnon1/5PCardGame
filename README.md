# 5PCardGame
5 Card Stud poker game.

Five Card Draw

========================================================

========================================================

Adam Sandor

Brian Lam

Carlie Abraham

Note: Carlie Abraham wrote the CArd, Hand, and Deck files. We 
all have similar files to those from previous projects, but we 
chose to use hers for this project.
========================================================

Summary

========================================================

This program extends our Game class to implement Five Card

Draw. It can be run with multiple players, and gives them

a hand of cards from a standard deck of cards. It is run

with Lab3.exe



========================================================

Test Cases

========================================================



$    Lab3.exe FiveCardDraw Brian Carlie

The program runs Five Card Draw with two players (Brian and Carlie). The

game stops and the error level is 0 if a player leaves and there aren't

enough players int he game.



$    Lab3.exe Goats Brian Carlie

The program throws an error "Unknown Game" and sets the error level to

68.



$    Lab3.exe FiveCardDraw Adam Brian Carlie David Emma Fred George Harry Ivan Jack Karly Larry

The program throws an error that there aren't enough cards left in the deck (because there are 

12 players and a standard deck cannot hold enough cards to give each of them a hand). Error level 

is set to 455.



$    Lab3.exe FiveCardDraw Obama

The program throws an error message that there weren't enough arguments passed in and 

displays the usage message. Error level is set to 1. 




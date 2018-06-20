#!/usr/bin/python3
# Author: Claudio M Coppola
# Version: 1.0
# Description: There are 10 cards numbered 1 to 10, they are divided into two piles,
# so that the cards in Pile 0 sum to a number as close as possible to 36, and
# the remaining cards in Pile 1 multiply to a number as close as possible to 360.

import random

cards = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

print("The Card Problem")

solved = False
tries = 0

while solved is False:
    pile0Total = 0
    pile1Total = 1

    # Shuffle the deck of cards
    random.shuffle(cards)

    # The first half of the deck goes to Pile 0
    pile0 = cards[0:5]
    # The second half of the deck goes to Pile 1
    pile1 = cards[5:10]

    # Sum every number in Pile 0
    for num0 in pile0:
        pile0Total += num0

    # Multiply every number in Pile 1
    for num1 in pile1:
        pile1Total *= num1

    # Check that Pile 0 = 36 and Pile 1 = 360
    if pile0Total == 36 and pile1Total == 360:
        print("Pile 0 " + str(pile0) + " = " + str(pile0Total))
        print("Pile 1 " + str(pile1) + " = " + str(pile1Total))
        print("Attempts = " + str(tries))
        solved = True

    tries += 1






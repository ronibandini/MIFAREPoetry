# MIFARE Poetry generator
# Roni Bandini, August 2024
# MIT License

import random
import os
from art import *

os.system('cls')

print(text2art("MIFARE POETRY", font="small"))
print("Roni Bandini")
print("")


with open('nouns.txt', 'r') as f:
    nouns = []
    for noun in f:
        nouns.append(noun.strip())

with open('adjectives.txt', 'r') as f:
    adjectives = []
    for adjective in f:
        adjectives.append(adjective.strip())

with open('verbs.txt', 'r') as f:
    verbs = []
    for verb in f:
        verbs.append(verb.strip())

with open('adverbs.txt', 'r') as f:
    adverbs = []
    for adverb in f:
        adverbs.append(adverb.strip())

with open('gerunds.txt', 'r') as f:
    gerunds = []
    for gerund in f:
        gerunds.append(gerund.strip())


def getLine():

	myLen=17

	myTemplate=random.randint(1,5)

	if myTemplate==1:
		while myLen>16:
			myAdjective=random.choice(adjectives)
			myNoun=random.choice(nouns)
			myVar=random.randint(1,4)
			myLine=""
			if myVar==1:
				myLine="My "+myAdjective+" "+myNoun
			if myVar==2:
				myLine="Your "+myAdjective+" "+myNoun
			if myVar==3:
				myLine=myAdjective+" "+myNoun
			if myVar==4:
				myLine="("+myAdjective+" "+myNoun+")"


			myLen=len(myLine)
	if myTemplate==2:
		while myLen>16:
			myVerb=random.choice(verbs)
			myAdverb=random.choice(adverbs)
			myLine=myVerb+" "+myAdverb
			myLen=len(myLine)

	if myTemplate==3:
		while myLen>16:
			myGerund=random.choice(gerunds)
			myNoun=random.choice(nouns)
			myLine=myGerund+" "+myNoun
			myLen=len(myLine)

	if myTemplate==4:
		while myLen>16:
			myNoun1=random.choice(nouns)
			myNoun2=random.choice(nouns)	
			myNoun3=random.choice(nouns)	
			myVar=random.randint(1,3)
			if myVar==1:
				myLine=myNoun1+", "+myNoun2+", "+myNoun3
			if myVar==2:
				myLine=myNoun1+", "+myNoun2 
			if myVar==3:
				myLine=myNoun1+" ("+myNoun2+"?)" 

			myLen=len(myLine)

	if myTemplate==5:
		while myLen>16:
			myAdverb1=random.choice(adverbs)
			myLine=myAdverb1
			mySign=random.randint(1,4)

			if mySign==1:
				myLine=myLine+"?"
			if mySign==2:
				myLine=myLine+"!"
			if mySign==3:
				myLine=myLine+"..."

			myLen=len(myLine)

	return myLine

print(getLine())
print(getLine())
print(getLine())
print("")
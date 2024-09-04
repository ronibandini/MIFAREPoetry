# MIFARE Poetry generator
# Roni Bandini, August 2024
# MIT License
# pip install pyrhyme

import random
import os
from rhyme import rhymes_with

os.system('cls')

print("MIFARE Poetry v2")
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

def getLine1():
	global myWordToRhyme
	myLen=17
	myVar=random.randint(1,4)
	myLine=""
	myAdjective=random.choice(adjectives)
	myNoun1=random.choice(nouns)
	myNoun2=random.choice(nouns)

	while myLen>16:
		if myVar==1:
			myLine="My "+myAdjective+" "+myNoun1
		if myVar==2:
			myLine="Your " +myNoun1
		if myVar==3:
			myLine=myAdjective+" "+myNoun1
		if myVar==4:
			myLine=myNoun2+", "+myNoun1
		myLen=len(myLine)

	myWordToRhyme=myNoun1
	return myLine

def getLine2():

	myLen=17
	myVar=random.randint(1,4)
	myLine=""
	while myLen>16:
		myVerb=random.choice(verbs)
		myAdverb=random.choice(adverbs)
		myLine=myVerb+" "+myAdverb
		myLen=len(myLine)
	return myLine

def getLine3():

	global myWordToRhyme
	myLen=17
	myVar=random.randint(1,4)
	myLine=""
	myRhymedNouns=rhymes_with(myWordToRhyme)
	while myLen>16:
		myGerund=random.choice(gerunds)
		myNoun=random.choice(myRhymedNouns)
		myLine=myGerund+" "+myNoun
		myLen=len(myLine)

	return myLine

def getLine4():
	myLen=17
	while myLen>16:
		myAdverb1=random.choice(adverbs)
		myLine=myAdverb1
		mySign=random.randint(1,5)

		if mySign==1:
			myLine=myLine+"?"
		if mySign==2:
			myLine=myLine+"!"
		if mySign==3:
			myLine=myLine+"..."
		if mySign==4:
			myLine=myLine+"/"

		myLen=len(myLine)

	return myLine




print(getLine1())
print(getLine2())
print(getLine3())


print("")
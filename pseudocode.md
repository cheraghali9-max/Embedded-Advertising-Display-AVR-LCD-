# TODO
## How it supposed to work:
- 20 sec for every advert
- never use advert from the same customer twice
- a customer that pays 600 for advert should appear twice as often more than a customer that pays 300
- we should show advert for ourself

## pseudocode:
<input type=checkbox checked> return a random number between 0-4999 5000-7999 8000-11999 12000-13499 13500-14499
<br>
<input type=checkbox checked> check what advert to show
<br>
<input type=checkbox checked> if it already have been show before than return a random number else show current number
<br>
<input type=checkbox> display advert for 20 sec
<br>
<input type=checkbox checked> return and do it again

## implementing random function steps:
- research LCG linear congruential generator

## things to consider to add:
- Adjust brightness depending if it is night or day
- Add more customers if configure button is pressed for 3 second
- Add support for swedish characters
- add readme with explanation on how the program works

## Customer:
### Hederlige Harrys Bilar:
paid 5000. want to randomly show one of the three messages.
- "Köp bil hos Harry" (scroll)
- "En god bilaffär (för Harry!)" text
- "Hederlige Harrys Bilar" text (blinkande)
 
### Farmor Ankas Pajer AB:
paid 3000. want to randomly show one of the two messages.
- "Köp paj hos Farmor Anka"  (scroll)
- "Skynda innan Mårten ätit alla pajer" text
 
### Svarte Petters Svartbyggen:
paid 1500. want to show:
- "Låt Petter bygga åt dig"  (scroll) - på jämna minuter
- "Bygga svart? Ring Petter" text - på ojämna minuter
 
### Långbens detektivbyrå:
paid 4000. want to show:
- "Mysterier? Ring Långben"  text 
- "Långben fixar biffen" text 

### sometimes want to show advert for ourselves:
motsvarande för 1000 kr. 
Meddelande:
<br>
"Synas här? IOT:s Reklambyrå"
#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "lcd.h"
#include "uart.h"

// https://wokwi.com/projects/416241646559459329

// PORTB B (digital pin 8 to 13)
// C (analog input pins)
// PORTD D (digital pins 0 to 7)
#define LED_PIN 2
#define BUTTON_PIN 1

#define BIT_SET(a, b) ((a) |= (1ULL << (b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b)))) 

#define BUTTON_IS_CLICKED(PINB,BUTTON_PIN) !BIT_CHECK(PINB,BUTTON_PIN)

typedef enum
{
    harrys,
    Pajer_AB,
    detektivbyrå,
    Svartbyggen,
    buyAdvert,
}advert;

advert returnRandomAdvert();
void scrollText(char *text);

int main(void){
    HD44780 lcd;

    lcd.Initialize(); // Initialize the LCD
    lcd.Clear();      // Clear the LCD

    int lastAdvert = buyAdvert;

    while(1)
    {
        char result[15] = "result: ";

        int currentAdvert;
        do
        {
            currentAdvert = returnRandomAdvert();
        } while (lastAdvert == currentAdvert);
        lastAdvert = currentAdvert;

        switch (currentAdvert)
        {
            case harrys:
                strcat(result, "harrys");
                break;
            case Pajer_AB:
                strcat(result, "pajer_ab");
                break;
            case detektivbyrå:
                strcat(result, "detetivbyra");
                break;
            case Svartbyggen:
                strcat(result, "svartbyggen");
                break;
            case buyAdvert:
                strcat(result, "buy advert");
                break;
            default:
                printf("error novalue returned");
                break;
        }

        scrollText(result);
        _delay_ms(1000);
        lcd.Clear();
    }
    // // //Sätt till INPUT_PULLUP
    // BIT_CLEAR(DDRB,BUTTON_PIN); // INPUT MODE
    // BIT_SET(PORTB,BUTTON_PIN); 

    // DATA DIRECTION = avgör mode
    // om output så skickar vi  1 eller 0 på motsvarande pinne på PORT
    // om input så läser vi  1 eller 0 på motsvarande pinne på PIN
    //bool blinking = false;
    while(1){
    }
    return 0;
}

void scrollText(char *text)
{
    HD44780 lcd;

    lcd.Initialize(); // Initialize the LCD
    lcd.Clear();      // Clear the LCD
    
    for (int x = 16; x >= 0; x--)
    {
        char rText[20] = " ";
        strncpy(rText, text, 16-x);
        lcd.Clear();
        lcd.GoTo(x, 0);
        lcd.WriteText(rText);

        _delay_ms(500);
    }
    for (int index = 1; index <= (int)strlen(text); index++)
    {
        char rText[20] = " ";
        lcd.Clear();
        lcd.GoTo(0, 0);
        strncpy(rText, text + index, 16);
        lcd.WriteText(rText);

        _delay_ms(500);
    }
    
}

advert returnRandomAdvert()
{
    // time(null) will always return same value
    srand(time(NULL));
    int value = rand() % (14499 + 1);

    printf("%d ", value);
    
    if (value < 5000)
    { // Hederlige Harrys Bilar
        return harrys;
    }
    else if (value < 8000 && value >= 5000)
    { // Farmor Ankas Pajer AB
        return Pajer_AB;
    }
    else if (value < 12000 && value >= 8000)
    { // Långbens detektivbyrå
        return detektivbyrå;
    }
    else if (value < 13500 && value >= 12000)
    { // Svarte Petters Svartbyggen
        return Svartbyggen;
    }
    else
    {// our own advert
        return buyAdvert;
    }
}
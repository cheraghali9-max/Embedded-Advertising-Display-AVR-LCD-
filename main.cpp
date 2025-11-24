#define __AVR_ATmega32U4__
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <time.h>
#include<string.h>
#include <stdbool.h>
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
    harrys_bilar,
    Pajer_AB,
    Svartbyggen,
    långbens_Detektivbyrå,
    buyAdvert,
}advert;

typedef struct 
{
  const char* name;
  int payment;
  const char* messages[3];
  int messageCount;
  bool illegal;
} Advertiser;

Advertiser customers[] = {
    {"Harry bilar", 50 , {"Köp bill hos Harry",
                    "En god bil affär(för Harry!)",
                    "Hedelige Harrys Bilar"}, 3, false},

    {"Farmor Anka: Pajer AB", 30, {"Köp paj hos Farmor Anka",
                         "Skynda innan Mårten ätit alla pajer"}, 2, false},

    {"Svarte Petter Svartbyggen:", 15, {"Låt Petter bygga åt dig",
                    "Bygga svart? Ring Petter"}, 2, true},

    {"Långben Detektivbyrå", 40, {"Mysterier? Ring Långben",
                     "Långben fixar biffen"},2, false},

    {"IOT Reklambyrå", 10, {"Synas här? IOT:s Reklambyrå"}, 1, false},
};

const int customerCount = sizeof(customers)/sizeof(customers[0]);

advert returnRandomAdvert();
void scrollText(char *text);

//Enklare ADC noise läsning som ett random boost
unsigned int adc_noise() {
    ADMUX = 0b00001111; 
    ADCSRA = 0b11000011;
    _delay_ms(2);
    return ADC;
}

int main(void){
    init_serial();
    HD44780 lcd;

    lcd.Initialize(); // Initialize the LCD
    lcd.Clear();      // Clear the LCD

      srand( adc_noise() ); // För riktigt random fungerade

    int lastAdvert = buyAdvert;

    while(1)
    {
        char result[32] = "result: "; //Jag har ökat till dubbelt att unkvika overflow

        int currentAdvert;
        do
        {
            currentAdvert = returnRandomAdvert();
        } while (lastAdvert == currentAdvert);
        lastAdvert = currentAdvert;

        switch (currentAdvert)
        {
            case harrys_bilar:
                strcat(result, "harrys");
                break;
            case Pajer_AB:
                strcat(result, "pajer_ab");
                break;
            case långbens_Detektivbyrå:
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

void flashingText(char *text)
{
    HD44780 lcd;

    lcd.Initialize();
    lcd.Clear();

    for (int index = 0; index < 10; index++)
    {
        lcd.WriteText(text);
        _delay_ms(500);
        lcd.Clear();
        _delay_ms(500);
    }
}

advert returnRandomAdvert()
{
    // time(null) will always return same value
    //srand(time(NULL)); Fel att använda den för att den returnerar samma värde varje gång. Det blir inte slump
    int value = rand() % (14499 + 1);

    printf("%d ", value);
    
    if (value < 5000)
    { // Hederlige Harrys Bilar
        return harrys_bilar;
    }
    else if (value < 8000 && value >= 5000)
    { // Farmor Ankas Pajer AB
        return Pajer_AB;
    }
    else if (value < 12000 && value >= 8000)
    { // Långbens detektivbyrå
        return långbens_Detektivbyrå;
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
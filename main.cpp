#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
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

//Struktur för kunderna
typedef struct 
{
    const char* name;
    int percentage;
    const char* messages[3];
    int messageCount;
    bool specialRules;
} Advertiser;

// Kunder, procentvis och deras meddelanden
Advertiser customers[] = {
    {"Harry", 50 , {"Köp bill hos Harry", "En god bil affär(för Harry!)","Hedelige Harrys Bilar"}, 3, false};
    {"Farmor Anak", 30, {"Köp paj hos Farmor ANka", "Skynda innan Mårten ätit alla pajer"}, 2, false};
    {"Ptter", 15, {"Låt Ptter bygga åt dig", "Bygga svart? Ring Petter"}, 2, true};
    {"Långben", 40, {"Mysterier? Ring Lånben", "Långben fixar biffen"},2, false};
    {"IOT Reklambyrå", 10, {"Synas här? IOT:s Reklambyrå"}, 1, false};
};
int customerCount = 5;

int lastCustomerIndex = -1; // Att inte räkan en kund 2 gångar i rad

// Slumpa kunderna procentuellt baserad på deras belopp

int pickCustomer() {
    int totalVikt = 0;
      int weights[5];

    for (int i = 0; i < customerCount; i++) {
        if (i == lastCustomerIndex) weights[i] = 0;
        else weights[i] = customers[i].percentage;
        totalVikt += weights[i];
}

 int rnd = rand() % totalVikt;
    for (int i = 0; i < customerCount; i++) {
        if (rnd < weights[i]) return i;
        rnd -= weights[i];
    }

    return 0;
} 

// Funktion att visa meddelande på LCDn????

 void showAd (HD44780* lcd, Advertiser* A) {

    //Kund med specila regler
    if (A->specialRules){
        int currentMinute = (rand() %60);
        lcd ->Clear();
        if (currentMinute % == 0) //Jämn och ljämna minuter
        
            lcd->WriteText((char*)A->messages[0]); //jämn = scroll

        else
            lcd->WriteText((char*)A->messages[1]); //ojämn = text

        return;
        
    }
    //Normala kunder utan speciala regler
    int msgIndeX = rand() % A->messageCount;
    lcd->Clear();
    lcd->WriteText((char*)A->messages[msgIndeX]);
 }

int main(void){
    init_serial();
    HD44780 lcd;

    lcd.Initialize(); // Initialize the LCD
    lcd.Clear();      // Clear the LCD

    lcd.WriteText((char *)"Hej hej");
    printf("Hej hej\n");
    int r = 12;
    printf("Hej 2 %d\n",r);
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

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "lcd.h"
#include "uart.h"

//static volatile uint32_t g_millis = 0u;

// Advert messages defined as static char arrays since we don't have access to
// standard cpp.

void scrollText(char *text, size_t duration_ms);
void flashingText(char *text, size_t duration_ms);

class Display {
private:
  HD44780 lcd;

public:
  Display() { lcd.Initialize(); }

  void showMessage(const char *message) {
    lcd.Clear();
    lcd.WriteText((char *)message);
  }
};

static Display display;
static volatile bool running = true;
void requestClose() { running = false; }

// Effect types: 0 = normal display, 1 = scrolling, 2 = flashing
enum EffectType { NORMAL = 0, SCROLLING = 1, FLASHING = 2 };

struct Message {
  const char *text;
  EffectType effect;
  size_t duration_ms;

  Message(const char *t, EffectType e = NORMAL, size_t d = 3000) : text(t), effect(e), duration_ms(d) {}
};

class Advert {
private:
  const char *company_name;
  const size_t payment_amount;
  const Message *messages;
  size_t message_count;

public:
  Advert(const char *customer_name, const size_t payment_amount,
         const Message *msg_array, size_t message_count)
      : company_name(customer_name), payment_amount(payment_amount),
        messages(msg_array), message_count(message_count) {}
  void advertise() {
    printf("Advertizing for %s\n", company_name);
    
//Specialreger för Svartbyggen. Jämn och ojämn minuter.

// if (strcmp(company_name, "Svarte Petters Svartbyggen") == 0) {
//     // Använd millis()-baserad minut, robust och AVR-kompatibel
//     uint8_t minute = (uint8_t)((millis_ms() / 60000u) % 60u);

//     const Message &msg = (minute % 2 == 0) ? messages[0] : messages[1];

//     if (msg.effect == SCROLLING) {
//         scrollText((char*)msg.text, msg.duration_ms);
//     } else if (msg.effect == FLASHING) {
//         flashingText((char*)msg.text, msg.duration_ms);
//     } else {
//         display.showMessage(msg.text);
//         sleep_ms(msg.duration_ms);
//     }

//     printf("Finished advertizing for %s\n", company_name);
//     return;
// }

    //Regler för andra ordinarie kunder 
    const Message &msg = messages[rand() % message_count];

    // Apply the chosen effect for this specific message and honor its duration
    if (msg.effect == SCROLLING) {
      scrollText((char *)msg.text, msg.duration_ms);
    } else if (msg.effect == FLASHING) {
      flashingText((char *)msg.text, msg.duration_ms);
    } else {
      display.showMessage(msg.text);
      // keep displayed for the requested duration
      for (size_t t = 0; t < msg.duration_ms && running; ++t) {
        _delay_ms(1);
      }
    }

    printf("Finished advertizing for %s\n", company_name);
  }
};

void init() { init_serial(); }

// Helper: sleep for up to `ms` milliseconds, checking `running`.
static void sleep_ms(size_t ms) {
  for (size_t i = 0; i < ms && running; ++i) {
  _delay_ms(1);
  }
}

void scrollText(char *text, size_t duration_ms)
{
  HD44780 lcd;

  lcd.Initialize(); // Initialize the LCD
  lcd.Clear();      // Clear the LCD

  size_t elapsed = 0;
  const uint16_t frame = 250; // ms per frame
  int textlen = (int)strlen(text);
  const int displayWidth = 16;

  // Loop performing scrolling frames until duration reached
  while (elapsed < duration_ms && running) {
    for (int x = displayWidth; x >= -textlen && elapsed < duration_ms && running; --x) {
      char rText[20];
      memset(rText, 0, sizeof(rText));

      if (x >= 0) {
        int len = displayWidth - x;
        if (len > 0) {
          strncpy(rText, text, (size_t)len);
        }
        lcd.Clear();
        lcd.GoTo(x, 0);
        lcd.WriteText(rText);
      } else {
        int start = -x;
        strncpy(rText, text + start, displayWidth);
        lcd.Clear();
        lcd.GoTo(0, 0);
        lcd.WriteText(rText);
      }

      sleep_ms(frame);
      elapsed += frame;
    }
  }

  lcd.Clear();
}

void flashingText(char *text, size_t duration_ms)
{
  HD44780 lcd;

  lcd.Initialize();
  lcd.Clear();

  size_t elapsed = 0;
  const uint16_t half = 400; // ms on / off

  while (elapsed < duration_ms && running) {
    lcd.WriteText(text);
    sleep_ms(half);
    elapsed += half;
    if (elapsed >= duration_ms || !running) break;
    lcd.Clear();
    sleep_ms(half);
    elapsed += half;
  }

  lcd.Clear();
}

static const Message messages0[] = {
    Message("Kop bil hos Harry", SCROLLING, 20000),
    Message("En god bilaffar (for Harry!)", NORMAL, 20000),
    Message("Hederlige Harrys Bilar", FLASHING, 20000)
};

static const Message messages1[] = {
    Message("Kop paj hos Farmor Anka", SCROLLING, 20000),
    Message("Skynda innan Marten atit alla pajer", NORMAL, 20000)
};

static const Message messages2[] = {
    Message("Lat Petter bygga at dig", SCROLLING, 20000),
    Message("Bygga svart? Ring Petter", NORMAL, 20000)
};

static const Message messages3[] = {
    Message("Mysterier? Ring Langben", NORMAL, 20000),
    Message("Langben fixar biffen", NORMAL, 20000)
};
static const Message messages4[] = {
    Message("Synas här? IOT:s Reklambyrå",NORMAL, 20000),
    };


static Advert adverts[] = {
    Advert("Hederlige Harrys Bilar", 5000, messages0,
           sizeof(messages0) / sizeof(messages0[0])),
    Advert("Farmor Ankas Pajer AB", 3000, messages1,
           sizeof(messages1) / sizeof(messages1[0])),
    Advert("Svarte Petters Svartbyggen", 1500, messages2,
           sizeof(messages2) / sizeof(messages2[0])),
    Advert("Langbens detektivbyra", 4000, messages3,
           sizeof(messages3) / sizeof(messages3[0])),
    Advert("IOT Raklambyrå", 1000, messages4,
           sizeof(messages4) / sizeof(messages4[0])),
};

static volatile size_t lastAdvertIndex = (size_t)-1;

void loop() {
  size_t advertCount = sizeof(adverts) / sizeof(adverts[0]);
  size_t index = rand() % advertCount;
  if (index == lastAdvertIndex) {
    index = (index + 1) % advertCount;
  }

  lastAdvertIndex = index;

  adverts[index].advertise();
}

int main() {
  init();
  //timer0_millis_init();   // Initialize millis timer

  while (running)
    loop();

  return 0;
}
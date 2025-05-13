#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Button Pins
#define BUTTON_UP     14
#define BUTTON_DOWN   13
#define BUTTON_SELECT 26

// Relay Pin
#define RELAY_PIN     25

// EEPROM
#define EEPROM_SIZE   4
#define RELAY_ADDR    1

enum MenuState {MAIN_MENU, RELAY_MENU, SETTINGS_MENU};
MenuState currentMenu = MAIN_MENU;

const char* mainMenu[] = {"Relay Menu", "Settings"};
int mainMenuCount = sizeof(mainMenu) / sizeof(mainMenu[0]);

const char* relayMenu[] = {"Relay ON", "Relay OFF","Back"};
int relayMenuCount = sizeof(relayMenu) / sizeof(relayMenu[0]);

const char* settingsMenu[] = {"Brightness", "Theme","Back"};
int settingsMenuCount = sizeof(settingsMenu) / sizeof(settingsMenu[0]);

int currentIndex = 0;
int scrollY = 0;
int lineHeight = 20;

void setup() {
  Serial.begin(115200);

  // OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED not found");
    while(1);
  }

  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("Loading...");
  display.display();

  // Buttons
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_SELECT, INPUT_PULLUP);

  // Relay
  pinMode(RELAY_PIN, OUTPUT);

  // EEPROM
  EEPROM.begin(EEPROM_SIZE);
  int relayState = EEPROM.read(RELAY_ADDR);
  digitalWrite(RELAY_PIN, relayState);
}

void loop() {
  showMenu(scrollY);

  if (digitalRead(BUTTON_UP) == LOW) {
    int prevIndex = currentIndex;
    currentIndex = (currentIndex - 1 + getMenuCount()) % getMenuCount();
    delay(200);
    animateScroll(prevIndex, currentIndex);
    waitForRelease(BUTTON_UP);
  }

  if (digitalRead(BUTTON_DOWN) == LOW) {
    int prevIndex = currentIndex;
    currentIndex = (currentIndex + 1) % getMenuCount();
    delay(200);
    animateScroll(prevIndex, currentIndex);
    waitForRelease(BUTTON_DOWN);
  }

  if (digitalRead(BUTTON_SELECT) == LOW) {
    handleSelection(currentIndex);
    delay(500);
    waitForRelease(BUTTON_SELECT);
  }
}

void showMenu(int scrollY) {
  display.clearDisplay();
  display.setTextSize(2);
  const char** menu = getMenuArray();

  for (int i = 0; i < getMenuCount(); i++) {
    int y = i * lineHeight - scrollY;
     if (y >= -lineHeight && y < display.height()){
        if (i == currentIndex) {
          display.fillRect(0, y, SCREEN_WIDTH, 20, WHITE);
          display.setTextColor(BLACK);
        } else {
          display.setTextColor(WHITE);
        }
        display.setCursor(5, y + 2);
        display.println(menu[i]);
     }
  }
  display.display();
}


void animateScroll(int fromIndex, int toIndex) {
  int direction = (toIndex > fromIndex || (fromIndex == getMenuCount() - 1 && toIndex == 0)) ? 1 : -1;

  for (int offset = 0; offset <= lineHeight; offset += 2) {
    scrollY = fromIndex * lineHeight + offset * direction;
    showMenu(scrollY);
    delay(10);
  }

  scrollY = toIndex * lineHeight;
}

void handleSelection(int index) {
  if(currentMenu == MAIN_MENU){
    if(index == 0){
      currentMenu = RELAY_MENU;
    }
    else if(index == 1){
      currentMenu = SETTINGS_MENU;
    }
    currentIndex = 0;
    scrollY = 0;
  }
  else if(currentMenu == RELAY_MENU){
    if(index == 0) {
      digitalWrite(RELAY_PIN, LOW);
      EEPROM.write(RELAY_ADDR, LOW);
      EEPROM.commit();
      showMessage("Relay ON");
    } else if(index == 1) {
      digitalWrite(RELAY_PIN, HIGH);
      EEPROM.write(RELAY_ADDR, HIGH);
      EEPROM.commit();
      showMessage("Relay OFF");
    }else if(index == 2){
      currentMenu = MAIN_MENU;
      currentIndex = 0;
      scrollY = 0;
    }
  }
  else if (currentMenu == SETTINGS_MENU) {
    if (index == 0) {
      showMessage("Brightness");
    } else if (index == 1) {
      showMessage("Theme");
    } else if (index == 2) { // Back
      currentMenu = MAIN_MENU;
      currentIndex = 0;
      scrollY = 0;
    }
  }
}
 
  // Show confirmation
void showMessage(const char* msg){
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1.5);
  display.setCursor(0, 0);
  display.println(msg);
  display.display();
  delay(1500);
}

void waitForRelease(int pin) {
  while (digitalRead(pin) == LOW) {
    delay(10);
  }
}

const char** getMenuArray() {
  switch (currentMenu) {
    case RELAY_MENU: return relayMenu;
    case SETTINGS_MENU: return settingsMenu;
    default: return mainMenu;
  }
}

int getMenuCount() {
  switch (currentMenu) {
    case RELAY_MENU: return relayMenuCount;
    case SETTINGS_MENU: return settingsMenuCount;
    default: return mainMenuCount;
  }
}

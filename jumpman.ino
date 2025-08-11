#include <LiquidCrystal.h>

int buttonPin = 2;
int autoplayPin = 1;
int readWritePin = 10;
int contrastPin = 12;

const byte spriteRun1 = 1;
const byte spriteRun2 = 2;
const byte spriteJump = 3;
const byte spriteJumpUpper = '.';
const byte spriteJumpLower = 4;
const byte spriteEmptyTerrain = ' ';
const byte spriteSolidTerrain = 5;
const byte spriteSolidTerrainRight = 6;
const byte spriteSolidTerrainLeft = 7;

const byte heroXPos = 1;

const byte terrainWidth = 16;
const byte terrainEmpty = 0;
const byte terrainLowBlock = 1;
const byte terrainUpBlock = 2;

const byte heroOff = 0;
const byte heroLowPosition1 = 1;
const byte heroLowPosition2 = 2;
const byte heroJumpPosition1 = 3;
const byte heroJumpPosition2 = 4;
const byte heroJumpPosition3 = 5;
const byte heroJumpPosition4 = 6;
const byte heroJumpPosition5 = 7;
const byte heroJumpPosition6 = 8;
const byte heroJumpPosition7 = 9;
const byte heroJumpPosition8 = 10;
const byte heroUpperRunPosition1 = 11;
const byte heroUpperRunPosition2 = 12;

LiquidCrystal lcd(11, 9, 6, 5, 4, 3);
static char terrainUp[terrainWidth + 1];
static char terrainLow[terrainWidth + 1];
static bool buttonPushed = false;

void initializeGraphics() {

  static byte graphics[] = {

    // Run position 1
    B01100, B01100, B00000, B01110, B11100, B01100, B11010, B10011,

    // Run position 2
    B01100, B01100, B00000, B01100, B01100, B01100, B01100, B01110,

    // Jump position
    B01100, B01100, B00000, B11110, B01101, B11111, B10000, B00000,

    // Lower Jump Position
    B11110, B01101, B11111, B10000, B00000, B00000, B00000, B00000,

    // Ground
    B11111, B11111, B11111, B11111, B11111, B11111, B11111, B11111,

    // Ground Right
    B00011, B00011, B00011, B00011, B00011, B00011, B00011, B00011,

    // Ground left
    B11000, B11000, B11000, B11000, B11000, B11000, B11000, B11000
  };

  for (int i = 0; i < 7; ++i) {
    lcd.createChar(i+1, &graphics[i*8]);
  }

  for (int i = 0; i < terrainWidth; ++i) {
    terrainUp[i] = spriteEmptyTerrain;
    terrainLow[i] = spriteEmptyTerrain;
  }

}


void advanceTerrain(char* terrain, byte newTerrain) {
  for (int i = 0; i < terrainWidth; ++i) {
    char current = terrain[i];
    char next;
    if (i == terrainWidth - 1) {
      next = newTerrain;
    } else {
      next = terrain[i + 1];
    }

    if (current == spriteEmptyTerrain) {
      if (next == spriteSolidTerrain) {
        terrain[i] = spriteSolidTerrainRight;
      } else {
        terrain[i] = spriteEmptyTerrain;
      }
    } else if (current == spriteSolidTerrain) {
      if (next == spriteEmptyTerrain) {
        terrain[i] = spriteSolidTerrainLeft;
      } else {
        terrain[i] = spriteSolidTerrain;
      }
    } else if (current == spriteSolidTerrainRight) {
      terrain[i] = spriteSolidTerrain;
    } else if (current == spriteSolidTerrainLeft) {
      terrain[i] = spriteEmptyTerrain;
    }
  }
}

bool drawHero(byte position, char* terrainUpper, char* terrainLower, unsigned int score) {
  bool collide = false;
  char upperSave = terrainUpper[heroXPos];
  char lowerSave = terrainLower[heroXPos];
  byte upper, lower;

  if (position == heroOff) {
    upper = lower = spriteEmptyTerrain;
  } else if (position == heroLowPosition1) {
    upper = spriteEmptyTerrain;
    lower = spriteRun1;
  } else if (position == heroLowPosition2) {
    upper = spriteEmptyTerrain;
    lower = spriteRun2;
  } else if (position == heroJumpPosition1 || position == heroJumpPosition8) {
    upper = spriteEmptyTerrain;
    lower = spriteJump;
  } else if (position == heroJumpPosition2 || position == heroJumpPosition7) {
    upper = spriteJumpUpper;
    lower = spriteJumpLower;
  } else if (position == heroJumpPosition3 || position == heroJumpPosition4 || position == heroJumpPosition5 || position == heroJumpPosition6) {
    upper = spriteJump;
    lower = spriteEmptyTerrain;
  } else if (position == heroUpperRunPosition1) {
    upper = spriteRun1;
    lower = spriteEmptyTerrain;
  } else if (position == heroUpperRunPosition2) {
    upper = spriteRun2;
    lower = spriteEmptyTerrain;
  }

  if (upper != ' ') {
    terrainUpper[heroXPos] = upper;
    if (upperSave != spriteEmptyTerrain) {
      collide = true;
    }
  }

  if (lower != ' ') {
    terrainLower[heroXPos] = lower;
    if (lowerSave != spriteEmptyTerrain) {
      collide = true;
    }
  }

  byte digits;
  if (score > 9999) {
    digits = 5;
  } else if (score > 999) {
    digits = 4;
  } else if (score > 99) {
    digits = 3;
  } else if (score > 9) {
    digits = 2;
  } else {
    digits = 1;
  }

  terrainUpper[terrainWidth] = '\0';
  terrainLower[terrainWidth] = '\0';
  char temp = terrainUpper[16 - digits];
  terrainUpper[16 - digits] = '\0';

  lcd.setCursor(0,0);
  lcd.print(terrainUpper);
  terrainUpper[16 - digits] = temp;

  lcd.setCursor(0, 1);
  lcd.print(terrainLower);

  lcd.setCursor(16 - digits, 0);
  lcd.print(score);

  terrainUpper[heroXPos] = upperSave;
  terrainLower[heroXPos] = lowerSave;

  return collide;
}

void buttonPush() {
  buttonPushed = true;
}

void setup() {
  pinMode(readWritePin, OUTPUT);
  digitalWrite(readWritePin, LOW);
  pinMode(contrastPin, OUTPUT);
  digitalWrite(contrastPin, LOW);
  pinMode(buttonPin, INPUT);
  digitalWrite(buttonPin, HIGH);
  pinMode(autoplayPin, OUTPUT);
  digitalWrite(autoplayPin, HIGH);

  attachInterrupt(0, buttonPush, FALLING);

  initializeGraphics();
  lcd.begin(16, 2);
}

void loop() {
  static byte heroPos = heroLowPosition1;
  static byte newTerrainType = terrainEmpty;
  static byte newTerrainDuration = 1;
  static bool playing = false;
  static bool blink = false;
  static unsigned int distance = 0;

  if (!playing) {
    if (blink) {
      drawHero(heroOff, terrainUp, terrainLow, distance >> 3);
    } else {
      drawHero(heroPos, terrainUp, terrainLow, distance >> 3);
    }

    if (blink) {
      lcd.setCursor(0, 0);
      lcd.print("Press Start");
    }

    delay(250);
    blink = !blink;

    if (buttonPushed) {
      initializeGraphics();
      heroPos = heroLowPosition1;
      playing = true;
      buttonPushed = false;
      distance = 0;
    }
    return;
  }

  // Shift terrain
  if (newTerrainType == terrainLowBlock) {
    advanceTerrain(terrainLow, spriteSolidTerrain);
  } else {
    advanceTerrain(terrainLow, spriteEmptyTerrain);
  }

  if (newTerrainType == terrainUpBlock) {
    advanceTerrain(terrainUp, spriteSolidTerrain);
  } else {
    advanceTerrain(terrainUp, spriteEmptyTerrain);
  }

  // Make new terrain
  newTerrainDuration--;
  if (newTerrainDuration == 0) {
    if (newTerrainType == terrainEmpty) {
      if (random(3) == 0) {
        newTerrainType = terrainUpBlock;
      } else {
        newTerrainType = terrainLowBlock;
      }
      newTerrainDuration = 2 + random(10);
    } else {
      newTerrainType = terrainEmpty;
      newTerrainDuration = 10 + random(10);
    }
  }

  // Handle jump
  if (buttonPushed) {
    if (heroPos <= heroLowPostion2) {
      heroPos = heroJumpPosition1;
    }
    buttonPushed = false;
  }

  // Draw hero and check for collision
  if (drawHero(heroPos, terrainUp, terrainLow, distance >> 3)) {
    playing = false; /`/ Collision detected
  } else {
    if (heroPos == heroLowPostion2 || heroPos == heroJumpPosition8) {
      heroPos = heroLowPosition1;
    } else if ((heroPos >= heroJumpPosition3 && heroPos <= heroJumpPosition5) &&
               terrainLow[heroXPos] != spriteEmptyTerrain) {
      heroPos = heroUpperRunPosition1;
    } else if (heroPos >= heroUpperRunPosition1 &&
               terrainLow[heroXPos] == spriteEmptyTerrain) {
      heroPos = heroJumpPosition5;
    } else if (heroPos == heroUpperRunPosition2) {
      heroPos = heroUpperRunPosition1;
    } else {
      heroPos++;
    }

    distance++;

    if (terrainLow[heroXPos + 2] == spriteEmptyTerrain) {
      digitalWrite(autoplayPin, HIGH);
    } else {
      digitalWrite(autoplayPin, LOW);
    }
  }

  delay(50);
}
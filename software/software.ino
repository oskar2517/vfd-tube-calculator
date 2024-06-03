#define TUBE_COUNT 5
#define DISPLAY_MAX (pow(10, TUBE_COUNT) - 1)
#define CAN_ENTER_NUMBER(reg) ((reg) * 10 < DISPLAY_MAX)

#define KEY_ROWS 4
#define KEY_COLS 4

#define PIN_E1 7
#define PIN_E2 8
#define PIN_E3 9
#define PIN_E4 10
#define PIN_E5 11

#define PIN_SA 1
#define PIN_SB 0
#define PIN_SC 2
#define PIN_SD 3
#define PIN_SE 4
#define PIN_SF 5
#define PIN_SG 6

char segmentPins[7] = {PIN_SA, PIN_SB, PIN_SC, PIN_SD, PIN_SE, PIN_SF, PIN_SG};
char enablePins[5] = {PIN_E1, PIN_E2, PIN_E3, PIN_E4, PIN_E5};

char keys[KEY_ROWS][KEY_COLS] = {
  {'7', '8', '9', '*'},
  {'4', '5', '6', '+'},
  {'1', '2', '3', '/'},
  {'0', 'C', '-', '='}
};

unsigned int digitSegments[10] = {
  0b1111110, // 0
  0b0110000, // 1
  0b1101101, // 2
  0b1111001, // 3
  0b0110011, // 4
  0b1011011, // 5
  0b1011111, // 6
  0b1110000, // 7
  0b1111111, // 8
  0b1111011  // 9
};

unsigned int errorSegments[TUBE_COUNT] = {
  0b0000101, // r
  0b0011101, // o
  0b0000101, // r
  0b0000101, // r
  0b1001111, // E
};

char currentKey = '\0';
long display = 0;

long registerA = 0;
long registerB = 0;
char mathOperator = '\0';
bool shouldClear = false;
bool error = false;

char keyRowPins[KEY_ROWS] = {12, 13, 14, 15};
char keyColPins[KEY_COLS] = {19, 18, 17, 16};

void resetCalculator() {
  resetRegisterA();
  resetRegisterB();
  resetOperator();
  resetShouldClear();
  resetError();
}

void resetRegisterA() {
  registerA = 0;
}

void resetRegisterB() {
  registerB = 0;
}

void resetOperator() {
  mathOperator = '\0';
}

void resetShouldClear() {
  shouldClear = false;
}

void resetError() {
  error = false;
}

void performCalculation() {
  if (mathOperator == '\0') {
    return;
  }

  switch (mathOperator) {
    case '+':
      registerA += registerB;
      break;
    
    case '-':
      registerA -= registerB;
      break;

    case '*':
      registerA *= registerB;
      break;

    case '/':
      if (registerB == 0) {
        error = true;
      } else {
        registerA /= registerB;
      }
      break;
  }

  if (registerA > DISPLAY_MAX) {
    registerA = DISPLAY_MAX;
  }

  resetRegisterB();
  resetOperator();
  shouldClear = true;
}

void readKey() {
  static bool newKeyRead = false;
  static bool canPressKey = true;
  bool keyPressed = false;

  if (newKeyRead) {
    currentKey = '\0';
    newKeyRead = false;
  }

  for (int i = 0; i < KEY_ROWS; i++) {
    digitalWrite(keyRowPins[i], LOW);
    
    for (int j = 0; j < KEY_COLS; j++) {
      if (digitalRead(keyColPins[j]) == LOW) {
        if (canPressKey) {
          currentKey = keys[i][j];
          newKeyRead = true;
        }
        keyPressed = true;
      }
    }

    digitalWrite(keyRowPins[i], HIGH);
  }

  canPressKey = !keyPressed;
}

long longPow(int a, int b) {
  return (long) (pow((double) a, (double) b) + 0.5);
}

void displayDigit() {
  static int currentDigit = 0;

  if (--currentDigit < 0) {
    currentDigit = TUBE_COUNT - 1;
  }

  bool negative = display < 0;

  long digit = ((long) ((negative ? -display : display) / longPow(10, currentDigit))) % 10;

  unsigned int segments = digitSegments[digit];
  if (error) {
    segments = errorSegments[currentDigit];
  }

  char currentTube = enablePins[currentDigit];

  digitalWrite(currentTube, HIGH);

  if (negative && currentDigit == TUBE_COUNT - 1) {
    digitalWrite(PIN_SG, HIGH);
  } else {
    for (char bit = 6; bit >= 0; --bit) {
      char enabled = ((segments >> bit) & 1);

      digitalWrite(segmentPins[6 - bit], enabled);
    }
  }

  delay(1);

  for (char i = 0; i < 7; i++) {
    digitalWrite(segmentPins[i], LOW);
  }

  digitalWrite(currentTube, LOW);
}

void setup() {
  for (char i = 0; i < KEY_ROWS; i++) {
    pinMode(keyRowPins[i], OUTPUT);
    digitalWrite(keyRowPins[i], HIGH);
  }

  for (char i = 0; i < KEY_COLS; i++) {
    pinMode(keyColPins[i], INPUT_PULLUP);
  }

  pinMode(PIN_E1, OUTPUT);
  pinMode(PIN_E2, OUTPUT);
  pinMode(PIN_E3, OUTPUT);
  pinMode(PIN_E4, OUTPUT);
  pinMode(PIN_E5, OUTPUT);

  pinMode(PIN_SA, OUTPUT);
  pinMode(PIN_SB, OUTPUT);
  pinMode(PIN_SC, OUTPUT);
  pinMode(PIN_SD, OUTPUT);
  pinMode(PIN_SE, OUTPUT);
  pinMode(PIN_SF, OUTPUT);
  pinMode(PIN_SG, OUTPUT);  

  digitalWrite(PIN_E1, LOW);
  digitalWrite(PIN_E2, LOW);
  digitalWrite(PIN_E3, LOW);
  digitalWrite(PIN_E4, LOW);
  digitalWrite(PIN_E5, LOW);

  digitalWrite(PIN_SA, LOW);
  digitalWrite(PIN_SB, LOW);
  digitalWrite(PIN_SC, LOW);
  digitalWrite(PIN_SD, LOW);
  digitalWrite(PIN_SE, LOW);
  digitalWrite(PIN_SF, LOW);
  digitalWrite(PIN_SG, LOW);
}

void loop() {
  displayDigit();
  readKey();

  if (currentKey == '\0') {
    return;
  }

  switch (currentKey) {
    case 'C':
      resetCalculator();
      display = 0;
      break;

    case '+':
    case '-':
    case '*':
    case '/':
      performCalculation();
      display = registerA;
      
      mathOperator = currentKey;
      shouldClear = false;
      break;

    case '=':
      performCalculation();
      display = registerA;
      break;

    default:
      if (currentKey < '0' && currentKey > '9') {
        return;
      }
      
      if (shouldClear) {
        resetCalculator();
      }

      char number = currentKey - 0x30;

      if (mathOperator == '\0') {
        if (CAN_ENTER_NUMBER(registerA)) {
          registerA = registerA * 10 + number;
          display = registerA;
        }
      } else {
        if (CAN_ENTER_NUMBER(registerB)) {
          registerB = registerB * 10 + number;
          display = registerB;
        }
      }
  }
}
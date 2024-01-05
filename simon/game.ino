// Simon Two

/* Buzzer */
const int BUZZER = 2;

const int BUZZER_DELAY_TIME_MS = 500;

/* Tones */
const unsigned int TONE_A4 = 440;
const unsigned int TONE_B4 = 494;
const unsigned int TONE_C4 = 523;
const unsigned int TONE_D4 = 587;
const unsigned int TONE_E4 = 659;

/* Extra notes */
const unsigned int C = 523;
const unsigned int D = 587;
const unsigned int E = 659;
const unsigned int F = 698;
const unsigned int G = 784;
const unsigned int A = 880;
const unsigned int B = 988;
const unsigned int TrebleDo = 1047;

/* Buttons */
const int INPUTS = A1;

/* LEDs */
// const int LIGHT_RED = 7;
// const int LIGHT_YEL = 6;
// const int LIGHT_GRE = 5;
// const int LIGHT_BLU = 4;

// CLASS
const int LIGHT_RED = 6;
const int LIGHT_YEL = 7;
const int LIGHT_GRE = 8;
const int LIGHT_BLU = 9;

// // From left -> Right
// const int SIGNAL_RANGES[4][2] {
//   // { Lower limit, Upper limit }
//   { 970 ,990 }, // Red
//   { 480 ,520 }, // Yellow
//   { 175 ,210 }, // Green
//   { 75 ,100 } // Blue
// };

// Class
const int SIGNAL_RANGES[4][2] = {
  { 110, 130 },
  { 305, 320 },
  { 775, 790 },
  { 1010, 1023 }
};

/* Sequence */
const int SEQUENCE_LENGTH = 10;

int SEQUENCE[10];
int USER_SEQUENCE[10];

/* Game states */
bool isFirstRun = true;

bool hasWon = false;
int iteration = 0;
int score = 0;

void setup() {
  /* Init serial communications */
  Serial.begin(9600);

  /* Init buzzer */
  pinMode(BUZZER, OUTPUT);

  /* Init buttons */
  pinMode(INPUTS, INPUT);

  /* Init lights */
  pinMode(LIGHT_RED, OUTPUT);
  pinMode(LIGHT_YEL, OUTPUT);
  pinMode(LIGHT_GRE, OUTPUT);
  pinMode(LIGHT_BLU, OUTPUT);
}

void loop() {
  // // Startup sequence
  // if ( isFirstRun ) {
  //   generateSequence();

  //   startupSequence();
  //   printSequence();

  //   isFirstRun = false;
  // }

  // if (hasWon) {
  //   Serial.println("\n\nCongrats, you've won!\n\n");
  //   victorySequence();
  // }
  // else {
  //   // Update iteration
  //   iteration++;
  //   Serial.println("\n\nITERATION: ");
  //   Serial.print(iteration);

  //   // Play light sequence
  //   playSequence();

  //   // Poll buttons
  //   pollButtons();

  //   // Check user input
  //   bool passed = checkInputs();

  //   // Play respective output
  //   if (passed) {
  //     correctSequence();
  //   }
  //   else {
  //     failSequence();
  //   }
  // }
  music();
}

void generateSequence() {
  String key = "generateSequence: ";

  srand(14134155324 + millis());

  cprint(key, "Generating sequence...");

  for (int i = 0; i < 10; i++) {
    SEQUENCE[i] = rand() % 4;
  }

  cprint(key, "Sequence generated");
}

void playSequence() {
  String key = "playSequence: ";

  // Iterate through the sequence based on how
  // many iterations we are on
  for (int i = 0; i < iteration; i++) {
    if (SEQUENCE[i] == 0) {
      playButtonLight(LIGHT_RED, TONE_A4, 500);
    }
    else if (SEQUENCE[i] == 1) {
      playButtonLight(LIGHT_YEL, TONE_B4, 500);
    }
    else if (SEQUENCE[i] == 2) {
      playButtonLight(LIGHT_GRE, TONE_C4, 500);
    }
    else if (SEQUENCE[i] == 3) {
      playButtonLight(LIGHT_BLU, TONE_D4, 500);
    }
    else {
      cprint(key, "!! Playing sequence failed !!");
    }

    noLightSound();
    delay(200);
  }

  return;
}

void pollButtons() {
  String key = "pollButtons: ";

  int buttonLightDelayMS = 100;
  int selectedButton = -1;

  // We poll for however many iterations we are 
  // on (E.g. if we are on iteration 2, we need 
  // to take 2 inputs)
  for (int i = 0; i < iteration; i++) {
    cprint(key, "Polling buttons on ");
    // Continue to poll until we get a button
    while (true) {
      int signal = analogRead(INPUTS);

      // Red
      if (signal >= SIGNAL_RANGES[0][0] && signal <= SIGNAL_RANGES[0][1]) {
        playButtonLight(LIGHT_RED, TONE_A4, buttonLightDelayMS);
        selectedButton = 0;

        break;
      }
      // Yellow
      else if (signal >= SIGNAL_RANGES[1][0] && signal <= SIGNAL_RANGES[1][1]) {
        playButtonLight(LIGHT_YEL, TONE_B4, buttonLightDelayMS);
        selectedButton = 1;

        break;
      }
      // Green
      else if (signal >= SIGNAL_RANGES[2][0] && signal <= SIGNAL_RANGES[2][1]) {
        playButtonLight(LIGHT_GRE, TONE_C4, buttonLightDelayMS);
        selectedButton = 2;

        break;
      }
      // Blue
      else if (signal >= SIGNAL_RANGES[3][0] && signal <= SIGNAL_RANGES[3][1]) {
        playButtonLight(LIGHT_BLU, TONE_D4, buttonLightDelayMS);
        selectedButton = 3;

        break;
      }
      else {
        noLightSound();

        selectedButton = -1;
      }
    }
    cprint(key, "Updating user sequence");

    USER_SEQUENCE[i] = selectedButton;
    selectedButton = -1;
  }

  return;
}

bool checkInputs() {
  String key = "checkInputs: ";

  for (int i = 0; i < iteration; i++) {
    if (SEQUENCE[i] == USER_SEQUENCE[i]) {
      cprint(key, "Passed");
    }
    else {
      cprint(key, "Failed... ");
      resetGame();
      return false;
    }
  }
  score++;

  if (score == SEQUENCE_LENGTH) {
    //hasWon = true;
  }

  return true;
}

void resetGame() {
  String key = "resetGame: ";

  cprint(key, "Resetting...");

  generateSequence();
  iteration = 0;
  isFirstRun = true;
  score = 0;
}

void noLightSound() {
  digitalWrite(LIGHT_RED, LOW);
  digitalWrite(LIGHT_YEL, LOW);
  digitalWrite(LIGHT_GRE, LOW);
  digitalWrite(LIGHT_BLU, LOW);

  noTone(BUZZER);

  return;
}

void playButtonLight(int light, unsigned int playTone, unsigned long delayMS) {
  String key = "playButtonLight: ";

  cprint(key, "Running, ");
  Serial.print(light);

  digitalWrite(light, HIGH);
  tone(BUZZER, playTone);

  delay(delayMS);

  digitalWrite(light, LOW);
  noTone(BUZZER);

  return;
}

void printSequence() {
  Serial.println("\n\nPrinting sequence... ");

  Serial.println("Sequence size: ");
  Serial.print(sizeof(SEQUENCE));
  Serial.println("\n---\n");

  for (int i = 0; i < sizeof(SEQUENCE); i++) {
    Serial.println(SEQUENCE[i]);
  }
  Serial.println("Sequence printed\n\n");
}

void cprint(String key, String message) {
  Serial.println(key + message);

  return;
}

void victorySequence() {
  String key = "victorySequence: ";

  cprint(key, "Running vicotry sequence");

  digitalWrite(LIGHT_RED, HIGH);
  digitalWrite(LIGHT_YEL, HIGH);
  digitalWrite(LIGHT_GRE, HIGH);
  digitalWrite(LIGHT_BLU, HIGH);

  tone(BUZZER, TONE_B4);

  delay(1000);

  digitalWrite(LIGHT_RED, LOW);
  digitalWrite(LIGHT_YEL, LOW);
  digitalWrite(LIGHT_GRE, LOW);
  digitalWrite(LIGHT_BLU, LOW);

  tone(BUZZER, TONE_A4);

  delay(1000);

  noLightSound();
  delay(300);

  return;
}

void correctSequence() {
  String key = "correctSequence: ";

  cprint(key, "Running correct sequence");

  noLightSound();

  delay(500);

  digitalWrite(LIGHT_BLU, HIGH);
  tone(BUZZER, TONE_D4);
  delay(450);

  digitalWrite(LIGHT_GRE, HIGH);
  tone(BUZZER, TONE_C4);
  delay(450);

  digitalWrite(LIGHT_YEL, HIGH);
  tone(BUZZER, TONE_B4);
  delay(450);

  digitalWrite(LIGHT_RED, HIGH);
  tone(BUZZER,TONE_A4);
  delay(450);

  noLightSound();
  delay(500);

  cprint(key, "Passed sequence ending");
  return;
}

void failSequence() {
  String key = "failSequence: ";

  noLightSound();

  digitalWrite(LIGHT_RED, HIGH);
  digitalWrite(LIGHT_YEL, HIGH);
  digitalWrite(LIGHT_GRE, HIGH);
  digitalWrite(LIGHT_BLU, HIGH);

  tone(BUZZER, TONE_D4);

  delay(1000);

  digitalWrite(LIGHT_RED, LOW);
  digitalWrite(LIGHT_YEL, LOW);
  digitalWrite(LIGHT_GRE, LOW);
  digitalWrite(LIGHT_BLU, LOW);

  tone(BUZZER, TONE_E4);

  delay(1000);

  noLightSound();
  delay(300);

  return;
}

void startupSequence() {
  String key = "startupSequence: ";

  cprint(key, "Running startup sequence");

  tone(BUZZER, TONE_B4);
  digitalWrite(LIGHT_RED, HIGH);

  delay(500);

  tone(BUZZER, TONE_C4);
  digitalWrite(LIGHT_YEL, HIGH);

  delay(500);

  tone(BUZZER, TONE_B4);
  digitalWrite(LIGHT_BLU, HIGH);

  delay(500);

  tone(BUZZER, TONE_A4);
  digitalWrite(LIGHT_GRE, HIGH);

  delay(500);

  noLightSound();

  tone(BUZZER, TONE_D4);

  delay(500);

  cprint(key, "Startup sequence ending");
  return;
}

void alert() {

  noLightSound();

  digitalWrite(LIGHT_RED, HIGH);
  digitalWrite(LIGHT_YEL, HIGH);
  digitalWrite(LIGHT_GRE, HIGH);
  digitalWrite(LIGHT_BLU, HIGH);

  tone(BUZZER, TONE_A4);

  delay(700);

  digitalWrite(LIGHT_RED, LOW);
  digitalWrite(LIGHT_YEL, LOW);
  digitalWrite(LIGHT_GRE, LOW);
  digitalWrite(LIGHT_BLU, LOW);

  tone(BUZZER, TONE_E4);

  delay(700);

  digitalWrite(LIGHT_RED, HIGH);
  digitalWrite(LIGHT_YEL, HIGH);
  digitalWrite(LIGHT_GRE, HIGH);
  digitalWrite(LIGHT_BLU, HIGH);

  tone(BUZZER, TONE_B4);

  delay(700);

  digitalWrite(LIGHT_RED, LOW);
  digitalWrite(LIGHT_YEL, LOW);
  digitalWrite(LIGHT_GRE, LOW);
  digitalWrite(LIGHT_BLU, LOW);

  tone(BUZZER, TONE_E4);

  delay(700);
}

void music() {
  const int E_N = 250;
  const int Q_N = 500;
  const int H_N = 1000;
  const int WH_N = 2000;

  const double P = 0.5;

  tone(BUZZER, C);
  delay(Q_N);
  
  noTone(BUZZER);
  delay(P);

  tone(BUZZER, C);
  delay(Q_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, B);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, A);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, G);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, F);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, E);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, E);
  delay(H_N + Q_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, E);
  delay(WH_N);

  noTone(BUZZER);
  delay(P);

  noTone(BUZZER);
  delay(Q_N);

  tone(BUZZER, C);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, A);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, G);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, F);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, E);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, D);
  delay(E_N);

  noTone(BUZZER);
  delay(P);

  tone(BUZZER, C);
  delay(WH_N);

  noTone(BUZZER);
  delay(P);
}

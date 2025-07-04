int BUT1 = D4;
int BUT2 = D5;
int BUT3 = D6;


int LED1 = D8;
int LED2 = D9;
int LED3 = D10;
int LED4 = D7;
int SL = A3;
int motorPin = D2;
bool running = false;
bool paused = false;

unsigned long startMillis = 0;
unsigned long pausedMillis = 0;
unsigned long elapsedMillis = 0;

int timerDuration = 1;
int timeLeft = 1;
int slider = 1;
bool but2prev = false;
bool but1prev = false;
bool but3prev = false;

void setup() {
  pinMode(BUT1, INPUT);

  pinMode(BUT2, INPUT);

  pinMode(BUT3, INPUT);


  pinMode(LED1, OUTPUT);

  pinMode(LED2, OUTPUT);

  pinMode(LED3, OUTPUT);
}

void loop() {
  if (!running && !paused) {
    int slider = analogRead(SL);
    timerDuration = map(potValue, 0, 1023, 1, 60);
    timeLeft = timerDuration;
    Serial.print("Set timer: ");
    Serial.print(timerDuration);
    Serial.println(" minutes");
    delay(200);
  }

  if (digitalRead(BUT1) == HIGH && !running && !but1prev) {
    but1prev = true;
    running = true;
    paused = false;
    startMillis = millis();
    elapsedMillis = 0;
    timeLeft = timerDuration;
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    Serial.println("Timer started!");
  }
  if (digitalRead(BUT1) == HIGH && running && !but1prev) {
    but1prev = true;
    running = false;
    paused = false;
    elapsedMillis = 0;
    timeLeft = timerDuration;
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  else if (digitalRead(BUT2) == HIGH && running && !but2prev) {
    but2prev = true;
    if (!paused) {
      paused = true;
      pausedMillis = millis();
      digitalWrite(LED2, LOW);
      Serial.println("Paused.");
    }
    else {
      paused = false;
      startMillis += (millis() - pausedMillis);
      digitalWrite(LED2, HIGH);
      Serial.println("Resumed.");
    }
  }
  
  else if (digitalRead(BUT3) == HIGH && running) {
    currentTimeSequence();
  }
  

  if (running && !paused) {
    elapsedMillis = millis() - startMillis;
    int elapsedMinutes = elapsedMillis / 60000; 
    timeLeft = timerDuration - elapsedMinutes;
    float percent = (float) (4 * timeLeft/timerDuration)/4.0;
    if (percent == 0.25 || percent > 0.5) {
      digitalWrite(LED4, HIGH);
    } else {
      digitalWrite(LED4, LOW);
    }
    if (percent > 0.25) {
      digitalWrite(LED3, HIGH);
    } else {
      digitalWrite(LED3, LOW);
    }

    if (timeLeft <= 0) {
      running = false;
      ledSequenceAfterTimer();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      Serial.println("Timer finished!");
    }
  } else {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
  }
  if (digitalRead(BUT2) == LOW) {
    but2prev = false;
  }
  if (digitalRead(BUT1) == LOW) {
    but1prev = false;
  }
}

void ledSequenceAfterTimer() {
  unsigned long startTime = millis();
  while (millis() - startTime < 5000) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    delay(500);

    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    delay(500);
  }
}

void currentTimeSequence() {
  unsigned long startTime = millis();
  int first = timeLeft / 15;
  int second = (timeLeft - first * 15) / 5;
  int third = (timeLeft - first * 15 - second * 5);
  digitalWrite(LED1, (first > 3) ? HIGH : LOW);
  digitalWrite(LED2, (first > 2) ? HIGH : LOW);
  digitalWrite(LED3, (first > 1) ? HIGH : LOW);
  digitalWrite(LED4, (first > 0) ? HIGH : LOW);
  digitalWrite(motorPin, HIGH); 
  delay(500);

  digitalWrite(LED1, (second > 3) ? HIGH : LOW);
  digitalWrite(LED2, (second > 2) ? HIGH : LOW);
  digitalWrite(LED3, (second > 1) ? HIGH : LOW);
  digitalWrite(LED4, (second > 0) ? HIGH : LOW);
  delay(250);

  digitalWrite(motorPin, LOW);
  delay(250);

  digitalWrite(motorPin, HIGH);
  digitalWrite(LED1, (third > 3) ? HIGH : LOW);
  digitalWrite(LED2, (third > 2) ? HIGH : LOW);
  digitalWrite(LED3, (third > 1) ? HIGH : LOW);
  digitalWrite(LED4, (third > 0) ? HIGH : LOW);
  delay(500);
  digitalWrite(motorPin, LOW);

  }
}

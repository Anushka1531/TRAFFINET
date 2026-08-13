/*
  TRAFFINET - Smart Adaptive Traffic Management Prototype

  Hardware concept:
  - Arduino UNO
  - 6 IR vehicle-density sensors
  - 3 traffic signal groups
  - Optional emergency-priority input

  IMPORTANT:
  The exact physical pin routing can differ from the prototype image.
  Verify every pin against your actual wiring before powering the circuit.

  Sensor convention:
  LOW  = vehicle detected
  HIGH = no vehicle detected

  Signal groups:
  Group 0 = Road A
  Group 1 = Road B
  Group 2 = Road C
*/

const byte NUM_ROADS = 3;
const byte NUM_SENSORS = 6;

// Two IR sensors per road: entry/queue points.
const byte sensorPins[NUM_SENSORS] = {A0, A1, A2, A3, A4, A5};

// Road A: R=2, Y=3, G=4
// Road B: R=5, Y=6, G=7
// Road C: R=8, Y=9, G=10
const byte redPins[NUM_ROADS]   = {2, 5, 8};
const byte yellowPins[NUM_ROADS]= {3, 6, 9};
const byte greenPins[NUM_ROADS] = {4, 7, 10};

// Optional emergency input.
// Connect a push button/signal to this pin if you want to demo priority.
// INPUT_PULLUP: LOW means emergency request is active.
const byte emergencyPin = 11;

const unsigned long MIN_GREEN_MS = 5000;
const unsigned long MAX_GREEN_MS = 12000;
const unsigned long YELLOW_MS = 2000;

int roadDensity[NUM_ROADS] = {0, 0, 0};
int activeRoad = 0;
unsigned long greenStartedAt = 0;

void setup() {
  Serial.begin(9600);

  for (byte i = 0; i < NUM_SENSORS; i++) {
    pinMode(sensorPins[i], INPUT);
  }

  for (byte r = 0; r < NUM_ROADS; r++) {
    pinMode(redPins[r], OUTPUT);
    pinMode(yellowPins[r], OUTPUT);
    pinMode(greenPins[r], OUTPUT);
  }

  pinMode(emergencyPin, INPUT_PULLUP);

  allRed();
  setGreen(activeRoad);
  greenStartedAt = millis();

  Serial.println(F("TRAFFINET controller started."));
}

void loop() {
  readTrafficDensity();
  printStatus();

  if (digitalRead(emergencyPin) == LOW) {
    emergencyPriority();
    return;
  }

  int busiestRoad = selectBusiestRoad();

  unsigned long greenElapsed = millis() - greenStartedAt;

  // Keep the current road green for the minimum safe demonstration period.
  if (greenElapsed < MIN_GREEN_MS) {
    delay(200);
    return;
  }

  // Switch if another road is busier, or if the maximum green duration is reached.
  if (busiestRoad != activeRoad &&
      roadDensity[busiestRoad] > roadDensity[activeRoad]) {

    changeToRoad(busiestRoad);
  }
  else if (greenElapsed >= MAX_GREEN_MS) {
    changeToRoad(busiestRoad);
  }

  delay(200);
}

void readTrafficDensity() {
  // Two sensors represent each road.
  // A LOW reading means a vehicle is detected.
  roadDensity[0] = (digitalRead(sensorPins[0]) == LOW) +
                   (digitalRead(sensorPins[1]) == LOW);

  roadDensity[1] = (digitalRead(sensorPins[2]) == LOW) +
                   (digitalRead(sensorPins[3]) == LOW);

  roadDensity[2] = (digitalRead(sensorPins[4]) == LOW) +
                   (digitalRead(sensorPins[5]) == LOW);
}

int selectBusiestRoad() {
  int best = 0;

  for (byte r = 1; r < NUM_ROADS; r++) {
    if (roadDensity[r] > roadDensity[best]) {
      best = r;
    }
  }

  return best;
}

void changeToRoad(int nextRoad) {
  if (nextRoad == activeRoad) {
    return;
  }

  // Safe transition: current green -> yellow -> all red -> next green.
  digitalWrite(greenPins[activeRoad], LOW);
  digitalWrite(yellowPins[activeRoad], HIGH);
  delay(YELLOW_MS);
  digitalWrite(yellowPins[activeRoad], LOW);

  allRed();
  delay(300);

  activeRoad = nextRoad;
  setGreen(activeRoad);
  greenStartedAt = millis();

  Serial.print(F("Adaptive switch -> Road "));
  Serial.println(activeRoad + 1);
}

void emergencyPriority() {
  Serial.println(F("EMERGENCY PRIORITY REQUEST"));

  // Demonstration behavior: clear current green and give priority
  // to Road A. In a real system, the emergency route would be
  // determined from vehicle location and intersection topology.

  digitalWrite(greenPins[activeRoad], LOW);
  digitalWrite(yellowPins[activeRoad], HIGH);
  delay(YELLOW_MS);
  digitalWrite(yellowPins[activeRoad], LOW);

  allRed();
  delay(300);

  activeRoad = 0;
  setGreen(activeRoad);
  greenStartedAt = millis();

  Serial.println(F("Emergency route -> Road 1 GREEN"));

  // Wait for request to be released.
  while (digitalRead(emergencyPin) == LOW) {
    delay(50);
  }
}

void allRed() {
  for (byte r = 0; r < NUM_ROADS; r++) {
    digitalWrite(redPins[r], HIGH);
    digitalWrite(yellowPins[r], LOW);
    digitalWrite(greenPins[r], LOW);
  }
}

void setGreen(byte road) {
  for (byte r = 0; r < NUM_ROADS; r++) {
    digitalWrite(redPins[r], r == road ? LOW : HIGH);
    digitalWrite(yellowPins[r], LOW);
    digitalWrite(greenPins[r], r == road ? HIGH : LOW);
  }
}

void printStatus() {
  Serial.print(F("Density | R1: "));
  Serial.print(roadDensity[0]);
  Serial.print(F(" R2: "));
  Serial.print(roadDensity[1]);
  Serial.print(F(" R3: "));
  Serial.println(roadDensity[2]);
}

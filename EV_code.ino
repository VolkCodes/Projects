// Motor Driver L298N Pins
#define ENA 5   // Speed control for Motor A
#define IN1 9   // Motor A direction
#define IN2 10  // Motor A direction

// Encoder Pins
#define ENCODER_A 2   // Encoder Channel A (Interrupt)
#define ENCODER_B 3   // Encoder Channel B

volatile int pulseCount = 0;  // Encoder pulse counter

// User-defined parameters
float wheel_diameter = 0.05;  // Wheel diameter in meters
float gear_ratio = 1.5;       // Motor rotations per wheel rotation
int encoder_ticks_per_rev = 360;  // Encoder pulses per motor revolution
float target_distance = 1.0;  // Target distance in meters

// Computed values
float wheel_circumference;
float required_wheel_rotations;
float required_motor_rotations;
int required_encoder_pulses;

// Interrupt function for counting encoder pulses
void countPulse() {  // ✅ Removed IRAM_ATTR
    pulseCount++;
}

// Function to move the motor based on encoder pulses
void moveMotor(int speed, int direction, int target_pulses) {
    pulseCount = 0; // Reset encoder count

    // Set motor direction
    if (direction == 1) { // Forward
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
    } else { // Reverse
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
    }

    // Start moving the motor
    analogWrite(ENA, speed);

    // Wait until target pulses are reached
    while (pulseCount < target_pulses);

    // Stop the motor when the required pulses are reached
    analogWrite(ENA, 0);
}

void setup() {
    Serial.begin(9600);

    // Set motor pins as outputs
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    // Set encoder pins as inputs
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);

    // Attach interrupt for encoder channel A
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), countPulse, RISING);   

    // Compute motion parameters
    wheel_circumference = wheel_diameter * 3.14159265358979;
    required_wheel_rotations = target_distance / wheel_circumference;
    required_motor_rotations = required_wheel_rotations * gear_ratio;
    required_encoder_pulses = required_motor_rotations * encoder_ticks_per_rev;
}

void loop() {
    Serial.print("Moving forward for: ");
    Serial.print(target_distance);
    Serial.println(" meters");

    moveMotor(150, 1, required_encoder_pulses); // Move forward

    delay(2000); // Pause before moving back

    Serial.println("Reversing half the distance...");
    moveMotor(150, 0, required_encoder_pulses / 2); // Move back half the distance

    while (1); // Stop execution after movement
}

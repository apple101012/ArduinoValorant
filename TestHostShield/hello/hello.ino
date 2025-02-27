void setup() {
    // Start serial communication at 115200 baud
    Serial.begin(115200);
    
    // Wait for serial to be ready
    while (!Serial) {
        ; // Do nothing, just wait
    }

    // Print "Hello" to the Serial Monitor
    Serial.println("Hello");
}

void loop() {
    // Do nothing in the loop
}

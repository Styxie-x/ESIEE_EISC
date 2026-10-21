#include <Arduino.h>

const uint8_t LED_PIN = 3;
const unsigned long BLINK_INTERVAL_MS = 500;
const unsigned long TEMP_SEND_INTERVAL_MS = 500;

enum LedMode { LED_OFF, LED_ON, LED_BLINK };

LedMode led_mode = LED_OFF;
bool led_output_state = LOW;
unsigned long last_blink_time = 0;
unsigned long last_temp_send_time = 0;
int temp_threshold = 25;

int read_internal_temperature() {
  // Read internal temperature sensor of ATmega328P
  ADMUX = (1 << REFS1) | (1 << REFS0) | 8;  // 1.1V ref, temp sensor channel
  delay(10);  // Wait for channel change to settle
  ADCSRA |= (1 << ADSC);  // Start ADC conversion
  while (bit_is_set(ADCSRA, ADSC));  // Wait for conversion
  uint8_t low = ADCL;
  uint8_t high = ADCH;
  int raw = (high << 8) | low;
  
  // Take another reading for accuracy
  ADCSRA |= (1 << ADSC);
  while (bit_is_set(ADCSRA, ADSC));
  low = ADCL;
  high = ADCH;
  raw = (high << 8) | low;
  
  // Convert raw ADC to temperature (in Celsius)
  // Calibrated formula for ATmega328P internal sensor
  int temperature = (int)((raw - 324) / 1.22);
  return temperature;
}

void set_led_output(bool state) {
  if (led_output_state != state) {
    led_output_state = state;
    digitalWrite(LED_PIN, state);
  }
}

void handle_command(const String& cmd) {
  String c = cmd;
  c.trim();

  if (c.startsWith("TEMP:")) {
    temp_threshold = c.substring(5).toInt();
    Serial.print("TEMP set to ");
    Serial.println(temp_threshold);
  } else if (c == "LED:ON") {
    led_mode = LED_ON;
    set_led_output(HIGH);
    Serial.println("LED ON");
  } else if (c == "LED:OFF") {
    led_mode = LED_OFF;
    set_led_output(LOW);
    Serial.println("LED OFF");
  } else if (c == "LED:BLINK") {
    led_mode = LED_BLINK;
    last_blink_time = millis();
    Serial.println("LED BLINK");
  } else {
    Serial.print("Unknown cmd: ");
    Serial.println(c);
  }
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  // Initialize ADC for temperature sensor
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Enable ADC, prescaler 128
  set_led_output(LOW);
}

void loop() {
  unsigned long now = millis();

  if (led_mode == LED_BLINK) {
    if (now - last_blink_time >= BLINK_INTERVAL_MS) {
      led_output_state = !led_output_state;
      digitalWrite(LED_PIN, led_output_state);
      last_blink_time = now;
    }
  } else {
    set_led_output(led_mode == LED_ON ? HIGH : LOW);
  }

  // Temperature sensor reading from internal ATmega328P sensor
  if (now - last_temp_send_time >= TEMP_SEND_INTERVAL_MS) {
    int temperature = read_internal_temperature();
    Serial.print("TEMP:");
    Serial.println(temperature);
    last_temp_send_time = now;
  }

  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    if (cmd.length() > 0) {
      handle_command(cmd);
    }
  }
}


class Blinker {
  public:
    Blinker(int pin, uint32_t range, uint32_t on) :
      m_pin(pin),
      m_time_range_ms(range),
      m_time_on_ms(on)
    {
      pinMode(m_pin, OUTPUT);
    }
    Blinker(const Blinker&) = delete;
    Blinker& operator=(const Blinker&) = delete;
    void Update(uint32_t ms) {
      if (!m_led_state && (ms - m_last_ms) >= (m_time_range_ms - m_time_on_ms)) {
        m_last_ms = ms;
        toggleLed();
        Serial.print("\nON\tms: "); Serial.println(ms);
      } else if (m_led_state && (ms - m_last_ms) >= m_time_on_ms) {
        m_last_ms = ms;
        toggleLed();
        Serial.print("OFF\tms: "); Serial.println(ms);
      }
    }
  private:
    void toggleLed() {
      m_led_state = !m_led_state;
      digitalWrite(m_pin, m_led_state);
    }
    const int m_pin;
    uint32_t m_time_range_ms;
    uint32_t m_time_on_ms;
    bool m_led_state = false;
    uint32_t m_last_ms = 0;
};

Blinker led(LED_BUILTIN, 10000, 2000);

ISR(TIMER1_COMPA_vect) {
  led.Update(millis());
}

void setup() {
  Serial.begin(9600);
  TCCR1A  = 0;
  TCCR1B  = (1 << WGM12) | (1 << CS12) | (1 << CS10);
  TCNT1   = 0;
  OCR1A   = 0x3D08;
  TIMSK1 |= (1 << OCIE1A);
}

void loop() {}

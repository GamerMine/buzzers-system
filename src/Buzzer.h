#ifndef BUZZERS_BUZZER_H
#define BUZZERS_BUZZER_H

#include <vector>

typedef void BuzzerPushedCallback(void *buzzer);

class Buzzer {
public:
    static void add(const uint8_t gpio, const uint8_t led_gpio, BuzzerPushedCallback *cb) {
        _buzzers.push_back(Buzzer(gpio, led_gpio, cb));
    }

    static void enableAll() {
        for (Buzzer &buzzer : _buzzers) {
            buzzer.enable();
        }
    }

    static void disableAll() {
        for (Buzzer &buzzer : _buzzers) {
            buzzer.disable();
        }
    }

    static void disableAllExcept(const uint8_t id) {
        for (Buzzer &buzzer : _buzzers) {
            if (buzzer.id() == id) continue;
            buzzer.disable();
        }
    }

    static Buzzer *get(const uint8_t id) {
        for (Buzzer &buzzer : _buzzers) {
            if (buzzer.id() == id) {return &buzzer;}
        }

        return nullptr;
    }
    static std::vector<Buzzer> getAll() { return _buzzers; }
    static void resetAll() {
        for (Buzzer &buzzer : _buzzers) {
            buzzer._score = 0;
        }
    }

    [[nodiscard]] bool isEnabled() const { return _enabled; }
    [[nodiscard]] uint8_t id() const { return _buzzer_id; }
    [[nodiscard]] uint16_t score() const { return _score; }

    void enable() {
        digitalWrite(_led_gpio, HIGH);
        _enabled = true;
    }

    void disable() {
        digitalWrite(_led_gpio, LOW);
        _enabled = false;
    }

    void incrementScore() { _score++; }

private:
    Buzzer(const uint8_t gpio, const uint8_t led_gpio, BuzzerPushedCallback *cb): _enabled(false), _led_gpio(led_gpio), _buzzer_id(_buzzers.size() + 1), _score(0) {
        pinMode(gpio, INPUT_PULLUP);
        pinMode(led_gpio, OUTPUT);
        /*digitalWrite(led_gpio, LOW);*/
        attachInterruptParam(digitalPinToInterrupt(gpio), cb, FALLING, this);
    }

    inline static std::vector<Buzzer> _buzzers;

    bool _enabled;
    uint8_t _led_gpio;
    uint8_t _buzzer_id;
    uint16_t _score;
};

#endif //BUZZERS_BUZZER_H

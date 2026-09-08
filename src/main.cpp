#include <Arduino.h>
#include <BackgroundAudioWAV.h>
#include <PWMAudio.h>
#include <SD.h>

#include "config.h"
#include <st7796s.h>

#include "GameConfig.h"
#include "GameState.h"
#include "gui/page/Home.h"

static PWMAudio pwm {AUDIO_PWM_GPIO, false};
static BackgroundAudioWAV wav {pwm};

void setup() {
    Serial.begin(9600);

    // SD CARD
    SPI.setSCK(SD_SPI_SCK_GPIO);
    SPI.setTX(SD_SPI_MOSI_GPIO);
    SPI.setRX(SD_SPI_MISO_GPIO);
    SPI.setCS(SD_SPI_CS_GPIO);

    if (!SD.begin(SD_SPI_CS_GPIO)) {
        Serial.println("SD Init FAILED");
        return;
    }

    if (!wav.begin()) {
        Serial.println("wav.begin() FAILED");
        return;
    }

    GameState::init(&wav);
}

void setup1() {
    // LCD/TOUCHSCREEN
    SPI1.setSCK(LCD_SPI_SCK_GPIO);
    SPI1.setTX(LCD_SPI_MOSI_GPIO);
    SPI1.setRX(LCD_SPI_MISO_GPIO);

    ST7796S::begin(&SPI1, LCD_SPI_CS_GPIO, LCD_DC_GPIO, LCD_RESET_GPIO, TOUCH_SPI_CS_GPIO, TOUCH_IRQ_GPIO);
    ST7796S::clear();

    Home::show();
}

void loop() {
    GameState::update();
}

void loop1() {
    ST7796S::update();
}
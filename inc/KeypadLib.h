#ifndef KEYPADLIB_H
#define KEYPADLIB_H

#include <iostream>
#include <gpiod.hpp>
#include <string.h>
#include <unistd.h>

class Keypad {
public:
    Keypad();

    void init();                 // PDF'e uygun isim
    void release();              // PDF'e uygun isim
    char getKey();               // PDF'e uygun isim

    // İsteğe bağlı setter fonksiyonlar
    void setKeyGPIOCol(int col1, int col2, int col3, int col4) {
        keyGPIOCol[0] = col1;
        keyGPIOCol[1] = col2;
        keyGPIOCol[2] = col3;
        keyGPIOCol[3] = col4;
    }

    void setKeyGPIOrow(int row1, int row2, int row3, int row4) {
        keyGPIORow[0] = row1;
        keyGPIORow[1] = row2;
        keyGPIORow[2] = row3;
        keyGPIORow[3] = row4;
    }

    void setChipName(std::string ch) {
        chipName = ch;
    }

private:
    char key[4][4] = {
        {'1','2','3','A'},
        {'4','5','6','B'},
        {'7','8','9','C'},
        {'*','0','#','D'}
    };

    int keyGPIOCol[4] = {6,13,19,26};   // output
    int keyGPIORow[4] = {16,20,21,12};  // input

    struct gpiod_line *lineKeyCol[4];
    struct gpiod_line *lineKeyRow[4];
    std::string chipName;
    struct gpiod_chip* chip;
};

#endif

#ifndef SHIFTREG_H
#define SHIFTREG_H

#include <mbed.h>

/** A simple serial driver for a shift register that uses only three digital out pins.
* Based on a fork of Ollie8/ShiftOut
*/
class ShiftReg {

    public :
    
        ShiftReg();    
        ~ShiftReg();
        void write(int data);
         
    private :
        DigitalOut *clkout;
        DigitalOut *dataout;
        DigitalOut *latchout;
};

#endif

// test!!
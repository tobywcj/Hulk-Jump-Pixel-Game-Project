#include "ShiftReg.h"

ShiftReg::ShiftReg()
{
    clkout = new DigitalOut(p7);
    dataout = new DigitalOut(p5);
    latchout = new DigitalOut(p30);
}

ShiftReg::~ShiftReg()
{
    delete clkout;
    delete dataout;
    delete latchout;
}

void ShiftReg::write(int data)
{
    *latchout = 0;
    for (int i = 7; i >=  0; i--) {
        *clkout = 0;
        *dataout = (data & (1 << i)) != 0;
        *clkout = 1;
    }
    *latchout = 1;
}
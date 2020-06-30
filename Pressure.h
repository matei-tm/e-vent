/**
 * MIT Emergency Ventilator Controller
 * 
 * MIT License:
 * 
 * Copyright (c) 2020 MIT
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * Pressure.h
 * Calculates and stores the key pressure values of the breathing cycle.
 */

#ifndef Pressure_h
#define Pressure_h

#include "Arduino.h"
#include "Wire.h"

namespace pressure
{
  class Pressure
  {
  public:
    Pressure(uint8_t init_address);
    virtual void begin() = 0;

    //Get pressure reading
    virtual void read() = 0;

    const float &get();

    void set_peak_and_reset();

    void set_plateau();

    void set_peep();

    const float &peak();
    const float &plateau();
    const float &peep();

  protected:
    int init_address_;
    float current_;
    float current_peak_;
    float peak_, plateau_, peep_;
  };

  class PressureByI2C : public Pressure
  {
  public:
    PressureByI2C(uint8_t init_address);

    void begin();

    //Get pressure reading
    void read();

  private:
    void getdata(byte *a, byte *b, byte *c, byte *d);
  };

  class PressureByAnalog : public Pressure
  {
  public:
    PressureByAnalog(uint8_t init_pin);

    void begin();

    //Get pressure reading
    void read();
  };
} // namespace pressure

#endif

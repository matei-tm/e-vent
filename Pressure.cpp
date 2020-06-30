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
 * raw_pressure_output OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * Pressure.cpp
 * Calculates and stores the key pressure values of the breathing cycle.
 */

#include "Pressure.h"

namespace pressure
{
  Pressure ::Pressure(uint8_t i2c_address) : i2c_address_(i2c_address),
                                             current_(0.0),
                                             current_peak_(0.0),
                                             peak_(0.0),
                                             plateau_(0.0),
                                             peep_(0.0) {}

  void Pressure ::begin()
  {
    Wire.begin();
  }

  // Get pressure reading for NPA-730B-005D
  void Pressure ::read()
  {
    byte aa, bb, cc, dd;
    uint16_t raw_pressure_output; // measured pressure on sensor output
    uint16_t raw_temp_output; // measured temperature on sensor output

    getdata(&aa, &bb, &cc, &dd);

    float Pmin = 0;
    float Pmax = 5;          // Pmax=5PSI
    float OutMin = 8192.00;  // Differential at 0 is 8192.00 counts and 1638 at -5PSi ;
    float OutMax = 16383.00; // 14745.00 Page 2, Table 1 https://www.amphenol-sensors.com/en/component/edocman/356-novasensor-npa-pressure-sensor-application-guide/download?Itemid=8429%20%27

    raw_pressure_output = ((aa & 0x3F) << 8) | bb;

    raw_temp_output = ((cc & 0xFF) << 8) | dd;
    raw_temp_output >>= 5;

    // convert to pressure
    float pres = (float)(raw_pressure_output - OutMin) / (OutMax - OutMin) * (Pmax - Pmin) + Pmin; // PSI
    float temperature = (float)(raw_temp_output * 200.00) / (2048.00) - 50.00;

    // convert to cmH20
    pres *= 70.306957829636;

    // update peak
    current_peak_ = max(current_peak_, pres);

    current_ = pres;
  }

  const float &Pressure ::get()
  {
    return current_;
  }

  void Pressure ::set_peak_and_reset()
  {
    peak_ = current_peak_;
    current_peak_ = 0.0;
  }

  void Pressure ::set_plateau()
  {
    plateau_ = get();
  }

  void Pressure ::set_peep()
  {
    peep_ = get();
  }

  const float &Pressure ::peak() { return peak_; }
  const float &Pressure ::plateau() { return plateau_; }
  const float &Pressure ::peep() { return peep_; }

  void Pressure ::getdata(byte *a, byte *b, byte *c, byte *d)
  {
    Wire.beginTransmission(i2c_address_);
    Wire.write(0);
    Wire.endTransmission();

    Wire.requestFrom(i2c_address_, 4);
    *a = Wire.read();
    *b = Wire.read();
    *c = Wire.read();
    *d = Wire.read();
  }
} // namespace pressure
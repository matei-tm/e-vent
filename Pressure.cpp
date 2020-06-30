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
  Pressure ::Pressure(uint8_t init_address) : init_address_(init_address),
                                              current_(0.0),
                                              current_peak_(0.0),
                                              peak_(0.0),
                                              plateau_(0.0),
                                              peep_(0.0) {}

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

} // namespace pressure
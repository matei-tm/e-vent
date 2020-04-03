#include "Display.h"

namespace display {


/// TextAntimation ///

void TextAnimation::reset(const String& text){
  reset_time_ = millis();
  text_ = text;
}

bool TextAnimation::empty(){
  return text_.length() == 0;
}

const String& TextAnimation::text(){
  return text_;
}

const String TextAnimation::getLine(){
  String new_text;
  unsigned long time_now = millis();
  if(time_now - reset_time_ < kBlinkOnFraction * kBlinkPeriod){
    new_text = text_;
    while(new_text.length() < WIDTH){
      new_text += " ";
    }
    if(new_text.length() > WIDTH){
      new_text = new_text.substring(0, 20);
    }
  }
  else if(time_now - reset_time_ < kBlinkPeriod){
    new_text = kBlankLine;
  }
  else {
    reset_time_ = time_now;
  }
  return new_text;
}


/// Display ///

Display::Display(LiquidCrystal* lcd): lcd_(lcd) {}

void Display::begin() {
  lcd_->begin(WIDTH, HEIGHT);
  lcd_->noCursor(); 
  update();
}

void Display::update(){
  if(animation_.empty()){
    write(0, 10, " P(cmH2O):");
  } 
  else {
    write(0, 0, animation_.getLine());
  }
}

void Display::writeAlarmText(const String& alarm){
  if(animation_.text() != alarm){
    animation_.reset(alarm);
  }
}

void Display::writeVolume(const int& vol){
  if(animation_.empty()){
    char buff[11];
    sprintf(buff, "V=%3d cc  ", vol);
    write(0, 0, buff);
  }
}

void Display::writeBPM(const int& bpm){
  char buff[12];
  sprintf(buff, "RR=%2d/min  ", bpm);
  write(1, 0, buff);
}

void Display::writeIEratio(const float& ie){
  char ie_buff[4];
  dtostrf(ie, 3, 1, ie_buff);
  char buff[12];
  sprintf(buff, "I:E=1:%s  ", ie_buff);
  write(2, 0, buff);
}

void Display::writeACTrigger(const float& ac_trigger, const float& lower_threshold){
  char buff[13];
  if(ac_trigger > lower_threshold){
    char ac_buff[4];
    dtostrf(ac_trigger, 3, 1, ac_buff);
    sprintf(buff, "AC=%scmH20 ", ac_buff);
  } else {
    sprintf(buff, "AC=OFF     ");
  }
  write(3, 0, buff);
}

void Display::writePeakP(const int& peak){
  char buff[10];
  sprintf(buff, "  peak=%2d", peak);
  write(1, 11, buff);
}

void Display::writePlateauP(const int& plat){
  char buff[10];
  sprintf(buff, "  plat=%2d", plat);
  write(2, 11, buff);
}

void Display::writePEEP(const int& peep){
  char buff[10];
  sprintf(buff, "  PEEP=%2d", peep);
  write(3, 11, buff);
}

template <typename T>
void Display::write(const int& row, const int& col, const T& printable){
  lcd_->setCursor(col, row);
  lcd_->print(printable);
}


}  // namespace display

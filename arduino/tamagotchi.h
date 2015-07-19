#include <Arduino.h>

struct RGB {
  byte r;
  byte g;
  byte b;
};


class life {
  private:
    
    int lightOn = 200;
    int addToCounter=0;
    
    unsigned long counter = 1000;
    unsigned long prevTime = 0;
  
  public:
    bool flash(unsigned long time) {

      if ( counter > time - prevTime) { 
        return LOW;
      }
      else if (counter + lightOn > time - prevTime) {
        return HIGH;
      }
      else {
        prevTime = time;
        addToCounter++;
        if (addToCounter == 5){
            counter += 100;
          }
      }
    }
      
};

class state {
  private:
    unsigned long lastUpdate = 0;
    unsigned long lastIncrease = 0;
    unsigned long updateTimes[25];
    unsigned long updateCount = 0;
    byte writeBits = 0b1111; //default is 15 in binary
    
  public:    
    void updateTime (unsigned long time) {
      lastUpdate = time;
      updateTimes[updateCount++] = time;
    }
    unsigned long * getTimes () {
      return updateTimes;
    }
    int getUpdate() {
      return lastUpdate;
    }
    int getWriteBits(){
      return writeBits;
    }
    void update (unsigned long time, int difference = 3000) {
      if (time - lastUpdate > difference) {
          writeBits = writeBits >> 1;
          lastUpdate = time;
      }
    }
    void increaseState(unsigned long time){
      if (writeBits < 0b1111) {
        if(time -lastIncrease > 1500){
          writeBits = (writeBits << 1) +1;
          lastIncrease = time;
          //lastUpdate = time;
          this->updateTime(time);
          }
        }
    }
  void decreaseState(unsigned long time){
    if (writeBits < 0b1111) {
        writeBits = writeBits >> 1;
        lastUpdate = time;
    }
  }
    
};

class button {
  private:
    volatile boolean state = 0;
    volatile boolean changedState = false;
  public:
    void update(boolean stateIn){
      state = stateIn;
      changedState = true;
    }
    void toggle (){
      this->update(!state);
    }
    boolean getState(boolean silent = false){
      if (silent){
        return state;
      }
      changedState = silent;
      return state;
    }
    boolean changed(){
      return changedState;
    }
};


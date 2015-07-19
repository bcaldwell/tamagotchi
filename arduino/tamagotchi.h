#include <Arduino.h>

struct RGB {
  byte r;
  byte g;
  byte b;
};


class life {
  private:
    bool alive = true;
    int lightOn = 20;
    int addToCounter=0;
    
    unsigned long counter = 100;
    unsigned long prevTime = 0;
  
  public:

  unsigned long timeAlive(unsigned long time){
      return time;
    } 
  
  bool isAlive(){
    return alive;
    }
    bool flash(unsigned long time) {

      if (time > 5*60*1000) {
        alive = false;
        return LOW;
      }

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
            lightOn += 50;
            addToCounter = 0;
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
    bool update (unsigned long time, int difference = 3000) {
      if (time - lastUpdate > difference) {
          writeBits = writeBits >> 1;
          lastUpdate = time;
      }
      if (writeBits == 0){
        return false;
        }
        return true;
    }
    void increaseState(unsigned long time){
      if (writeBits < 0b1111) {
        if(time -lastIncrease > 1500){
          writeBits = (writeBits << 1) +1;
          lastIncrease = time;
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


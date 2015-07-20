#include <Arduino.h>

struct RGB {
  byte r;
  byte g;
  byte b;
};

//Class for dealing with lifespan
class life {
  private:
    bool alive = true;
    int lightOn = 20;
    int addToCounter=0;
    
    unsigned long counter = 100;
    unsigned long prevTime = 0;
  
  public:
// complicated function to determine alive time
  unsigned long timeAlive(unsigned long time){
      return time;
    } 
  //check if alive and determine light speed for flashing heartbeat
  bool isAlive(){
    return alive;
    }
    bool flash(unsigned long time) {
    //Death after 4mins no matter what 
      if (time > 4*60*1000) {
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
        //Make light slowly decrease speed
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
    unsigned long updateTimes[50];
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
    bool update (unsigned long time, int difference = 10000) {
      if (time - lastUpdate > difference) {
          writeBits = writeBits >> 1;
          lastUpdate = time;
      }
      if (writeBits == 0){
        return false;
        }
        return true;
    }
    //Function for when increases are needed based on time events 
    void increaseState(unsigned long time, bool override = false){

      if (writeBits < 0b1111) {
        if(time -lastIncrease > 3000){
          writeBits = (writeBits << 1) +1;
          lastIncrease = time;
          this->updateTime(time);
          }
          else if(override == true){
            writeBits = (writeBits << 1) +1;
          }
        }
    }
    //Function for when decreases are needed based on time events
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


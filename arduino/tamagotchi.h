#include <Arduino.h>

struct RGB {
  byte r;
  byte g;
  byte b;
};

class state {
  private:
    int lastUpdate = 0;
    int updateTimes[25];
    int updateCount = 0;
    byte writeBits = 0b1111; //default is 15 in binary
    
  public:    
    void updateTime (unsigned long time) {
      lastUpdate = time;
      updateTimes[updateCount++] = time;
    }
    int* getTimes () {
      return updateTimes;
    }
    int getUpdate() {
      return lastUpdate;
    }
    int getWriteBits(){
      return writeBits;
    }
    void update (unsigned long time, int difference = 3000) {
      if (time - lastUpdate > 3000) {
          writeBits = writeBits >> 1;
          lastUpdate = time;
      }
    }
};

class button {
  private:
    boolean state = 0;
    boolean changedState = false;
  public:
    void update(boolean stateIn){
      state = stateIn;
      changedState = true;
    }
    boolean getState(){
      changedState = false;
      return state;
    }
    boolean changed(){
      return changedState;
    }  
};


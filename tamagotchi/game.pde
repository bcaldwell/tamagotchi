boolean gameInProcess = false;
egg[] eggs = new egg[20];
long startTime = 0;
int bottomDistance = 150;
int lastEgg = 0;

void game () {
  if (gameInProcess) {
    for (int i = 0; i < eggs.length; i++) {
      eggs[i].update(startTime);
    }
    strokeWeight(10);
    stroke (#FF0000);
    line(0, height-bottomDistance, width, height-bottomDistance);
    strokeWeight(1);
    stroke (255);
  } else {    
    int[] dropTimes = new int[eggs.length];

    for (int i = 0; i < dropTimes.length; i++) {
      int rand;
      if (i >= 14) {
        rand = int(random(50, 60));
      } else if (i >= 8) {
        rand = int(random(30, 50));
      } else if (i>=1) {
        rand = int(random(5, 30));
      } else {
        rand = 0;
      }
      dropTimes[i] = rand;
    }
    dropTimes = sort(dropTimes);
    int lastPos = -1;
    for (int i = 0; i < dropTimes.length; i++) {
      int pos;
      do {
        pos = int(random(0, 3));
      }
      while (pos == lastPos);

      eggs[i] = new egg(dropTimes[i], pos);
      lastPos = pos;
    }
    startTime = millis();
    gameInProcess = true;
  }
}


//sort array of eggs by start time and the process them in order on button press

class egg {
  private long ypos = 0;
  private int xpos = width/2;
  private float speed = 5; 
  private long dropTime;
  private boolean enabled = true;
  egg() {
  };
  egg(long time, int pos) {
    xpos = int(width/4)*(pos+1);
    dropTime = time;
  }
  int getDropTime() {
    return 1;
  }
  void setDropTime(long time) {
    dropTime = time;
  }
  void update (long startTime) {
    if (enabled && (millis() - startTime)/1000 > dropTime) {
      ypos += speed;
      fill(255);
      ellipse(xpos, ypos, 60, 100);
    }
  }
  boolean check (int pos) {
    enabled = false;
    return false;
  }
}

void keyPressed() {
  int pos = -1;
  if (key == 97) {
    pos = 0;
  } else if (key == 115) {
    pos = 1;
  } else if (key == 100) {
    pos = 2;
  }
  if (pos > 0) {
    if (!eggs[lastEgg++].check(pos)) {
      mode = "move";
      faceState = "normal";
    }
  }
}


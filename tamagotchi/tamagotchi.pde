import processing.serial.*;

PImage bg;
PImage normal;
int imageScale = 4;
int ychange = 0;
int xchange = 0;
boolean movingUp = true;
boolean movingLeft = true;

int ychangeSpeed = 3;
int xchangeSpeed = 1;

public void setup() {
  size (1152, 504);
  bg = loadImage("../background.jpg");
  normal = loadImage("../normal_face.png");
  normal.resize(normal.width/imageScale, normal.height/imageScale);
}

public void draw() {  
  imageMode(CENTER);
  background(bg);
  image(normal, width/2 - xchange, height/2 - ychange);

  //determine y position
  if (movingUp) {
    ychange+= ychangeSpeed;
  } else {
    ychange-= ychangeSpeed;
  }
  if (ychange == 75) {
    movingUp = false;
  } else if (ychange == 0) {
    movingUp = true;
    movingLeft = boolean(int(random(0,100)%2));
  }

  if (movingLeft) {
    xchange+= xchangeSpeed;
  } else {
    xchange-= xchangeSpeed;
  }
}


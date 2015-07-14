import processing.serial.*;

PImage bg;
PImage normal;
int imageScale = 4;
int ychange = 0;
int xchange = 0;
boolean movingUp = true;
boolean movingLeft = true;

int ychangeSpeed = 3;
int xchangeSpeed = 4;
int counter = 0;

public void setup() {
  size (1152, 504);
  bg = loadImage("../background.jpg");
  normal = loadImage("../normal_face.png");
  normal.resize(normal.width/imageScale, normal.height/imageScale);å  
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
  if (ychange >= 75) {
    movingUp = false;
  } else if (ychange <= 0) {
    movingUp = true;
    movingLeft = boolean(int(random(0, 100)%2));
  }

  if (movingLeft) {
    xchange+= xchangeSpeed;
  } else {
    xchange-= xchangeSpeed;
  }

  if (abs(xchange) >= (width - normal.width)/2) {

    movingLeft = !movingLeft;
  }


}

public void spin(PImage image) {
  for (int counter = 0; counter < 360; counter++) {
    translate(width/2-normal.width/2, height/2-normal.height/2);
    rotate(counter*TWO_PI/360);
    translate(-normal.width/2, -normal.height/2);
    image(image, width/2 - xchange, height/2 - ychange);
  }
}

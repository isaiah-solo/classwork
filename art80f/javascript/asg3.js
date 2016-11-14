var capture;

function setup() {
  createCanvas(640, 480);
  capture = createCapture(VIDEO);
  capture.size(320, 240);
  capture.hide();
}

function draw() {
  background (200);
  image(capture, 0, 0, 640, 480);
  textSize(50);
  text("discovery", mouseX, mouseY); 
  fill(100, 0, 100);
  text("truth", mouseX * 2, mouseY * 2); 
  fill(100, 100, 0);
  text("relief", mouseY, mouseX);
  fill(0, 100, 100);
  text("disappointment", mouseY / 2, mouseX / 2);
  fill(100, 100, 100);
  textSize(70);
  fill(0, 0, 0);
  text("You", 20, 70)
  fill(255, 255, 255);
  text("Tube", 140, 70);
}
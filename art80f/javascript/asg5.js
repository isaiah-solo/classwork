function setup() {
  createCanvas(640, 480);
  background (200);
}

function draw() {
  background (0, 0, 100);
  noStroke();
  if (mouseX < 280){
    fill((mouseX * 280)/255, 255 - ((mouseX * 280)/255), random(0, (mouseX * 280)/255));
    ellipse(mouseX, 240, (mouseX * 80)/280, (mouseX * 80)/280);
    fill(255, 255, 0);
    textSize(20);
    text("START --->", 10, 440);
    text("END", 560, 440);
  }
  if (mouseX >= 280 && mouseX < 480){
    fill(255, 255, 255, mouseX);
    rect(mouseX - 40, 200, 80, 80);
    fill(255, 255, 255, ((mouseX - 280) * 255)/200);
    rect(440, 0, 80, 80);
    rect(440, 100, 80, 80);
    rect(440, 300, 80, 80);
    rect(440, 400, 80, 80);
    fill(255, 255, 0, 255 - ((mouseX - 280) * 255)/200);
    textSize(20);
    text("START --->", 10, 440);
    fill(255, 255 - ((mouseX - 280) * 255)/200, 0);
    text("END", 560, 440);
  }
  if (mouseX >= 480) {
    fill(255, 255, 255);
    rect(440, 0, 80, 80);
    rect(440, 200, 80, 80);
    rect(440, 100, 80, 80);
    rect(440, 300, 80, 80);
    rect(440, 400, 80, 80);
    fill(255, 0, 0);
    textSize(20);
    text("END", 560, 440);
  }
  fill(255, 255, 0);
  rect(230, 0, 100, 480);
  triangle(210, 190, 230, 190, 230, 210);
  triangle(210, 290, 230, 290, 230, 270);
  triangle(350, 190, 330, 190, 330, 210);
  triangle(350, 290, 330, 290, 330, 270);
}
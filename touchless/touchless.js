
var id = 43;

var dataItems = [255];

function setup() {
  createCanvas(1000, 800);
  frameRate(60);

  var prep = [2, 2, 2];
  var parity = 0;
  for (var i = 0; i < 7; i++) {
    var j = ((id & (1 << i)) > 0) ? 1 : 0;
    prep.push(j);
    parity += (j == 1 ? 1 : 0);
  }
  prep.push(parity % 2 == 0 ? 1 : 0);
  console.log(prep);

  for (var i = 0; i < prep.length; i++) {
    if (prep[i] == 2) {
      dataItems.push(0);
      dataItems.push(0);
      dataItems.push(0);
      dataItems.push(255);
    }
    if (prep[i] == 1) {
      dataItems.push(0);
      dataItems.push(0);
      dataItems.push(255);
    }
    if (prep[i] == 0) {
      dataItems.push(0);
      dataItems.push(255);
    }
  }
  for (var i = 0; i <15; i++) {
    dataItems.push(0);
  }
  console.log(dataItems);
}



var b = 0;

function draw() {
  rectMode(CENTER);
  translate(width / 2, height / 2);
  background(0);

  b = b % dataItems.length;
  if (b <= dataItems.length) {
    bit = b % dataItems.length;
    var col = 0;
    fill(dataItems[bit] * 1);
    rect(0, 0, 150, 150);
  }
  b++;
}

var b2Vec2 = Box2D.Common.Math.b2Vec2;
var TILESIZE = 32;

function print(t) {
    var textArea = document.getElementById('log');
    textArea.innerHTML += t + "\n";
    textArea.scrollTop += 1000;
}

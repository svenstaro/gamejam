var b2Vec2 = Box2D.Common.Math.b2Vec2;
var TILESIZE = 32;

function print(t, no_newline) {
    var textArea = document.getElementById('log');
    if(no_newline === "true") {
        textArea.innerHTML += t;
    } else {
        textArea.innerHTML += t + "\n";
    }
    textArea.scrollTop += 1000;
}

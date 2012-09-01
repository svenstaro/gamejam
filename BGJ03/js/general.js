var b2Vec2 = Box2D.Common.Math.b2Vec2;
var TILESIZE = 32;
var LEFT_KEYS = [gamvas.key.LEFT, gamvas.key.A];
var RIGHT_KEYS = [gamvas.key.RIGHT, gamvas.key.D];
var JUMP_KEYS = [gamvas.key.UP, gamvas.key.W, gamvas.key.SPACE];

function print(t, no_newline) {
    var textArea = document.getElementById('log');
    if(no_newline === "true") {
        textArea.innerHTML += t;
    } else {
        textArea.innerHTML += t + "\n";
    }
    textArea.scrollTop += 1000;
}

function isKeyDown(keys) {
    for(var i = 0; i < keys.length; ++i) {
        if(gamvas.key.isPressed(keys[i])) return true;
    }
    return false;
}

function isKey(k, keys) {
    for(var i = 0; i < keys.length; ++i) {
        if(k == keys[i]) return true;
    }
    return false;
}

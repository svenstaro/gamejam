var b2Vec2 = Box2D.Common.Math.b2Vec2;
var DEBUG = false;
MUSIC = new Audio("snd/clocksong1.ogg"); // fuck the police
var TILESIZE = 32;
var LEFT_KEYS = [gamvas.key.LEFT, gamvas.key.A];
var RIGHT_KEYS = [gamvas.key.RIGHT, gamvas.key.D];
var JUMP_KEYS = [gamvas.key.UP, gamvas.key.W, gamvas.key.SPACE];
var BLAST_ANGLE = Math.PI / 4;
var BLAST_DISTANCE = 8;
var BLAST_RAYS = 10;
var WIND_FORCE_VERTICAL = 100;
var WIND_FORCE_HORIZONTAL = 200;
var WIND_FORCE_PLAYER = 150;
var RECOIL = 2;

function print() {
    var textArea = document.getElementById('log');

    for(var i = 0; i < arguments.length; i++) {
        try {
            if(typeof arguments[i] == "object")
                textArea.innerHTML += JSON.stringify(arguments[i]);
            else
                textArea.innerHTML += arguments[i];
        } catch(e) {
            textArea.innerHTML += arguments[i];
        }
    }
    textArea.scrollTop += 1000;
}

function println() {
    var x = [];
    for(var i = 0; i < arguments.length; i++) {
        x.push(arguments[i]);
    }
    x.push("\n");
    print.apply({}, x);
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

LAST_ID = 0;
function nextId(prefix) {
    LAST_ID++;
    return prefix + LAST_ID;
}

// returns 0 when failing
function tryParseInt(value) {
    try {
        return parseInt(value);
    } catch(e) {
        return 0;
    }
}

//deletes the first occurence of o from a
function deleteFromArray(a, o)
{
    for(var i = 0; i < a.length; ++i)
    {
        if(a[i] == o)
        {
            a.splice(i, 1);
            return;
        }
    }
}

function mousePosition() {
    var m = gamvas.mouse.getPosition();
    var s = gamvas.state.getCurrentState();
    m.x += s.camera.position.x - s.dimension.w / 2;
    m.y += s.camera.position.y - s.dimension.h / 2;
    return m;
}

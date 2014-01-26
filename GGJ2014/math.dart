part of game;

num lerp(num a, num b, num t) {
    return a + ((b - a) * t);
}

int randSign() => random.nextInt(2) == 0 ? -1 : 1;

int sign(x) => x > 0 ? 1 : -1;

num sinwave(num a) {
    return 0.5 - cos(a / PI *20)/2;
}

num angle01(num a, num b) {
    var result = (a - b).abs();
    if(result > .5) {
        result = 1 - result;
    }
    return result;
}

part of game;

class Wind implements Animatable{

    static num secondsPerWave = 8;
    static num windPower = 0.003;
    static num _time = 0;
    static num get power => sinwave(_time) * windPower;

    bool advanceTime(num time) {
        _time += time / secondsPerWave;
    }
}

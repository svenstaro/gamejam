part of game;

class Wind implements Animatable{
    
    num secondsPerWave = 8;
    static num _time = 0;
    static num get power => sinwave(_time);
    
    Wind({secondsPerWave: 4});
    
    bool advanceTime(num time) {
        _time += time / secondsPerWave;
    }
}

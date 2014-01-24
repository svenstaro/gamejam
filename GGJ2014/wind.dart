part of game;

class Wind implements Animatable{
    
    num secondsPerWave = 4;
    static num time = 0;
    static num get power => (sin(time) + 1) /2;
    
    bool advanceTime(num time) {
        Wind.time += time / (PI * secondsPerWave) * 8;
    }
}
part of game;

class Clock implements Animatable {
    static num secondsPerDay = 3;
    static num secondsPerYear = 60 * 5;
    
    static num day = 0;
    static num year = 0;
    
    static num get daytime => day % 1;
    
    bool advanceTime(num time) {
        day += time / secondsPerDay;
        year += time / secondsPerYear;
    }
}
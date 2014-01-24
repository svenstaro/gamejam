part of game;

class Clock implements Animatable {
    static num secondsPerDay = 3;
    static num secondsPerYear = 60 * 5;

    static num day = 0;
    static num year = 0;

    static num get daytime => day % 1;
<<<<<<< HEAD
    
    bool advanceTime(num time) {
        day += time / secondsPerDay;
        year += time / secondsPerYear;
=======

    Clock() {
        this.onEnterFrame.listen(_update);
    }

    _update(EnterFrameEvent e) {
        day += e.passedTime / secondsPerDay;
        year += e.passedTime / secondsPerYear;

        // print(day);
    }

    render(renderState) {
>>>>>>> 89063a65a801e2e694456ebdf0c1f343c94353d9
    }
}

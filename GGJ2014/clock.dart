
import 'package:stagexl/stagexl.dart';

class Clock extends DisplayObject {
    static num secondsPerDay = 3;
    static num secondsPerYear = 60 * 5;

    static num day = 0;
    static num year = 0;

    static num get daytime => day % 1;

    Clock() {
        this.onEnterFrame.listen(_update);
    }

    _update(EnterFrameEvent e) {
        day += e.passedTime / secondsPerDay;
        year += e.passedTime / secondsPerYear;

        // print(day);
    }

    render(renderState) {
    }
}

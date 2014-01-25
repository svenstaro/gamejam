part of game;

class AwesomeColor {
    num r;
    num g;
    num b;
    num a;

    int get hex => (a * 0xFF).round() * 0x01000000
                 + (r * 0xFF).round() * 0x00010000
                 + (g * 0xFF).round() * 0x00000100
                 + (b * 0xFF).round();

    AwesomeColor(this.r, this.g, this.b, [this.a = 1]);

    AwesomeColor.fromHex(int hex) {
        r = (hex & 0x00FF0000) / 0x00FF0000;
        g = (hex & 0x0000FF00) / 0x0000FF00;
        b = (hex & 0x000000FF) / 0x000000FF;
        a = (hex & 0xFF000000) / 0xFF000000;
    }

    static AwesomeColor lerpColor(AwesomeColor a, AwesomeColor b, num t) {
        return new AwesomeColor(lerp(a.r, b.r, t), lerp(a.g, b.g, t), lerp(a.b, b.b, t), lerp(a.a, b.a, t));
    }

    AwesomeColor operator +(AwesomeColor other) => new AwesomeColor(r + other.r, g + other.g, b + other.b, a * other.a);

    AwesomeColor operator *(var other) {
        if(other is AwesomeColor) {
            return new AwesomeColor(r * other.r, g * other.g, b * other.b, a);
        } else if(other is num) {
            return new AwesomeColor(r * other, g * other, b * other, a);
        } else {
            throw new Exception("Unknown type for AwesomeColor multiplication: [${other.runtimeType}].");
        }
    }

    String toString() {
        return "AwesomeColor(${r.toStringAsFixed(2)}, ${g.toStringAsFixed(2)}, ${b.toStringAsFixed(2)}, ${a.toStringAsFixed(2)})";
    }
}

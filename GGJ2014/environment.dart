part of game;

class Environment extends Animatable {

    static num _time = 0.0;
    static AwesomeColor sunColor1 = new AwesomeColor.fromHex(0xfff1d4ba); //sun at zenith
    static AwesomeColor sunColor2 = new AwesomeColor.fromHex(0xffb84e43); //sun at horizon

    static AwesomeColor moonColor1 = new AwesomeColor.fromHex(0xff082e4e); //moon at zenith
    static AwesomeColor moonColor2 = new AwesomeColor.fromHex(0xff031f37); //moon at horizon

    static var _seed = 1337.22;
    static var _rainGen;
    static var _windGen;

    Environment() {
        _rainGen = makeOctave2(simplex2, 3, 0.01);
        _windGen = makeOctave2(simplex2, 4, 0.4);
    }

    bool advanceTime(num time) {
        _time += time;


        var wp = _windGen(_seed, _time * 0.002);
        Wind.windPower = wp * 0.5 + 0.5;
        Wind.secondsPerWave = 10 - Wind.windPower * 8;
        //print("${Wind.secondsPerWave} und ${Wind.windPower}");

        amountOfRain = (_rainGen(_seed, _time * 0.01)).clamp(0,1) * 100;
    }

    static AwesomeColor getLightColorFor(Branch b) {
        var angle = (b.getTipPosition().rads + PI) / (2 * PI);
        
        //print(Clock.daytime);
        //print(angle);
        
        var light = sinwave(Clock.daytime * 2);
        var sunColor = AwesomeColor.lerpColor(sunColor1, sunColor2, 1-light);
        //var moonColor = AwesomeColor.lerpColor(moonColor1, moonColor2, 1-light);
        var moonColor = moonColor2;
        
        var dayLight = sinwave(Clock.daytime - .25);
        sunColor = sunColor * (dayLight + .3);
        moonColor = moonColor * (1-dayLight + .3);
        
        var sunAngle = Clock.daytime;
        var moonAngle = (Clock.daytime + .5) % 1;
        var sunPower = angle01(angle, sunAngle);
        var moonPower = angle01(angle, moonAngle);

        return (sunColor * sunPower) + (moonColor * moonPower);
    }
}

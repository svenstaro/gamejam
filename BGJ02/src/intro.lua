-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function Intro:__init()
    self.canvas = love.graphics.newCanvas()
    self.effect = love.graphics.newPixelEffect [[
	uniform float width;
	uniform float height;
    uniform float pixelSize;

	vec4 effect(vec4 color, Image tex, vec2 uv, vec2 xy) {
        float dx = pixelSize * (1.0 / width);
        float dy = pixelSize * (1.0 / height);
        vec2 coord = vec2(dx * floor(uv.x / dx), dy * floor(uv.y / dy));
        return vec4(texture2D(tex, coord).rgb, 1.0);
	}
	]]
    self.effect:send("width", love.graphics.getWidth())
    self.effect:send("height", love.graphics.getHeight())
    self.effect:send("pixelSize", 4)
end

function Intro:draw()
    self.canvas:clear()
    love.graphics.setCanvas(self.canvas)

    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.draw(resources.images.test, 0, 0)
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Press Escape to skip intro", 10, 10)



    love.graphics.setCanvas()
    love.graphics.setPixelEffect(self.effect)
    love.graphics.draw(self.canvas, 0, 0)
    love.graphics.setPixelEffect()
end

function Intro:keypressed(k, u)
    stack:pop()
end


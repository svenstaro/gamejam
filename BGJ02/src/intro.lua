-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function Intro:__init()
    self.canvas = love.graphics.newCanvas()
    self.effect = resources.shaders.pixelate
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

    love.graphics.setCanvas()
    love.graphics.setPixelEffect(self.effect)
    love.graphics.draw(self.canvas, 0, 0)
    love.graphics.setPixelEffect()

    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Press Escape to skip intro", 10, 10)
end

function Intro:skip()
    currentState = menu
end

function Intro:keypressed(k, u)
    self:skip()
end


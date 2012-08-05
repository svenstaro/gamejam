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
    love.graphics.setCanvas(self.canvas)

    love.graphics.setColor(17, 17, 17)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.test, 0, 0)

    love.graphics.setCanvas()
    love.graphics.setPixelEffect(self.effect)
    love.graphics.draw(self.canvas, 0, 0)
    love.graphics.setPixelEffect()

    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Press Escape to skip intro", 10, 10)
end

function Intro:skip()
    self:transitionTo(menu)
end

function Intro:keypressed(k, u)
    self:skip()
end


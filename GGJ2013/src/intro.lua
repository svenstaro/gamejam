-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function Intro:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Press Escape to skip intro", 10, 10)
end

function Intro:keypressed(k, u)
    stack:pop()
end

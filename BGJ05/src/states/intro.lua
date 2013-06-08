-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function Intro:onDraw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Woooosh.", 10, 10)
end

function Intro:onKeyPressed(k, u)
    stack:pop()
    stack:push(states.game)
end

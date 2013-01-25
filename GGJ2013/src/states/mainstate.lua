-- intro

require("core/gamestate")
require("core/resources")

MainState = class("MainState", GameState)

function MainState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("This is the game. Fuck yeah.", 10, 10)
end

function MainState:keypressed(k, u)
    stack:pop()
end

-- intro

require("core/gamestate")
require("core/resources")

IntroState = class("IntroState", GameState)

function IntroState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("Press Escape to skip intro", 10, 10)
end

function IntroState:keypressed(k, u)
    stack:pop()
    stack:push(main)
end

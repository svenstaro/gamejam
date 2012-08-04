-- intro

require("util/gamestate")
require("util/resources")

Game = class("Game", GameState)

function Game:__init()
end

function Game:draw()
    love.graphics.setBackgroundColor(0, 0, 0)
    love.graphics.clear()

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)

    local s = "Game state"
    love.graphics.print(s, love.graphics.getWidth() - love.graphics.getFont():getWidth(s) - 10, 10)
end

function Game:keypressed(k, u)

end


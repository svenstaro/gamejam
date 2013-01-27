-- intro

require("core/gamestate")
require("core/resources")

GameOverState = class("GameOverState", GameState)

function GameOverState:__init()
    self.lifetime = 0

    self.heartbeat = resources:makeSound("heartbeat")
end

function GameOverState:update(dt)
    self.lifetime = self.lifetime + dt
end

function GameOverState:draw()
    local w, h = love.graphics.getWidth(), love.graphics.getHeight()
    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.failure, w / 2, h / 2, 0, 4, 4, resources.images.failure:getWidth() / 2, resources.images.failure:getHeight() / 2)
    if self.lifetime > 3 then
        love.graphics.setFont(resources.fonts.handBig)
        love.graphics.setColor(255, 255, 255, math.sin((self.lifetime - 3) * 4) * 55 + 150)
        local t = "Press any key to continue"
        love.graphics.print(t, w / 2 - love.graphics.getFont():getWidth(t) / 2, h / 2 + 170)
    end
end

function GameOverState:keypressed(k, u)
    stack:pop()
    stack:push(menu)
end

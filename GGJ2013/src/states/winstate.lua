-- intro
require("core/listmenu")
require("core/gamestate")
require("core/resources")

WinState = class("WinState", GameState)

function WinState:__init()
    self.lifetime = 0
end

function WinState:update(dt)
    self.lifetime = self.lifetime + dt
end

function WinState:draw()
    local w, h = love.graphics.getWidth(), love.graphics.getHeight()
    love.graphics.setFont(resources.fonts.handBig)
    love.graphics.setColor(255, 255, 255, 255)
    local t = "       Thanks for playing! \n \n What are you going to do now?"
    love.graphics.print(t, w / 2 - 210 , h / 2)
end

function WinState:keypressed()
    if self.lifetime > 2 then
        stack:pop()
        stack:push(menu)
    end
end

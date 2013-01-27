-- intro

require("core/gamestate")
require("core/resources")

GameOverState = class("GameOverState", GameState)

function GameOverState:__init()
    self.lifetime = 0
    self.counter = 0
    self.kill = true
    self.eating = resources:makeSound("final_monster_1")
    self.eating2 = resources:makeSound("final_monster_2")
    
    require("core/gamestate")
    require("core/resources")
    require("core/listmenu")

end

function GameOverState:update(dt)
    self.lifetime = self.lifetime + dt
    if self.counter < 1 then
        self.eating:setVolume(0.2)
        self.eating:play()
        self.counter = self.counter + 1
    end
    if self.kill then
        self.eating2:setVolume(1)
        self.eating2:play()
        self.kill = false
    end 
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

function GameOverState:keypressed()
    self.eating:stop()
    self.eating2:stop()
    
    self.counter = 0
    self.kill = true
    
    if self.lifetime > 2 then
        stack:pop()
        stack:push(menu)
    end
end

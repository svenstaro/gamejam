-- a patient's file that is being drawn above the normal game

require("core/gamestate")
require("core/resources")

FileState = class("FileState", GameState)

FileState.drawBelow = true

function FileState:__init()
	self.headline = "- empty headline -"
    self.text     = "- empty file -"
end

function FileState:start()
    self.fade = 0
    self.target = 1
end

function FileState:update(dt)
    if self.target == 0 then
        self.fade = self.fade - dt * 3

        if self.fade < 0 then
            stack:pop()
        end
    else
        self.fade = self.fade + dt * 3
    end

    if self.fade > 1 then self.fade = 1 end
    if self.fade < 0 then self.fade = 0 end
end

function FileState:draw()
    love.graphics.push()
    love.graphics.translate(- (1 - self.fade) * (1 - self.fade) * resources.images.akte:getWidth(), 0)

    love.graphics.setColor(0, 0, 0, 200 * self.fade)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.akte, 0, 0)

    love.graphics.setColor(0, 0, 0)
    love.graphics.setFont(resources.fonts.handBig)
    love.graphics.print(self.headline, 30, 25)

    love.graphics.setFont(resources.fonts.handSmall)
    love.graphics.printf(self.text, 30, 65, 400, "left")
    love.graphics.printf("Press [E] to continue", 30, 575, 420, "center")

    love.graphics.pop()
end

function FileState:keypressed(k, u)
    if k == " " or k == "e" or k == "escape" then
        self.target = 0
    end
end

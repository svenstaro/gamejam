-- a patient's file that is being drawn above the normal game

require("core/gamestate")
require("core/resources")

FileState = class("FileState", GameState)

FileState.drawBelow = true

function FileState:__init()
    self.text = "- empty file -"
end

function FileState:draw()
    love.graphics.setColor(0, 0, 0, 100)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    love.graphics.setColor(255, 255, 255)
    love.graphics.draw(resources.images.akte, 0, 0)

    love.graphics.setColor(0, 0, 0)
    love.graphics.setFont(resources.fonts.handBig)
    love.graphics.print("Patient sowieso", 30, 25)

    love.graphics.setFont(resources.fonts.handSmall)
    love.graphics.printf(self.text, 30, 95, 420, "left")
    love.graphics.printf("Press [E] to continue", 30, 540, 420, "center")
end

function FileState:keypressed(k, u)
    if k == " " or k == "e" or k == "escape" then
        stack:pop()
    end
end

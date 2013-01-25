-- a patient's file that is being drawn above the normal game

require("core/gamestate")
require("core/resources")

FileState = class("FileState", GameState)

FileState.drawBelow = true

function FileState:__init()
    self.number = "0523"
end

function FileState:draw()
    love.graphics.setColor(0, 0, 0, 100)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print("This is the patient's file " .. self.number, 100, 210)
    love.graphics.print("This should look better", 100, 250)
    love.graphics.print("Press any key to continue", 100, 290)
end

function FileState:keypressed(k, u)
    stack:pop()
end

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
    love.graphics.setFont(resources.fonts.normal)
    love.graphics.print(self.text, 100, 200)
    love.graphics.print("Press any key to continue", 100, 400)
end

function FileState:keypressed(k, u)
    stack:pop()
end

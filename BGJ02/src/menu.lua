require("util/gamestate")
require("util/resources")
require("util/listmenu")

Menu = class("Menu", GameState)

function Menu:__init()
    self.menu = ListMenu({"Start Game", "Credits", "Quit"},
        Vector(100, 340),
        140,
        resources.fonts.huge,
        60)

    self.menu.callback = function(index, text) 
        if index == 1 then
            self:transitionTo(game)
        elseif index == 3 then
            currentState = nil
        end
    end
end

function Menu:draw()
    love.graphics.setColor(17, 17, 17)
    love.graphics.rectangle("fill", 0, 0, love.graphics.getWidth(), love.graphics.getHeight())

    self.menu:draw()
end

function Menu:keypressed(k, u)
    self.menu:keypressed(k, u)

    if k == "escape" then
        currentState = nil
    end
end


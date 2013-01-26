-- menu

require("core/gamestate")
require("core/resources")
require("core/listmenu")

MenuState = class("MenuState", GameState)

function MenuState:__init()
    self.lifetime = 0

    self.menu = ListMenu({"START GAME", "CREDITS", "QUIT"}, 200, 200,
                         150, resources.fonts.huge, 100)

    self.menu.callback = function(index, text)
        if index == 1 then
            stack:pop()
            stack:push(main)
        elseif index == 2 then
            stack:push(credits)
        elseif index == 3 then
            stack:pop()
        end
    end
end

function MenuState:update(dt)
    self.lifetime = self.lifetime + dt

    self.menu:update(dt)
end

function MenuState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(200, 200, 200)
    love.graphics.setFont(resources.fonts.huge)
    love.graphics.print("7th Sense", 50, 50)
    self.menu:draw()
end

function MenuState:keypressed(k, u)
    self.menu:keypressed(k, u)

    if k == "escape" then
        stack:pop() 
    end
end

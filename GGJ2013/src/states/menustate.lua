-- menu

require("core/gamestate")
require("core/resources")
require("core/listmenu")

MenuState = class("MenuState", GameState)

function MenuState:__init()
    self.lifetime = 0


    self.heart = Sprite(resources.images.heart)
    self.heart:setSounds({"heartbeat"}, 1, 0)
    self.heart.scale = 1
    self.heart.x = love.graphics.getWidth() / 2 + 25
    self.heart.y = love.graphics.getHeight() / 2

    self.menu = ListMenu({"CREDITS", "START GAME", "QUIT"}, 200, 650,
                         800, resources.fonts.handBig, 50)

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

    self.heart.scale = ((self.lifetime + 0.08) % 1) > 0.1 and 1 or 1.05
    self.heart:update(dt)
end

function MenuState:draw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(200, 200, 200)
    love.graphics.setFont(resources.fonts.huge)
    --love.graphics.print("7th Sense", 50, 50)
    self.menu:draw()
    self.heart:draw()
end

function MenuState:keypressed(k, u)
    self.menu:keypressed(k, u)

    if k == "escape" then
        stack:pop() 
    end
end

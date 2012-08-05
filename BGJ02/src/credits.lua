require("util/gamestate")
require("util/resources")

require("intro")

Credits = class("Credits", GameState)

function Credits:draw()
    love.graphics.setColor(255, 255, 255)

    local c = love.graphics.getWidth() / 2

    centerText("Programming", c, 100, resources.fonts.huge)
    centerText("Paul 'opatut' Bienkowski", c, 160, resources.fonts.normal)
    centerText("Sascha 'FlaSh.G' Graeff", c, 190)
    centerText("Sven-Hendrik 'svenstaro' Haase", c, 220)
    

    centerText("Music", c, 300, resources.fonts.huge)
    centerText("The Hamster Alliance", c, 360, resources.fonts.normal)
    centerText("http://thehamsteralliance.com/", c, 390)
end

function Credits:keypressed(k, u)
    self:transitionTo(menu, "left")
end


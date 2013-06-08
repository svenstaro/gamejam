-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function Intro:onDraw()
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.setColor(255, 255, 255)

    love.graphics.clear()
    love.graphics.setFont(resources.fonts.normal)

    local w = SIZE.x
    local scale = 0.3
    local s = resources.images.key_f:getWidth() * scale
    local y = SIZE.y - 0.5 * s - 10

    love.graphics.draw(resources.images.key_f, 10, y, 0, scale, scale, 0, s/scale*0.5)
    love.graphics.print("Toggle fullscreen", 20+s, y - love.graphics.getFont():getHeight() / 2)

    love.graphics.draw(resources.images.key_m, 170, y, 0, scale, scale, 0, s/scale*0.5)
    love.graphics.print("Mute/unmute sound", 180+s, y - love.graphics.getFont():getHeight() / 2)

    love.graphics.draw(resources.images.key_esc, 330, y, 0, scale, scale, 0, s/scale*0.5)
    love.graphics.print("Pause/Quit",340+s, y - love.graphics.getFont():getHeight() / 2)
end

function Intro:onKeyPressed(k, u)
    stack:pop()
    stack:push(states.game)
end

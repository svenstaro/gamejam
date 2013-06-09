-- intro

require("util/gamestate")
require("util/resources")

Intro = class("Intro", GameState)

function Intro:__init()
    self.world = World()
    local lampstatic = LampStatic()
    lampstatic.position = Vector(SIZE.x / 2 - 100, -SIZE.y*0.6)
    self.world:add(lampstatic)
end

function Intro:onUpdate(dt)
    self.world:update(dt)
end

function Intro:onDraw()
    LIGHT_CANVAS:clear(0, 0, 0)
    love.graphics.setBackgroundColor(17, 17, 17)
    love.graphics.clear()

    love.graphics.push()
    love.graphics.translate(SIZE.x/2, SIZE.y)
    self.world:draw()
    love.graphics.pop()

    drawLights()

    love.graphics.setColor(255, 255, 255)
    love.graphics.setFont(resources.fonts.normal)

    local scale = 0.5
    local s = resources.images.key_f:getWidth() * scale
    local x = SIZE.x / 2
    local y = SIZE.y - 5 * (s * 1.5)

    love.graphics.draw(resources.images.key_f, x-0.25*s, y, 0, scale, scale, s/scale, s/scale*0.5)
    love.graphics.print("Toggle fullscreen", x+0.25*s, y - love.graphics.getFont():getHeight() / 2)
    y = y + s * 1.5

    love.graphics.draw(resources.images.key_m, x-0.25*s, y, 0, scale, scale, s/scale, s/scale*0.5)
    love.graphics.print("Mute/unmute sound", x+0.25*s, y - love.graphics.getFont():getHeight() / 2)
    y = y + s * 1.5

    love.graphics.draw(resources.images.key_esc, x-0.25*s, y, 0, scale, scale, s/scale, s/scale*0.5)
    love.graphics.print("Pause/Quit", x+0.25*s, y - love.graphics.getFont():getHeight() / 2)
    y = y + s * 1.5

    love.graphics.draw(resources.images.key_space, x-0.25*s, y, 0, scale, scale, resources.images.key_space:getWidth(), s/scale*0.5)
    love.graphics.print("Start game", x+0.25*s, y - love.graphics.getFont():getHeight() / 2)
    y = y + s * 1.5
end

function Intro:onKeyPressed(k, u)
    if k == " " then
        stack:pop()
        stack:push(states.game)
    elseif k == "escape" then
        stack:pop()
    end
end

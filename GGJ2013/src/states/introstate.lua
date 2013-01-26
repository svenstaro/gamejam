-- intro

require("core/gamestate")
require("core/resources")

IntroState = class("IntroState", GameState)

function IntroState:__init()
    self.ggj_circle = {{37, 33, 98}, {61, 45, 100}, {87, 55, 105}, {111, 80, 138}, {140, 109, 175}, {138, 145, 168}, {135, 187, 158}, {204, 220, 87}, {245, 237, 49}}

    self.lifetime = 0
end

function IntroState:update(dt)
    self.lifetime = self.lifetime + dt
end

function IntroState:draw()
    local w, h = love.graphics.getWidth(), love.graphics.getHeight()

    if self.lifetime < 5 then
        -- 8 circles, 1 background color
        love.graphics.setBackgroundColor(self.ggj_circle[9])
        love.graphics.clear()

        local x, y = w / 2, h / 2 + 50

        for i = 8, 1, -1 do --math.min(math.round(self.lifetime * 8), 8), 1, -1 do
            love.graphics.setColor(unpack(self.ggj_circle[i]))
            love.graphics.circle("fill", x, y, (55 + 35 * i * math.sin(math.pi / 2 * math.min(1, self.lifetime - 0.5))))
        end

        love.graphics.setColor(255, 255, 255, math.max(0, math.min(1, self.lifetime - 2)) * 255)
        love.graphics.draw(resources.images.ggj_logo, x - 160, y - 285)

        local d = math.max(0, math.min(1, self.lifetime - 2))
        love.graphics.setColor(0, 0, 0)
        love.graphics.rectangle("fill", 0, 0, w, y - 380)
        love.graphics.rectangle("fill", 0, y + 280, w, h)
        love.graphics.rectangle("fill", 0, 0, x - 230, h)
        love.graphics.rectangle("fill", x + 230, 0, w, h)
    elseif self.lifetime < 8 then
        love.graphics.setBackgroundColor(17, 17, 17)
        love.graphics.setColor(255, 255, 255)

        love.graphics.clear()
        love.graphics.setFont(resources.fonts.normal)
        local t = "Best with headphones"
        love.graphics.print(t, w / 2 - love.graphics.getFont():getWidth(t) / 2, h / 2)
    else
        stack:pop()
        stack:push(main)
    end
end

function IntroState:keypressed(k, u)
    stack:pop()
    stack:push(main)
end

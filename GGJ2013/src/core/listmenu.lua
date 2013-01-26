require("core/helper")
require("core/vector")
require("core/resources")

ListMenu = class("ListMenu")

function ListMenu:__init(entries, x, y, width, font, line_height)
    self.entries = entries or {}
    self.x = x
    self.y = y
    self.width = width or 140
    self.font = font or love.graphics.getFont()
    self.line_height = line_height or 30

    self.callback = function(index, text) end -- empty callback

    self.selected = 1
    self.prev_selected = 1

    self.selected_with_mouse = false

    self.last_mouse_position = Vector(0,0)
end

function ListMenu:keypressed(k, u)
    if k == "up" then
        self.selected = self.selected - 1
        self.selected_with_mouse = false
    elseif k == "down" then
        self.selected = self.selected + 1
        self.selected_with_mouse = false
    elseif k == "return" or k == " " or k == "right" then
        self.callback(self.selected, self.entries[self.selected])
    end
    
    if self.selected > #self.entries then 
        self.selected = 1
    elseif self.selected < 1 then
        self.selected = #self.entries
    end
end

function ListMenu:update(dt)
    if self.prev_selected ~= self.selected then
        resources:makeSound("heartbeat"):play()
    end
    self.prev_selected = self.selected

    local x,y = love.mouse.getPosition()
    local mouse_position = Vector(x,y)
    --hardcoded as fuck
    local offset = 8
    local side_padding = 20

    if mouse_position ~= self.last_mouse_position then
        if x >= self.x - side_padding
        and x <= self.x + self.width + side_padding then
            for i = 1, #self.entries do
                if y >= self.y + self.line_height * (i-1) - offset
                and y <= self.y + self.line_height * i - offset then
                    self.selected = i
                    self.selected_with_mouse = true
                    break
                end
            end
        end
        self.last_mouse_position = mouse_position
    end

    if self.selected_with_mouse and love.mouse.isDown('l') then
        self.callback(self.selected, self.entries[self.selected])
    end
end

function ListMenu:draw()
    love.graphics.setFont(self.font)
    for i = 1,#self.entries do
        if i == self.selected then v = 255 else v = 128 end
        love.graphics.setColor(255, 255, 255, v)

        local p = self:getLinePosition(i)
        local s = self.entries[i]:lower()
        if i == self.selected then s = s:upper() end
        love.graphics.print(s, p.x, p.y)
    end
end

function ListMenu:getLinePosition(i)
    return Vector(self.x, self.y + self.line_height * (i - 1))
end

require("util/helper")

ListMenu = class("ListMenu")

function ListMenu:__init(entries, position, width, font, line_height)
    self.entries = entries or {}
    self.position = position
    self.width = width or 140
    self.font = font or love.graphics.getFont()
    self.line_height = line_height or 30

    self.callback = function(index, text) end -- empty callback

    self.selected = 1
end

function ListMenu:keypressed(k, u)
    if k == "up" then
        self.selected = self.selected - 1
    elseif k == "down" then
        self.selected = self.selected + 1
    elseif k == "return" or k == " " or k == "right" then
        self.callback(self.selected, self.entries[self.selected])
    end
    
    if self.selected > #self.entries then 
        self.selected = 1
    elseif self.selected < 1 then
        self.selected = #self.entries
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
    return Vector(self.position.x, self.position.y + self.line_height * (i - 1))
end

-- gamestack

require("core/helper")

GameStack = class("GameStack")

function GameStack:__init()
    self.states = {}
end

function GameStack:current()
    if #self.states > 0 then
        return self.states[#self.states]
    else
        return nil
    end
end

function GameStack:push(state)
    if self:current() then self:current():stop() end
    table.insert(self.states, state)
    self:current():start()
end

function GameStack:quit()
    self:pop(#self.states)
end

function GameStack:pop(number)
    if not self:current() then return end

    self:current():stop()
    table.remove(self.states, #self.states)

    if number and number > 1 then
        self:pop(number - 1)
    elseif self:current() then
        self:current():start()
    end
end

function GameStack:update(dt)
    if self:current() then
        self:current():update(dt)
    else
        love.event.push("quit")
    end
end

function GameStack:draw()
    if self:current() then self:current():draw() end
end

function GameStack:keypressed(k, u)
    if self:current() then self:current():keypressed(k, u) end
end

function GameStack:keyreleased(k, u)
   if self:current() then self:current():keyreleased(k, u) end
end

function GameStack:mousepressed(x, y, button)
   if self:current() then self:current():mousepressed(x, y, button) end
end

function GameStack:mousereleased(x, y, button)
   if self:current() then self:current():mousereleased(x, y, button) end
end

-- game state

require("util/helper")

GameState = class("GameState")
GameState.transition = 0
GameState.lifetime = 0

function GameState:__init()end

function GameState:doUpdate(dt)
    self.lifetime = self.lifetime + dt
    if self.transition > 0 then
        self.transitionState:update(dt)
        if self.transition == 1 then
            -- done, switch states
            setState(self.transitionState)
            self.transition = 0
        end
    end
    self:update(dt)
end

function GameState:doDraw()
    local i = - 1
    if self.transitionMode == "right" then i = 1 end

    if self.transition > 0 then
        love.graphics.push()
        love.graphics.translate(- i * self.transition * love.graphics.getWidth(), 0)
    end
    self:draw()
    if self.transition > 0 then
        love.graphics.pop()
        love.graphics.push()
        love.graphics.translate(i * (1 - self.transition) * love.graphics.getWidth(), 0)
        self.transitionState:draw()
        love.graphics.pop()
    end
end

function GameState:draw()end
function GameState:update(dt)end
function GameState:start()end
function GameState:stop()end
function GameState:keypressed(k, u)end
function GameState:mousepressed(x, y, mb)end

function GameState:transitionTo(state, mode, duration)
    -- transitions to another game state
    self.transition = 0
    self.transitionState = state
    self.transitionMode = mode or "right"
    tween(duration or 0.5, self, { transition = 1 }, "outQuint")
    resources.audio.swipe:play()
end

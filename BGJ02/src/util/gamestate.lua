-- game state

require("util/helper")
local tween = require("externals/tween/tween")

GameState = class("GameState")

function GameState:__init()end

function GameState:update(dt)end
function GameState:draw()end
function GameState:start()end
function GameState:stop()end
function GameState:keypressed(k, u)end
function GameState:mousepressed(x, y, mb)end

--function GameState:transitionTo(mode, state, duration)
--    -- transitions to another game state
--
--    self.transition = -1
--    tween(duration or 1, self, { transition = 1 })
--end

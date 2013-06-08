-- settings

require("util/helper")

Settings = class("Settings")

function Settings:__init()
    self.filename = "config.lua"
    self.data = {}
end

function Settings:get(key, default)
    if self.data[key] ~= nil then
        return self.data[key]
    else
        self:set(key, default)
        return default
    end
end

function Settings:inc(key, d)
    if not d then d = 1 end
    self:set(key, self:get(key, 0) + d)
end

function Settings:set(key, value)
    self.data[key] = value
end

function Settings:save()
    savefile = love.filesystem.newFile(self.filename)
    savefile:open("w")
    savefile:write("s = {}\n")
    for k,v in pairs(self.data) do
        local var = v
        if type(var) == "string" then var = "\"" .. var .. "\"" end -- escape strings
        savefile:write("s['" .. k .. "'] = " .. tostring(var) .. "\n")
    end
    savefile:write("return s\n")
    savefile:close()
end

function Settings:load()
    ok, chunk = pcall(love.filesystem.load, self.filename)
    if not ok then return false end

    ok, contents = pcall(chunk)
    if not ok then return false end

    for k,v in pairs(contents) do self:set(k, v) end
    return true
end

--[[
This script is used to config the interpretoer,
such as adding the search patch.
--]]

function DebugLogger(message, ...)
    print('----- DEBUG:', message, ...)
end

-- AddPath to add a path to the search for loading file
local function AddPath(path)
    package.path = package.path..";"..path
end

AddPath("D:\\GitHub\\Lua\\LoadAssets\\?.lua")
-- AddPath("D:\\GitHub\\Lua\\LoadAssets\\?.dll")
AddPath(".\\luaScript\\?.lua")

-- store the original 'loadfile', replace it with loadfileInPath
old_loadfile = loadfile

-- modify the loadfile function,
-- enable the function the ability to search through the package.path, 
-- not just in current folder.
-- notice that if the fileName end with ".lua", 
-- ".lua" will be removed.
loadfile = function(filePath)
    -- remove the ".lua" in the end.
    fileName = string.match(filePath, "(%w-).lua$") or filePath
    
    -- use the searchpath to find the actual path for the file
    fileName = package.searchpath(fileName, package.path)
    
    if fileName then
        return old_loadfile(fileName)
    else
        error("no such file:\n\t"..filePath)
    end
end

luaSock = require("LuaSocket")

print('Testing...')
print('try to startup...')
assert(luaSock.Startup(2, 2))
print('startup success')

print('try to cleanup...')
assert(luaSock.Cleanup())
print('cleanup success')
print('Test end')

print('Starup')
assert(luaSock.Startup(2, 2))
print('startup success')

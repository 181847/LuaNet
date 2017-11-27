--[[
This script is used to config the interpretoer,
such as adding the search patch.
--]]
print("Start Initialization")

function DebugLogger(message, ...)
    print('----- DEBUG:', message, ...)
end

-- AddPath to add a path to the search for loading file
local function AddPath(path)
    package.path = package.path..";"..path
end

-- add path for the C module such as the dll
local function AddCPath(path)
    package.cpath = package.cpath..";"..path
end

-- print the key, value in a table
function PrintTable(t)
    for k, v in pairs(t) do print(k, v) end
end

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

print("Initialization Completed")

local startup = assert(loadfile("StartUpFile.lua"))

if startup then
    assert(startup())
end

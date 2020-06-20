local inspect = require 'inspect'

major, minor, build = hostgetversion()
print("The host version is ", major, minor, build)
print("The lua interpreter version is ", _VERSION)

function mood(b)
	if (b and major > 0) then
		return 'mood-happy (행복회로 다시 시도 zlzzlz킼킼)'
	elseif (major == 0) then
		return 'mood_confused'
	else
		return 'mood-sad'
	end
end

local array = {"Lua", "Tutorial", "좋음"}

function elementIterator (collection)

   local index = 0
   local count = #collection
	
   -- The closure function is returned
	
   return function ()
      index = index + 1
		
      if index <= count
      then
         -- return the current element of the iterator
         return collection[index]
      end
		
   end
	
end

for element in elementIterator(array)
do
   print(element)
end

local mytable = setmetatable({key1 = "value1"}, { __index = { key2 = "metatablevalue" } })

print(mytable.key1, mytable.key2, mytable.asdfj)

function locals()
  local variables = {}
  local idx = 1
  while true do
    local ln, lv = debug.getlocal(2, idx)
    if ln ~= nil then
      variables[ln] = lv
    else
      break
    end
    idx = 1 + idx
  end
  return variables
end

canyouseeme=1985

print(inspect(locals()))



print('---')
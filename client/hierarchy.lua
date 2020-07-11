local inspect = require 'inspect'

major, minor, build = hostgetversion()
print("Hierarchy: The host version is ", major, minor, build)
print("The lua interpreter version is ", _VERSION)

selectSurface = HostLoadBmp('select.bmp')
monsterSurface = HostLoadBmp('monster.bmp')
screenSurface = nil

function gettileobjects(b)
	--print("2 Screen surface", screenSurface)
	HostBlitTileObject(screenSurface, selectSurface, 2, 3)
	HostBlitTileObject(screenSurface, monsterSurface, 3, 4)
	HostBlitTileObject(screenSurface, monsterSurface, 4, 5)
	return '^__^;'
end


print(selectSurface)

function setscreensurface(surface)
	screenSurface = surface
	print("1 Screen surface", screenSurface)
end


FacerUtil = { }

FacerUtil.ini_data_default = {
	Settings = {
		Name = "CBSB",
		Scenario = "cb.sb",
		Side = 2,
		IsSpectator = false,
		Color = 0,
		CustomLoadScreen = "easb.pcx",
		AIPlayers = 2,
		Host = true,
		Seed = 1208,
		GameMode = 1,
		GameID = 9001,
		Crates = false,
		ShortGame = true,
		Superweapons = true,
		MCVRedeploy = true,
		BridgeDestory = true,
		BuildOffAlly = false,
		MultiEngineer = false,
		UnitCount = 0,
		GameSpeed = 5,
		Credits = 10000,
		FogOfWar = 'No'
	},

	HouseHandicaps = { },

	HouseCountries = { },

	HouseColors = { },

	IsSpectator = { },

	SpawnLocations = { }
}

-- lua-users.org/wiki/CopyTable
function FacerUtil.table_deepcopy(orig)
	local deepcopy
	deepcopy = function (orig_)
	    local orig_type = type(orig_)
	    local copy
	    if orig_type == 'table' then
	        copy = {}
	        for orig_key, orig_value in next, orig_, nil do
	            copy[deepcopy(orig_key)] = deepcopy(orig_value) end
	        setmetatable(copy, deepcopy(getmetatable(orig_)))
	    else copy = orig_ end
	    return copy
	end

	return deepcopy(orig)
end

function FacerUtil.write_table_to_ini(iniobj, data)
	for i, v in pairs(data) do
		iniobj:setValue(i, nil, nil)

		for key, value in pairs(v) do
			iniobj:setValue(i, key, tostring(value))
		end
	end
end

function FacerUtil.generate_table_sequence(len)
	local ret = { }
	for i = 1, len do
		table.insert(ret, i) end
	return ret
end

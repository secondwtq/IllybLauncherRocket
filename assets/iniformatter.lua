
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

	SpawnLocations = { },

	IsSpectator = { }
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

-- { 1, 2, 3 } -> { 1 = default_value, 2 = default_value, 3 = default_value }
--	'default_value' defaults false
function FacerUtil.array_to_hash(src, default_value)
	if default_value == nil then default_value = false end

	local ret = { }
	for i, v in pairs(src) do 
		ret[v] = default_value end
	return ret
end

function launch()
	local data = FacerUtil.table_deepcopy(FacerUtil.ini_data_default)

	local num_player = global_data.num_players
	local num_aiplayer = global_data.num_players - 1

	data.Settings.Name = "CBSB"
	data.Settings.CustomLoadScreen = "easb.pcx"
	data.Settings.Seed = 1208

	data.Settings.AIPlayers = num_aiplayer

	data.Settings.ShortGame = global_data.battle_settings.short_game
	data.Settings.Crates = global_data.battle_settings.crates
	data.Settings.MCVRedeploy = global_data.battle_settings.mcv_redeploy
	data.Settings.Superweapons = global_data.battle_settings.superweapons
	data.Settings.BuildOffAlly = global_data.battle_settings.build_off_ally

	data.Settings.GameSpeed = global_data.battle_settings.game_speed
	data.Settings.Credits = global_data.battle_settings.credits
	data.Settings.UnitCount = global_data.battle_settings.unit_count

	finish_place_n_color_for_player(1)
	data.SpawnLocations['Multi1'] = tonumber(global_data.player[1].place) - 1
	data.Settings.Color = tonumber(global_data.player[1].color) - 1
	data.Settings.Side = sideid_for_player(1)

	local startid = 2
	for i = 2, num_player do
		local handicap = tonumber(global_data.player[i].name)
		if handicap ~= nil then -- do not count 'None' players

			finish_place_n_color_for_player(i)

			global_data.player[i].actual_id = startid
			local prefix = 'Multi' .. startid

			data.HouseHandicaps[prefix] = handicap
			data.SpawnLocations[prefix] = tonumber(global_data.player[i].place) - 1
			data.HouseColors[prefix] = tonumber(global_data.player[i].color) - 1
			data.HouseCountries[prefix] = sideid_for_player(i)

			startid = startid + 1
		end
	end

	-- create INI object and write to it
	local ini = Facer.CSimpleIniAWrap()
	FacerUtil.write_table_to_ini(ini, data)
	local ini_string = ini:saveToString()
	print(ini_string)
end


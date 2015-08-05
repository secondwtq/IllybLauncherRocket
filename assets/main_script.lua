
dofile('iniformatter.lua')

function load_font()

	-- rocket:LoadFontFace('Delicious-Roman.otf')
	-- rocket:LoadFontFace('Delicious-Italic.otf')
	-- rocket:LoadFontFace('Delicious-Bold.otf')
	-- rocket:LoadFontFace('Delicious-BoldItalic.otf')

	rocket:LoadFontFace('SourceHanSansCN-Regular.otf')
	-- rocket:LoadFontFace('SourceHanSansCN-Normal.otf')
	-- rocket:LoadFontFace('SourceHanSansCN-Medium.otf')

end

math.randomseed(os.time())
load_font()
maincontext = rocket.contexts["main"]
document = maincontext:LoadDocument("main.rml")
document:Show()

global_data = {
	num_players = 6,
	num_sides = 3,
	colors = {
		'rgb(255, 0, 0)',
		'rgb(0, 255, 0)',
		'rgb(0, 0, 255)',
		'rgb(255, 255, 0)',
		'rgb(0, 255, 255)',
		'rgb(255, 0, 255)',
		'rgb(255, 255, 255)',
		'rgb(0, 0, 0)'
	},
	remaining_place = { 1, 2, 3, 4, 5, 6, 7 },
	remaining_color = { 1, 2, 3 },
	battle_settings = {
		mcv_redeploy = false,
		build_off_ally = true,
		superweapons = true,
		crates = true,
		short_game = true,

		game_speed = 5,
		credits = 10000,
		unit_count = 1
	},

	player = {

	},

	datasources = { }
}

for i = 1, 8 do
	global_data.player[i] = {
		enabled = false,
		name = 'easy',
		side = 'Random',
		color = '-',
		place = '-',
		ally = '-',

		cache_place = nil,
		cache_color = nil,
		actual_id = nil
	}
end

local srclocation = DataSource.new("srclocation")
srclocation.GetNumRows = function (table_name)
	if table_name == 'placeholder' then return global_data.num_players + 1 end
	if table_name:sub(1, 5) == 'place' then
		local player_id = tonumber(table_name:sub(6, 6))
		if global_data.player[player_id].place ~= '-' then
			return #global_data.remaining_place + 2
		end
		return #global_data.remaining_place + 1
	end
end
srclocation.GetRow = function (table_name, row_index, columns)
	if table_name == 'placeholder' then
		if row_index == 0 then return { '-', '-' } end
		return { tostring(row_index), tostring(row_index) }
	elseif table_name:sub(1, 5) == 'place' then
		local player_id = tonumber(table_name:sub(6, 6))
		if row_index == 0 then return { '-', '-' } end

		if global_data.player[player_id].place ~= '-' then
			if global_data.player[player_id].cache_place == nil then
				local cache = { }
				for i, p in pairs(global_data.remaining_place) do
					table.insert(cache, p)
				end
				table.insert(cache, global_data.player[player_id].place)
				table.sort(cache, function (x, y) return tonumber(x) < tonumber(y) end)
				global_data.player[player_id].cache_place = cache
			end

			return { global_data.player[player_id].cache_place[row_index], global_data.player[player_id].cache_place[row_index] }
		else
			return { global_data.remaining_place[row_index], global_data.remaining_place[row_index] }
		end
	end
end
global_data.datasources.location = srclocation

local srccolor = DataSource.new("srccolor")
srccolor.GetNumRows = function (table_name)
	if table_name == 'placeholder' then return #global_data.colors + 1 end
	if table_name:sub(1, 5) == 'color' then
		local player_id = tonumber(table_name:sub(6, 6))
		if global_data.player[player_id].color ~= '-' then
			return #global_data.remaining_color + 2
		end
		return #global_data.remaining_color + 1
	end
end
srccolor.GetRow = function (table_name, row_index, columns)
	if table_name == 'placeholder' then
		if row_index == 0 then return { '-', '-' } end
		return { tostring(row_index), tostring(row_index) }
	elseif table_name:sub(1, 5) == 'color' then
		if row_index == 0 then return { '-', '-' } end
		local player_id = tonumber(table_name:sub(6, 6))

		if global_data.player[player_id].color ~= '-' then
			if global_data.player[player_id].cache_color == nil then
				local cache = { }
				for i, p in pairs(global_data.remaining_color) do
					table.insert(cache, p)
				end
				table.insert(cache, global_data.player[player_id].color)
				table.sort(cache, function (x, y) return tonumber(x) < tonumber(y) end)
				global_data.player[player_id].cache_color = cache
			end

			return { global_data.player[player_id].cache_color[row_index], global_data.player[player_id].cache_color[row_index] }
		else
			return { global_data.remaining_color[row_index], global_data.remaining_color[row_index] }
		end
	end
end
global_data.datasources.color = srccolor

function setenable_aiplayer(id, enabled)
	id = tonumber(id)
	if global_data.player[id].enabled ~= enabled then
		global_data.player[id].enabled = enabled
		local selector = document:GetElementById('player-' .. id)

		local disable_input = function (element)
			if element.class_name ~= 'select-name' then
				Element.As.ElementFormControlInput(element).disabled = not enabled
			end
		end
		for i, v in pairs(selector:GetElementsByTagName('select')) do
			disable_input(v) end
		for i, v in pairs(selector:GetElementsByTagName('dataselect')) do
			disable_input(v) end
		if not enabled then
			for i, v in pairs({ 'select-name', 'select-color', 'select-place' }) do
				Element.As.ElementFormControlSelect(selector:GetElementsByClassName(v)[0]).selection = 0 end
		end

		local enabled_checkbox = Element.As.ElementFormControlInput(selector:GetElementsByClassName('checkbox')[0])
		if enabled_checkbox.checked ~= enabled then
			enabled_checkbox.checked = enabled end
	end
end

function select_place(event)
	local id = tonumber(event.current_element.parent_node.parent_node.parent_node.id:sub(8, 8))
	if global_data.player[id].place ~= event.parameters.value then
		global_data.player[id].place = event.parameters.value
		check_remaining_place()

		for i = 1, 8 do
			global_data.datasources.location:NotifyRowChange('place' .. i) end
	end
end

function select_switch(event)
	local id = tonumber(event.current_element.parent_node.parent_node.id:sub(8, 8))
	local enabled = (event.parameters.value == 'enabled')
	setenable_aiplayer(id, enabled)
end

function select_name(event)
	local id = tonumber(event.current_element.parent_node.parent_node.parent_node.id:sub(8, 8))
	setenable_aiplayer(id, event.parameters.value ~= 'none')
	global_data.player[id].name = event.parameters.value
end

function select_side(event)
	local id = tonumber(event.current_element.parent_node.parent_node.parent_node.id:sub(8, 8))
	global_data.player[id].side = event.parameters.value
end

function select_ally(event)
	local id = tonumber(event.current_element.parent_node.parent_node.parent_node.id:sub(8, 8))
	global_data.player[id].ally = event.parameters.value
end

function select_color(event)
	local id = tonumber(event.current_element.parent_node.parent_node.parent_node.id:sub(8, 8))
	if global_data.player[id].color ~= event.parameters.value then
		global_data.player[id].color = event.parameters.value
		check_remaining_color()
		for i = 1, 8 do
			global_data.datasources.color:NotifyRowChange('color' .. i) end
		update_colors_style()
	end
end

function update_rangevalue(range_element)
	local value = Element.As.ElementFormControlInput(range_element).value
	local data_element = range_element.parent_node:GetElementsByClassName('range-data')[0]
	data_element.inner_rml = tostring(math.floor(value))
end

function select_speed(event)
	update_rangevalue(event.current_element)
	global_data.battle_settings.game_speed = math.floor(tonumber(Element.As.ElementFormControlInput(event.current_element).value))
end

function select_credits(event)
	update_rangevalue(event.current_element)
	global_data.battle_settings.credits = math.floor(tonumber(Element.As.ElementFormControlInput(event.current_element).value))
end

function select_unitcount(event)
	update_rangevalue(event.current_element)
	global_data.battle_settings.unit_count = math.floor(tonumber(Element.As.ElementFormControlInput(event.current_element).value))
end

function select_shortgame(event)
	local enabled = (event.parameters.value == 'enabled')
	global_data.battle_settings.short_game = enabled
end

function select_mcvredeploy(event)
	local enabled = (event.parameters.value == 'enabled')
	global_data.battle_settings.mcv_redeploy = enabled
end

function select_crates(event)
	local enabled = (event.parameters.value == 'enabled')
	global_data.battle_settings.crates = enabled
end

function select_superweapons(event)
	local enabled = (event.parameters.value == 'enabled')
	global_data.battle_settings.superweapons = enabled
end

function select_buildoffally(event)
	local enabled = (event.parameters.value == 'enabled')
	global_data.battle_settings.build_off_ally = enabled
end

function update_numplayers(num)
	global_data.num_players = num
	for i = 1, 8 do
		local selector = document:GetElementById('player-' .. i)
		Element.As.ElementFormControlInput(selector:GetElementsByClassName('checkbox')[0]).checked = true
	end
	for i = num+1, 8 do
		local selector = document:GetElementById('player-' .. i)
		Element.As.ElementFormControlInput(selector:GetElementsByClassName('checkbox')[0]).checked = false
	end
end

function update_colors_style()
	for i, v in pairs(document:GetElementsByClassName('select-color')) do

		local selectvalue = v.child_nodes[2]
		if selectvalue.inner_rml then
			for id, color in pairs(global_data.colors) do
				v:SetClass('color-selected-' .. id, false)
			end
			v:SetClass('color-selected-' .. selectvalue.inner_rml, true)
		end

		local selection = Element.As.ElementFormControlSelect(v)
		for i, opt in pairs(selection.options) do
			opt['element'].style.background = global_data.colors[tonumber(opt['value'])]
			opt['element'].style.color = global_data.colors[tonumber(opt['value'])]
		end
	end
end

function check_remaining_place()
	local r = { }
	for i = 1, global_data.num_players+1 do
		r[i] = true
	end
	for i = 1, 8 do
		global_data.player[i].cache_place = nil end
	for i = 1, global_data.num_players do
		if global_data.player[i].place ~= '-' then
			r[tonumber(global_data.player[i].place)] = false
		end
	end

	local ret = { }
	for i = 1, global_data.num_players+1 do
		if r[i] then
			table.insert(ret, tostring(i))
		end
	end

	global_data.remaining_place = ret
end

function check_remaining_color()
	local r = { }

	for i = 1, #global_data.colors do
		r[i] = true end
	for i = 1, 8 do
		global_data.player[i].cache_color = nil
		if global_data.player[i].color ~= '-' then
			r[tonumber(global_data.player[i].color)] = false
		end
	end

	local ret = { }
	for i = 1, #global_data.colors do
		if r[i] then
			table.insert(ret, tostring(i))
		end
	end

	global_data.remaining_color = ret
end

check_remaining_place()
check_remaining_color()

update_numplayers(6)

function page_load(document)

	for i = 1, 8 do
		local selector = document:GetElementById('player-' .. i)
		local selector_place = Element.As.ElementFormControlDataSelect(
			selector:GetElementsByClassName('select-place')[0])
		local selector_color = Element.As.ElementFormControlDataSelect(
			selector:GetElementsByClassName('select-color')[0])

		selector_place:SetDataSource('srclocation.place' .. i)
		selector_place.selection = 0
		selector_color:SetDataSource('srccolor.color' .. i)
		selector_color.selection = 0
	end
	
	for i, name in pairs({ 'speed', 'credits', 'unitcount' }) do
		update_rangevalue(document:GetElementsByClassName('select-' .. name)[0]) end
	update_colors_style()
end

page_load(document)

-- fetch or generate random side ID with a player ID
function sideid_for_player(player_id)
	local sideid = tonumber(global_data.player[tonumber(player_id)].side)
	if sideid == nil then
		sideid = math.random(global_data.num_sides) end
	return sideid
end

-- maybe we need some 'premature optimization' :D
-- for a specific player each call, so update to
-- 'global_data.remaining_place/remaining_color' is needed
-- before we return
function finish_place_n_color_for_player(player_id)
	player_id = tonumber(player_id)

	-- this exists for some 'ambiguity' in the meaning of 'global_data.num_players'
	-- it's the actual count of players we have, or the number of slots the map has?
	local locations_has = global_data.num_players
	local locations_taken = FacerUtil.array_to_hash(global_data.remaining_place)
	local colors_taken = FacerUtil.array_to_hash(global_data.remaining_color)

	if global_data.player[player_id].place == '-' then
		while true do
			local candidate = tostring(math.random(locations_has))
			if locations_taken[candidate] ~= nil then
				global_data.player[player_id].place = candidate
				break
			end
		end
	end

	if global_data.player[player_id].color == '-' then
		while true do
			local candidate = tostring(math.random(#global_data.colors))
			if colors_taken[candidate] ~= nil then
				global_data.player[player_id].color = candidate
				break
			end
		end
	end

	check_remaining_place()
	check_remaining_color()
end

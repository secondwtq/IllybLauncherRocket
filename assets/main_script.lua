
function load_font()

	-- rocket:LoadFontFace('Delicious-Roman.otf')
	-- rocket:LoadFontFace('Delicious-Italic.otf')
	-- rocket:LoadFontFace('Delicious-Bold.otf')
	-- rocket:LoadFontFace('Delicious-BoldItalic.otf')

	rocket:LoadFontFace('SourceHanSansCN-Regular.otf')
	-- rocket:LoadFontFace('SourceHanSansCN-Normal.otf')
	-- rocket:LoadFontFace('SourceHanSansCN-Medium.otf')

end

load_font()
maincontext = rocket.contexts["main"]
document = maincontext:LoadDocument("main.rml")
document:Show()

global_data = {
	num_aiplayers = 6,
	colors = {
		'rgb(255, 0, 0)',
		'rgb(0, 255, 0)',
		'rgb(0, 0, 255)',
	},
	remaining_place = { 1, 2, 3, 4, 5, 6, 7 },
	remaining_color = { 1, 2, 3 },

	player = {

	}
}

for i = 1, 7 do
	global_data.player[i] = {
		enabled = false,
		name = 'easy',
		side = 'Random',
		color = '-',
		place = '-',
		ally = '-'
	}
end

datasources = { }

local srclocation = DataSource.new("srclocation")
srclocation.GetNumRows = function (table_name)
	if table_name == 'placeholder' then return global_data.num_aiplayers + 1 end
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
datasources.location = srclocation

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
datasources.color = srccolor

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
			Element.As.ElementFormControlSelect(selector:GetElementsByClassName('select-name')[0]).selection = 0
		end

		local enabled_checkbox = Element.As.ElementFormControlInput(selector:GetElementsByClassName('checkbox')[0])
		if enabled_checkbox.checked ~= enabled then
			enabled_checkbox.checked = enabled
		end
	end
end

function select_place(event)
	local id = tonumber(event.current_element.parent_node.parent_node.parent_node.id:sub(8, 8))
	if global_data.player[id].place ~= event.parameters.value then
		global_data.player[id].place = event.parameters.value
		check_remaining_place()

		for i = 1, 7 do
			datasources.location:NotifyRowChange('place' .. i)
		end
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
		for i = 1, 7 do
			datasources.color:NotifyRowChange('color' .. i)
		end
		update_colors_style()
	end
end

function update_numaiplayers(num)
	global_data.num_aiplayers = num
	for i = 1, 7 do
		local selector = document:GetElementById('player-' .. i)
		Element.As.ElementFormControlInput(selector:GetElementsByClassName('checkbox')[0]).checked = true
	end
	for i = num+1, 7 do
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
	for i = 1, global_data.num_aiplayers+1 do
		r[i] = true
	end
	for i = 1, 7 do
		global_data.player[i].cache_place = nil end
	for i = 1, global_data.num_aiplayers do
		if global_data.player[i].place ~= '-' then
			r[tonumber(global_data.player[i].place)] = false
		end
	end

	local ret = { }
	for i = 1, global_data.num_aiplayers+1 do
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
	for i = 1, 7 do
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

for i = 1, 7 do
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

update_numaiplayers(6)
update_colors_style()

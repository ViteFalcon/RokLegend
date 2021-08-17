extends WindowDialog

const CharacterServer = preload("res://data/scripts/CharacterServer.gd")

export(Array, Resource) var servers = []

func _ready():
	pass

func show_modal(exclusive:bool=true):
	var viewport_size = get_viewport_rect().size
	var viewport_width = viewport_size.x
	var viewport_height = viewport_size.y
	rect_position = Vector2((viewport_width - rect_size.x)/2, (viewport_height - rect_size.y)/2)
	if len(servers) == 0:
		return
	for item in servers:
		$ServerList.add_item("{name} ({players})".format({
			"name": item.name,
			"players": item.player_count,
		}))
	$ServerList.select(0)
	.show_modal(exclusive)

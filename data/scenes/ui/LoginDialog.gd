extends WindowDialog

const TWO_THIRDS = 2.0/3.0


# Called when the node enters the scene tree for the first time.
func _ready():
	self._on_viewport_size_changed()
	get_viewport().connect("size_changed", self, "_on_viewport_size_changed")


func _on_viewport_size_changed():
	var viewport_size = get_viewport_rect().size
	var height = viewport_size.y
	var dialog_top = height * TWO_THIRDS
	rect_position.x = (viewport_size.x - rect_size.x) / 2
	rect_position.y = dialog_top

func _on_LoginDialog_about_to_show():
	pass

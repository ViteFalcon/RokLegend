extends Node2D

const TWO_THIRDS = 2.0/3.0
# Declare member variables here. Examples:
# var a = 2
# var b = "text"


# Called when the node enters the scene tree for the first time.
func _ready():
	$LoginDialog.show_modal(true)
	self._on_viewport_size_changed()
	get_viewport().connect("size_changed", self, "_on_viewport_size_changed")


func _on_viewport_size_changed():
	var viewport_size = get_viewport_rect().size
	var height = viewport_size.y
	var dialog_top = height * TWO_THIRDS
	$LoginDialog.rect_position.x = (viewport_size.x - $LoginDialog.rect_size.x) / 2
	$LoginDialog.rect_position.y = dialog_top


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

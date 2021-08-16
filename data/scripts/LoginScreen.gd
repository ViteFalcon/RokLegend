extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	OS.set_window_maximized(true)
	$LoginDialog.show_modal(true)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

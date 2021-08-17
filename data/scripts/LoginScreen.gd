extends Node2D


# Called when the node enters the scene tree for the first time.
func _ready():
	OS.set_window_maximized(true)
	var result = $LoginDialog.connect("login_successful", self, "_on_login_successful")
	if result != OK:
		print("Failed to connect login successful signal")
	$LoginDialog.show_modal(true)


# Called every frame. 'delta' is the elapsed time since the previous frame.
#func _process(delta):
#	pass

func _on_login_successful(servers):
	$LoginDialog.hide()
	print("Server count: ", len(servers))
	$CharacterServers.servers = servers
	$CharacterServers.show_modal(true)

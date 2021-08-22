extends WindowDialog

const CharacterServer = preload("res://data/scripts/CharacterServer.gd")
const TWO_THIRDS = 2.0/3.0

export(String) var Username = ""
export(bool) var SaveUsername = false

signal login_successful(servers)


# Called when the node enters the scene tree for the first time.
func _ready():
	self._on_viewport_size_changed()
	var result = get_viewport().connect("size_changed", self, "_on_viewport_size_changed")
	if result != OK:
		print("Failed to connect to viewport size change")
	result = $CancelButton.connect("button_down", self, "_on_LoginDialog_modal_closed")
	if result != OK:
		print("Failed to connect cancel button down event")
	result = get_close_button().connect("button_down", self, "_on_LoginDialog_modal_closed")
	if result != OK:
		print("Failed to connect close button down event")
	result = $ConfirmationDialog.get_cancel().connect("button_down", self, "_on_ConfirmationDialog_cancelled")
	if result != OK:
		print("Failed to connect confirmation cancel button down event")
	_on_LoginDialog_tree_entered()


func _on_viewport_size_changed():
	var viewport_size = get_viewport_rect().size
	var height = viewport_size.y
	var dialog_top = height * TWO_THIRDS
	rect_position.x = (viewport_size.x - rect_size.x) / 2
	rect_position.y = dialog_top


func _on_LoginDialog_modal_closed():
	$ConfirmationDialog.popup_centered()
	$ConfirmationDialog.get_close_button().hide()


func _on_ConfirmationDialog_confirmed():
	get_tree().quit(0)


func _on_ConfirmationDialog_cancelled():
	show_modal(true)
	self._on_viewport_size_changed()


func _on_LoginButton_pressed():
	var servers = []
	var server1 = CharacterServer.new()
	server1.name = "The Server"
	server1.player_count = 11
	server1.ip_address = "127.0.0.1"
	server1.ip_address = 3282
	servers.append(server1)
	emit_signal("login_successful", servers)


func _on_LoginDialog_about_to_show():
	$Username.text = Username
	$CheckButton.pressed = SaveUsername
	_on_LoginDialog_tree_entered()


func _on_LoginDialog_tree_entered():
	if Username == "":
		$Username.grab_focus()
	else:
		$Password.grab_focus()
		$Password.grab_click_focus()

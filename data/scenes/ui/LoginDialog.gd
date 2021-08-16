extends WindowDialog

const TWO_THIRDS = 2.0/3.0


# Called when the node enters the scene tree for the first time.
func _ready():
	self._on_viewport_size_changed()
	var result = get_viewport().connect("size_changed", self, "_on_viewport_size_changed")
	if result != OK:
		print("Failed to connect to viewport size change")
	$CancelButton.connect("button_down", self, "_on_LoginDialog_modal_closed")
	get_close_button().connect("button_down", self, "_on_LoginDialog_modal_closed")
	$ConfirmationDialog.get_cancel().connect("button_down", self, "_on_ConfirmationDialog_cancelled")


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

extends TextureRect

export(Array, Texture) var textures

# Called when the node enters the scene tree for the first time.
func _ready():
	if self.texture == null:
		self.texture = textures[rand_range(0, len(textures))]
	else:
		print("SKIPPING to set texture because one is aleady defined")
	$LoadingImage.texture = self.texture
	self.on_viewport_size_changed()
	get_viewport().connect("size_changed", self, "on_viewport_size_changed")


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta):
	pass

func on_viewport_size_changed():
	self.rect_size = get_viewport_rect().size
	$BlurRect.rect_size = self.rect_size
	$LoadingImage.rect_size = Vector2($LoadingImage.texture.get_width(), self.rect_size.y)
	var padding = self.rect_size.x - $LoadingImage.rect_size.x
	$LoadingImage.rect_position.x = padding/2

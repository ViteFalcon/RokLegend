extends Node2D

export(Array, Texture) var textures

# Called when the node enters the scene tree for the first time.
func _ready():
	randomize()
	if $BlurredImage.texture == null:
		$BlurredImage.texture = textures[rand_range(0, len(textures))]
	else:
		print("SKIPPING to set texture because one is aleady defined")
	$LoadingImage.texture = $BlurredImage.texture
	self._on_viewport_size_changed()
	var result = get_viewport().connect("size_changed", self, "_on_viewport_size_changed")
	if result != OK:
		print("Failed to connect to viewport size change")


func _on_viewport_size_changed():
	$BlurredImage.rect_size = get_viewport_rect().size
	var texture = $BlurredImage.texture
	var aspect_ratio = float(texture.get_width()) / float(texture.get_height())
	var image_height = $BlurredImage.rect_size.y
	var image_width = image_height * aspect_ratio
	$BlurRect.rect_size = $BlurredImage.rect_size
	$LoadingImage.rect_size = Vector2(image_width, image_height)
	var padding = $BlurredImage.rect_size.x - image_width
	$LoadingImage.rect_position.x = padding/2

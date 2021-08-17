extends Resource

class_name CharacterServer

var name = "" setget set_name, get_name
var player_count = 0 setget set_player_count, get_player_count
var ip_address = "" setget set_ip_address, get_ip_address
var port = 0 setget set_port, get_port

func set_name(value):
	name = value

func get_name():
	return name

func set_player_count(value):
	#assert(value >= 0)
	player_count = value

func get_player_count():
	return player_count

func set_ip_address(value):
	#assert(value != "")
	ip_address = value

func get_ip_address():
	return ip_address

func set_port(value):
	#assert(value >= 1024)
	port = value

func get_port():
	return port

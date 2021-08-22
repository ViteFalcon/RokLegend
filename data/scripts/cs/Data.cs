using Godot;
using RokLegend.data.scripts.cs;
using System;

public class Data : Node
{
	// Declare member variables here. Examples:
	// private int a = 2;
	// private string b = "text";
	[Export]
	public LoginData LoginData { get; private set; }

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		LoginData = new LoginData();
		Load();
	}

	public void Load()
	{
		LoginData.Username = "test";
		LoginData.LastSelectedCharacterIndex = 0;
		LoginData.SaveUsername = true;
		GD.Print("Loaded data!");
	}

	public void Save()
	{
		//
	}

//  // Called every frame. 'delta' is the elapsed time since the previous frame.
//  public override void _Process(float delta)
//  {
//      
//  }
}

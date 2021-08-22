using Godot;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RokLegend.data.scripts.cs
{
    public class LoginData : Resource
    {
        [Export]
        public string Username { get; set; }

        [Export]
        public bool SaveUsername { get; set; }

        [Export]
        public uint CharacterServerIndex { get; set; }

        [Export]
        public uint LastSelectedCharacterIndex { get; set; }
    }
}

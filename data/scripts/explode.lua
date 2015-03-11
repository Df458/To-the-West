if this.passable ~= true or this.occupied == true then
    game.effect("radius", this.x, this.y, 1, true, "X", 8, "", "dragonfire.lua");
    this.passable = true;
    this.color = 15;
    this.char = "~"
else
    this.color = 15;
end

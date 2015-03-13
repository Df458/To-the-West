if this.passable ~= true or this.occupied == true then
    game.message("Where the potion landed, the ground melts away into a pond.");
    game.effect("radius", this.x, this.y, 5, false, "X", 8, "lakeform.lua", "");
    this.passable = true;
    this.color = 6;
    this.char = "~"
end

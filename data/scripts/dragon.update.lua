if target ~= nil and math.abs(target.x - this.x) < 15 and math.random(25) == 1 then
    if this.targeting_player then
        game.message("The " .. this.name .. " spits out a burst of flame!", 4);
    end
    game.effect("line", this.x, this.y, target.x, target.y, false, true, "X", 8, "explode.lua", "");
    this.time = this.time - 25;
end

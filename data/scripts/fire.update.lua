if target ~= nil and math.abs(target.x - this.x) < 25 and math.random(3) == 1 then
    game.message("The " .. this.name .. " fires a searing beam!", 4);
    game.effect("line", this.x, this.y, target.x, target.y, true, true, "*", 8, "", "fireball.lua");
    this.time = this.time - 3;
end


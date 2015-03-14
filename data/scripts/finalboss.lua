if this.time <= 0 then
    game.message("The warlock's age seems to catch up to him. He stops a moment to catch his breath.");
    this.time = 0;
else
    if target ~= nil and math.abs(target.x - this.x) < 25 and math.random(20) == 1 then
        game.message("The " .. this.name .. " fires a searing beam!", 4);
        game.effect("line", this.x, this.y, target.x, target.y, true, true, "*", 8, "", "fireball.lua");
        this.time = this.time - 15;
    elseif target ~= nil and math.abs(target.x - this.x) < 25 and math.random(40) == 1 then
        game.message("The " .. this.name .. " holds his staff aloft, and begins to chant as the Band of the Stars glows!", 4);
        if math.random(5) == 1 then
            game.message("You feel very weak.", 8);
            target.str = target.str - 7;
            target.def = target.def - 7;
        else
            game.message("Mochus doubles over into a coughing fit, and curses loudly. The spell fizzles!", 2)
        end
        this.time = -1;
    elseif target ~= nil and math.abs(target.x - this.x) < 25 and math.random(30) == 1 then
        game.message("The " .. this.name .. " summons a powerful sandstorm!", 4);
        game.effect("radius", this.x, this.y, 8, true, ":", 5, "", "sandstorm.lua");
        this.time = this.time - 5;
    elseif target ~= nil and math.abs(target.x - this.x) < 25 and math.random(60) == 1 then
        game.message("The " .. this.name .. " summons a wave of lava!", 4);
        game.effect("radius", this.x, this.y, 5, true, ":", 6, "", "lavaburn.lua");
        this.time = -1;
    elseif target ~= nil and math.abs(target.x - this.x) < 25 and math.random(50) == 1 then
        game.message("The " .. this.name .. " summons a dragon!", 4);
        game.spawn(this.x + math.random(6) - 3, this.y + math.random(4) - 2, "dragon");
        this.time = -1;
    end
end

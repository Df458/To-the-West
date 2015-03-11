if math.random(40) == 1 then
    game.message("A powerful simoom sweeps across the land, buffeting all that stands in its' way with torrents of sand!", 4);
    game.effect("radius", this.x, this.y, 45, true, ":", 5, "", "sandstorm.lua");
end

if this.controlled == true and math.random(3) == 1 then
    this.hp = this.hp - 1;
    if this.hp / this.hp_max >= .3 and this.hp / this.hp_max < .6 then
        if math.random(20) == 1 then
            game.message("This horrid sunlight saps your very will to survive!");
        elseif math.random(19) == 1 then
            game.message("Your parched throat aches, as though it were begging for water.");
        elseif math.random(18) == 1 then
            game.message("You begin to wish you were back at home, relaxing with a cool drink.");
        end
    elseif this.hp / this.hp_max < .3 then
        if math.random(7) == 1 then
            game.message("It's impossible to concentrate under the torrid heat of the sun.");
        elseif math.random(6) == 1 then
            game.message("The bleached skulls of those foolish enough to enter seem to laugh at your misfortune.");
        elseif math.random(6) == 1 then
            game.message("You doubt that you can stand this heat much longer!");
        elseif math.random(5) == 1 then
            game.message("Water...");
        elseif math.random(5) == 1 then
            game.message("You rejoice as you see a beautiful oasis. Then you blink, and it's gone.");
        end
    end
end

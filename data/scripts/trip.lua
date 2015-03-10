if this.controlled == true then
    if math.random(40) == 1 then
        game.message("You trip on a rock, but catch yourself.");
    elseif math.random(100) == 1 and this.hp > 1 then
        game.message("You trip on a rock, and land flat on your face!", 11);
        this.hp = this.hp - 1;
    end
end

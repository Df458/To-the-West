damage = math.random(5, 17);
this.hp = this.hp - damage;
if this.controlled == true then
    game.message("Powerful desert winds slam into you for " .. damage .. " damage!", 4);
end

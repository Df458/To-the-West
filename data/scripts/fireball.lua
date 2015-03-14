damage = math.random(2, 10);
if this.controlled == true then
    game.message("The fire burns you for " .. damage .. " damage!", 4)
end
this.hp = this.hp - damage;

damage = math.random(8, 10);
this.hp = this.hp - damage;
game.message("The dart strikes the " .. this.name .. " for " .. damage .. " damage!");

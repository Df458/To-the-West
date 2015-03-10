continue = true;
if this.controlled == true then
    continue = game.ask("Are you sure you want to step in lava?", 8);
end

if continue then
    this.hp = this.hp - 50;
    if this.controlled == true then
        game.message("The searing heat burns you alive for 50 damage!", 8)
    end
end

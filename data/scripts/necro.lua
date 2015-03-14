--Make skeletons here
if target ~= nil and math.abs(target.x - this.x) < 35 and math.random(10) == 1 then
    game.spawn(this.x + math.random(6) - 3, this.y + math.random(4) - 2, "skeleton");
end

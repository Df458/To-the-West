if this.targeting_player == true then
    if math.random(35) == 1 then
        game.message("The brigand makes an unsavory comment about your mother.");
    elseif math.random(4) == 1 and this.hp / this.hp_max < 0.5 then
        game.message("The brigand pulls out a small silver flask, and takes a swig.");
        game.message("The brigand looks much healthier!", 11);
        this.hp = this.hp + (this.hp_max / 4);
        this.time = this.time - 3;
        if math.random(6) == 1 then
            game.message("Brigand: \"That hit the spot!\"");
        end
        if math.random(15) == 1 then
            game.message("The brigand belches loudly, and grins as he returns to his combat stance.");
            game.message("The brigand looks fiercer!", 11);
            this.str = this.str + 5;
            this.def = this.def + 5;
            this.time = this.time - 2;
        elseif math.random(6) == 1 then
            game.message("The brigand belches loudly.");
        end
    end
end

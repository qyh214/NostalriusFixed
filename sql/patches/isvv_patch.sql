UPDATE command SET security=2 WHERE name LIKE "cast%";
UPDATE command SET security=2 WHERE name LIKE "learn%";
UPDATE command SET security=2 WHERE name LIKE "additem%";
UPDATE command SET security=2 WHERE name LIKE "levelup%";
UPDATE command SET security=2 WHERE name LIKE "modify%";
UPDATE command SET security=2 WHERE name LIKE "quest%";
UPDATE command SET security=5 WHERE name LIKE "mmap%";
UPDATE command SET security=2 WHERE security >= 2 AND name IN ("aura","bank","cast","cast back","cast dist","cast self","cast target","character reputation","combatstop","cooldown","damage","debug anim","debug play cinematic","debug play sound","demorph","dismount","distance","explorecheat","gm","gm chat","gm ingame","go creature","go graveyard","go grid","go","go taxinode","go trigger","go xy","go xyz","go zonexy","gobject near","gobject target","goname","gps","groupgo","guid","hover","instance unbind","instance listbinds","instance stats","instance savedata","list creature","list item","list object","lookup area","lookup creature","lookup event","lookup faction","lookup item","lookup itemset","lookup object","lookup quest","lookup skill","lookup spell","lookup taxinode","lookup tele","namego","neargrave","notify","npc info","recall","reset talents","server info","setskill","showarea","taxicheat","tele","tele group","tele name","unaura","unlearn","unmute","waterwalk","whispers","stable","go object","list talents","gm setview","god","gm fly","gm options","list auras");
UPDATE command SET security=2 WHERE name IN ("npc aiinfo", "npc info", "guid", "respawn", "god", "die");
UPDATE command SET security=6 WHERE name LIKE "ban%";
UPDATE command SET security=6 WHERE name LIKE "unban%";
UPDATE command SET security=6 WHERE name LIKE "baninf%";
UPDATE command SET security=6 WHERE name LIKE "debug%";
UPDATE command SET security=6 WHERE name LIKE "account%";
UPDATE command SET security=6 WHERE name LIKE "anticheat%";
UPDATE command SET security=6 WHERE name LIKE "reload%";
UPDATE command SET security=6 WHERE name LIKE "pinfo";
UPDATE command SET security=5 WHERE name LIKE "kick";
UPDATE command SET security=2 WHERE name LIKE "debug bg";
UPDATE command SET security=2 WHERE name LIKE "bg %";
UPDATE command SET security=2 WHERE name LIKE "gm visible";
UPDATE command SET security=2 WHERE name LIKE "event%";
UPDATE command SET security=4 WHERE name LIKE "account set gmlevel";
UPDATE command SET security=2 WHERE name LIKE "npc summon";
UPDATE command SET security=2 WHERE name = "debug loottable";
UPDATE command SET security=4 WHERE name IN ("debug setvalue", "debug getvalue", "server restart", "npc add", "npc additem", "npc delitem", "replay", "debug time");
UPDATE command SET security=2 WHERE name IN ("npc changelevel");
UPDATE command SET security=4 WHERE name = "server restart";
UPDATE command SET security=4 WHERE name LIKE "reload%";
UPDATE command SET security=5 WHERE name LIKE "%warden%" OR name LIKE "%cheat%";
UPDATE command SET security=4 WHERE name = "gobject add";

# TODO: Clean anticheat tables here, so people don't experiment with anticheat on the PTR!

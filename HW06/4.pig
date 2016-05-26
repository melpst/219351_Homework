edges = LOAD '/user/webapp4/input' AS (x:int, y:int);
grouped = GROUP edges BY x;
nodecount = FOREACH grouped GENERATE group, COUNT(edges);
store nodecount into 'edgeCount_pig_new';

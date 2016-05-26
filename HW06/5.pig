A = LOAD '/user/webapp4/input' AS (a:int, b:int);

B = FOREACH A GENERATE a as b , b as c;
C = JOIN A BY b, B by b;

E = FOREACH C GENERATE $0,$3;


X = UNION A,E;

G = group X by $0;

ANS = FOREACH G {

	dX = DISTINCT X.$1;
	GENERATE group,dX;
}


--DUMP ANS;


store ANS into 'FOFoutput_pig';


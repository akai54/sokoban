% Game representation
% '#' wall
% 'p' player
% '$' box
% '*' box moved in goal position
% 'g' goal position
% 'o' empty spot


% converts string list to atoms
convertList([], []).
convertList([H|Q], [NL|T]):- convertList(Q, T), string_chars(H, NL).

% unifies Y with all elements from Ss
remove([X|_], X).
remove([_|Ss], Y):- remove(Ss, Y).

% helper for finding goal x -> @ -> o
replaceCells([], []).
replaceCells([#|TB], [#|TA]):- !, replaceCells(TB, TA).
replaceCells([p|TB], [p|TA]):- !, replaceCells(TB, TA).
replaceCells([g|TB], [*|TA]):- !, replaceCells(TB, TA).
replaceCells([$|TB], [o|TA]):- !, replaceCells(TB, TA).
replaceCells([o|TB], [o|TA]):- !, replaceCells(TB, TA).

% takes list of lists and from each list erases head
% RL is the cut part
extractFirstCol([], [], []).
extractFirstCol([[H|HH]|T], [HH|TC], [H|Rem]):- extractFirstCol(T, TC, Rem).

% add Column to the beginning of LL and return it in R
addFirstCol([], [], []).
addFirstCol([Row|T], [HH|Column], [NextRow|TT]):-
	NextRow=[HH|Row], addFirstCol(T, Column, TT).

%returns the smaller of the two numbers
min(X, Y, X):- X < Y.
min(X, Y, Y):- Y =< X.

getPos(C, S, L):- getPos(C, S, L, 0).

%getPos - impl, Y is Y-coord, this cuts by lines
getPos(_, [], [], _).
getPos(C, [LS|Ss], L, Y):- getPos(C, LS, L1, 0, Y), Y2 is Y+1,
						   getPos(C, Ss, L2, Y2),
						   append(L1, L2, L).
getPos(_, [], [], _, _).
getPos(C, [C|LS], [[X,Y]|L], X, Y):- X2 is X+1, getPos(C, LS, L, X2, Y).
getPos(C, [D|LS], L, X, Y):- D\=C, X2 is X+1,  getPos(C, LS, L, X2, Y).


manhattan_distance(_, _, [], 999999).
manhattan_distance(X, Y, [[X2,Y2]|S], FinalDist):- manhattan_distance(X, Y, S, Dist),
				Ans is abs(X-X2) + abs(Y-Y2),
				min(Dist, Ans, FinalDist).

heurHelper(S,R):- getPos($, S, Boxes), 
				  getPos(g, S, Spots), 
				  heurHelper(Boxes, Spots, R).

heurHelper([], _, 0).
heurHelper([[X,Y]|B], S, R2):-
	manhattan_distance(X, Y, S, Dist),
	heurHelper(B, S, R),
	R2 is R + Dist.
% advanced heuristic function that takes into account the distances
% between all boxes and their respective goals
heuristic(S, R) :-
    getPos($, S, Boxes),
    getPos(g, S, Spots),
    findall(D, boxToGoalDistances(Boxes, Spots, D), Dists),
    flatten(Dists, AllDists),
    sumlist(AllDists, SumDists),
    heurHelper(Boxes, Spots, R1),
    R is R1 + SumDists.

% helper function to calculate the distances between each box and its goal
boxToGoalDistances([], _, []).
boxToGoalDistances([[X, Y] | Rest], Spots, Dists) :-
    maplist(manhattan_distance(X, Y), Spots, Distances),
    min_list(Distances, MinDist),
    boxToGoalDistances(Rest, Spots, RestDists),
    Dists = [MinDist | RestDists].


% finds final position from a given one
% by appropriate replacements
findGoal([], []).
findGoal([X|XS], [Y|YS]):- replaceCells(X,Y), findGoal(XS, YS).

% get all possible moves from F with the direction D (lrud)
allPossibleMove(F,T,D):- setof([M,D], move(F,M,D), MS), remove(MS, [T,D]).


% implementation of allPossibleMove
% it cuts columns and rows in all possible ways until we
% find astar and then try to move him.
% remove first column, run recursively and give it back
move(X, M, D):- X \= [], extractFirstCol(X,Y, Col),
				move(Y, Y2, D), addFirstCol(Y2, Col, M).

move([X|Y], [X|M], D):- X \= [], move(Y, M, D).

% right
move([[p,o|X]|Y], [[o,p|X]|Y], right).
move([[p,$,g|X]|Y], [[o,p,*|X]|Y], right).
move([[p,$,o|X]|Y], [[o,p,$|X]|Y], right).
% left
move([[o,p|X]|Y], [[p,o|X]|Y], left).
move([[g,$,p|X]|Y], [[*,p,o|X]|Y], left).
move([[o,$,p|X]|Y], [[$,p,o|X]|Y], left).


% up
move([[o|X],[p|Y]|R], [[p|X],[o|Y]|R], up).
move([[g|X],[$|Y],[p|Z]|R], [[*|X],[p|Y],[o|Z]|R], up).
move([[o|X],[$|Y],[p|Z]|R], [[$|X],[p|Y],[o|Z]|R], up).

% down
move([[p|X],[o|Y]|R], [[o|X],[p|Y]|R], down).
move([[p|X],[$|Y],[g|Z]|R], [[o|X],[p|Y],[*|Z]|R], down).
move([[p|X],[$|Y],[o|Z]|R], [[o|X],[p|Y],[$|Z]|R], down).


% A-STAR Algorithm:
astar(X, Sol):- convertList(X,Start), findGoal(Start, Goal),
	heuristic(Start, R),
	astarH(Start, Goal, R, Sol).

astarH(Start, Goal, Max, Moves):-
	successor(Start, Goal, 0, Max, Moves),!.
astarH(Start, Goal, Max, Moves):-
	Max < 40, Max2 is Max +1,
	astarH(Start, Goal, Max2, Moves).


successor(Goal, Goal, _, _, []):-!.
successor(Start, Goal, Depth, Max, [Dir|Moves]):-
	Depth < Max,
	allPossibleMove(Start, Move, Dir),
	heuristic(Start, R),
	Depth2 is Depth + 1,
	Depth2 + R =< Max,
	successor(Move, Goal, Depth2, Max, Moves).


% the only one which is working now...
puzzleI(["########",
         "#ooooop#",
         "#$$$$$$#",
         "#gggggg#",
		 "#oooooo#",
		 "########"]).

% take long time...
puzzleII(["oooo#####",
		  "oooo#ooo#",
	  	  "oooo#$oo#",
	  	  "oo###oo$##",
	  	  "oo#oo$o$o#",
	  	  "###o#o##o#ooo######",
	  	  "#ooo#o##o#####oogg#",
	  	  "#o$oo$ooooooooopgg#",
	  	  "#####o###o#o##oogg#",
	  	  "oooo#ooooo#########",
	  	  "oooo#######"]).

% return false because of complexity of the puzzles.
puzzleIII(["############",
			"#ggo#ooo###",
			"#ggo#o$o$o#",
			"#ggpo#$####o#",
			"#ggooo##o#",
			"#ggo#o#o$o##",
			"######o##$o$o#",
			"o#o$o$o$o$o#",
			"o#oo#ooo#",
			"o############"]).

puzzleIV(["oooooooo########",
			"oooooooo#opoooo#",
			"oooooooo#o$#$o##",
			"oooooooo#o$oo$#",
			"oooooooo##$o$o#",
			"#########o$o#o###",
			"#ggggoo##o$oo$oo#",
			"##gggoooo$oo$ooo#",
			"#ggggoo##########",
			"########"]).

puzzleV(["ooooooooooo########",
		 "ooooooooooo#oogggg#",
	 	 "############oogggg#",
	 	 "#oooo#oo$o$ooogggg#",
	 	 "#o$$$#$oo$o#oogggg#",
	 	 "#oo$ooooo$o#oogggg#",
	 	 "#o$$o#$o$o$########",
	 	 "#oo$o#ooooo#",
	 	 "##o#########",
	 	 "#oooo#ooooo#",
	 	 "#ooooo$ooop#",
	 	 "#oo$$#$$ooo#",
	 	 "#oooo#ooooo#",
	 	 "###########"]).

puzzleVI(["######o#####",
		  "#ggoo#o#poo#",
	  	  "#ggoo###ooo#",
	  	  "#ggooooo$$o#",
	  	  "#ggoo#o#o$o#",
	  	  "#gg###o#o$o#",
	  	  "####o$o#$oo#",
	  	  "ooo#oo$#o$o#",
	  	  "ooo#o$oo$oo#",
	  	  "ooo#oo##ooo#",
	  	  "ooo#########"]).


list_butlast([X|Xs], Ys) :-                 
list_butlast_prev(Xs, Ys, X).            

list_butlast_prev([], [], _).
list_butlast_prev([X1|Xs], [X0|Ys], X0) :-  
   list_butlast_prev(Xs, Ys, X1).
% a predicat to test the program.
solver :- 
	statistics(walltime, [Start,_]),
	puzzleI(Puzzle),
	astar(Puzzle, Sol), 
	write(Sol),
	statistics(walltime, [End,_]),
    Time is End - Start,
	nl,
    format('Execution time: ~3f seconds.~n', [Time/1000]).

% to use the program just execute the following command in a swipl interpreter: 
% ?- solver.

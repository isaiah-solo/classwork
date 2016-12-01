% Isaiah Solomon
% icsolomo

not(X) :- X, !, fail.
not(_).

degmin_to_degs(degmin(Degrees, Minutes), DegreesOnly) :-
   DegreesOnly is Degrees + Minutes / 60.

pythagoras(X1, Y1, X2, Y2, Hypotenuse) :-
   DeltaX is X1 - X2,
   DeltaY is Y1 - Y2,
   Hypotenuse is sqrt(DeltaX * DeltaX + DeltaY * DeltaY).

distance(Airport1, Airport2, Miles) :-
   airport(Airport1, _, LatDeg1, LonDeg1),
   airport(Airport2, _, LatDeg2, LonDeg2),
   degmin_to_degs(LatDeg1, Lat1),
   degmin_to_degs(LatDeg2, Lat2),
   degmin_to_degs(LonDeg1, Lon1),
   degmin_to_degs(LonDeg2, Lon2),
   pythagoras(Lat1, Lon1, Lat2, Lon2, Degrees),
   Miles is 69 * Degrees.

flight_time(Airport1, Airport2, Time) :-
   distance(Airport1, Airport2, Miles),
   Time is Miles / 500.

arrival_time(flight(Airport1, Airport2, time(DH,DM)), ArrivalTime) :-
   flight_time(Airport1, Airport2, Time),
   time_to_hours(time(DH,DM), DepartureTime),
   ArrivalTime is DepartureTime + Time.

mins_to_hours(Mins, Hours):-
   Hours is Mins / 60.

hours_to_mins(Mins, Hours) :-
   Mins is Hours * 60.

time_to_hours(time(Hours, Mins) , InHours) :-
   InHours is Hours + Mins / 60.

print_digits(Digits) :-
   Digits < 10, print(0), print(Digits).

print_digits(Digits) :-
   Digits >= 10, print(Digits).

print_time(InHours) :-
   InMins is floor(InHours * 60),
   Hours is InMins // 60,
   Mins is InMins mod 60,
   print_digits(Hours),
   print(':'),
   print_digits(Mins).

write_path([]).

write_path([flight(Depart,Arrive,DTimeHM)|List]) :-
   airport(Depart, Depart_name, _, _),
   airport(Arrive, Arrive_name, _, _),
   time_to_hours(DTimeHM, DepartTime),
   arrival_time(flight(Depart,Arrive,DTimeHM), ArrivalTime),
   write('depart  '), write(Depart), 
      write('  '), write(Depart_name), 
      write('  '), print_time(DepartTime),
   nl,
   write('arrive  '), write(Arrive), 
      write('  '), write(Arrive_name), 
      write('  '), print_time(ArrivalTime),
   nl,
   write_path(List). 

safe_time(H1, T2) :-
   time_to_hours(T2, H2),
   hours_to_mins(M1, H1),
   hours_to_mins(M2, H2),
   M1 + 29 < M2.

safe_arrival(flight(Dep, Arriv, DepTime)) :-
   arrival_time(flight(Dep, Arriv, DepTime), ArrivTime),
   ArrivTime < 24.

path(Node, End, [flight(Node, Next, NextDep)|Outlist]) :-
   not(Node = End), 
   flight(Node, Next, NextDep),
   path(Next, End, [flight(Node, Next, NextDep)], Outlist).

path(Node, Node, _, []).

path(Node, End,
   [flight(PreDep,PreArr,PreDepTime)|Tried], 
   [flight(Node, Next, NextDep)|List]) :-
   flight(Node, Next, NextDep),
   arrival_time(flight(PreDep,PreArr,PreDepTime), PreArriv), .
   safe_time(PreArriv, NextDep), 
   safe_arrival(flight(Node, Next, NextDep)),
   Tried2 = append([flight(PreDep, PreArr, PreDepTime)], Tried),
   not(member(Next, Tried2)), 
   not(Next = PreArr),
   path(Next, End, 
   [flight(Node, Next, NextDep)|Tried2], 
      List). 

travel([flight(Dep, Arr, DTimeHM)|List], Length) :-
   length(List, 0),
   time_to_hours(DTimeHM,DTimeH), 
   arrival_time(flight(Dep, Arr, DTimeHM), ArrivalTime),
   Length is ArrivalTime - DTimeH.

travel([flight(Dep, Arr, DTimeHM)|List], Length) :-
   length(List, L),
   L > 0,
   travel(flight(Dep, Arr, DTimeHM), List, Length).

travel(flight(_, _, DTimeHM), [Head|List], Length) :-
   length(List, 0),
   time_to_hours(DTimeHM, DTimeH),
   arrival_time(Head, ArrivalTime),
   Length is ArrivalTime - DTimeH.

travel(flight(Dep, Arr, DTimeHM), [_|List], Length) :-
   length(List, L),
   L > 0,
   travel(flight(Dep, Arr, DTimeHM), List, Length).

shortest(Depart, Arrive, List) :-
   path(Depart, Arrive, List),
   noshorter(Depart, Arrive, List).

noshorter(Depart, Arrive, List) :-
   path(Depart, Arrive, List2),
   travel(List, Length1),
   travel(List2, Length2),
   Length1 > Length2,
   !, fail.

noshorter(_, _, _).

fly(Depart, Arrive) :-
   shortest(Depart, Arrive, List),
   nl,
   write_path(List),!.

fly(Depart, Depart) :-
   write('Error: Departure and arrival airports are the same.'),
   !, fail.

fly(Depart, _) :-
   \+ airport(Depart, _, _, _),
   write('Departure airport was invalid.'),
   !, fail.

fly(_, Arrive) :-
   \+ airport(Arrive, _, _, _),
   write('Arrival airport was invalid.'),
   !, fail.

fly(Depart, Arrive) :- 
   \+shortest(Depart, Arrive, _),
   write('Error: Did not find a valid itinerary.'),
   !, fail.

program calnumero;
var x, i, j,y:integer;
var q, r , u:single;

var z, w:real;

begin
     writeln('Entre com dois numeros X e Y');
     readln(x,y,q);
	 x := (2);
	 i := 1;
	 j := 2;
	 while (i < 10) do
	 begin
		i := i + 1;			
	 end;
	 if j < 5 then
	 begin	 j:=3;
	 end
	 else
	 begin	 j:=5*(4+(3*4))-1;
	 end;
     writeln('X + Y =',x+y);
     writeln('X * Y^2=',x*y);
     writeln('X^2 =',x*x);
     writeln('Seno de (X - Y)=',x-y);
     writeln('Valor Absoluto de X=',x);
end.

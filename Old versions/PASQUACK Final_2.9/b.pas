program listadevalores;
uses crt;
var x1, x2, i:integer;
begin
     writeln('Entre com dois valores x1 e x2');
     readln(x1, x2);
     if x1<x2 then
        for i:=x1 to x2 do
           write(i,'funciona');
     writeln('Loool');
     if x1<x2 then
        for i:=x2 downto x1 do
           write(i,'eita');
     if x1=x2 then
        writeln('Numeros Iguais');
      readkey;
end.

program listadevalores;
uses crt;
var x1, x2, i:integer;
//var listadevalores:String;

var l,r:integer;


// LOLLLLLL/*

begin
     writeln('Entre com dois valores x1 e x2');
     readln(x1, x2);
     if x1<x2 then
        for i:=x1 to x2 do
           write(i,' ');
     writeln();
     if x1<x2 then
        for i:=x2 downto x1 do
           write(i,' ');
     if x1=x2 then
        writeln('Numeros Iguais');
      readkey;
end.

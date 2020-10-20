Horz = 8;
Vert = 8;
numHorz = 1;
numVert = 2;
ancho = 1;
alto = 6;
sizeHorz = 65.3;
sizeVert = 66.3;


for(i=[0:1:(Horz*numHorz)])
{
    translate([(sizeVert/Vert)*i,0,0])
    cube([ancho,(sizeVert*numVert)-ancho,alto]);
}
for(i=[0:1:(Vert*numVert)])
{
    translate([0,(sizeHorz/Horz)*i,0])
    cube([(sizeHorz*numHorz)+ancho,ancho,alto]);
}

for(i=[0:1:(Horz*numHorz)])
{
    translate([((Horz*numHorz)*(sizeVert/Vert)+10)+(sizeVert/Vert)*i,0,0])
    cube([ancho,(sizeVert*numVert)-2*ancho,alto]);
}
for(i=[0:1:(Vert*numVert)-1])
{
    translate([((Horz*numHorz)*(sizeVert/Vert)+10),(sizeHorz/Horz)*i,0])
    cube([(sizeHorz*numHorz)+ancho,ancho,alto]);
}

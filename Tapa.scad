thickness = 1.2;

$fn = 50;

difference()
{
    cylinder(d = 25 + 2*thickness, h = 15);
    cylinder(d = 25, h = 10);
    
    translate([0, 0, 10])
    cylinder(d = 5, h = 5);
}
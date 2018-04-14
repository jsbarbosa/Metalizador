union()
{
    translate([-25, -2.5 + 32.2/2, 1.5])
    cube([50, 5, 3], center = true);

    translate([-25, 2.5 -32.2/2, 1.5])
    cube([50, 5, 3], center = true);

    difference()
    {
        translate([0, 0, 95/2])
        cube([5, 38, 95], center = true);

        translate([0, 0, -11.8/2 + 95])
        cube([10, 23.2, 11.8], center = true);
        
        translate([0, 32.2/2 -1, + 95 - 11.8/2])
        rotate([0, 90, 0])
        cylinder(h = 10, d = 4, center = true);

        translate([0, -32.2/2 +1, + 95 - 11.8/2])
        rotate([0, 90, 0])
        cylinder(h = 10, d = 4, center = true);
    }

    translate([-10/2 -2, 32.2/2 - 2.5, 10/2 + 1])
    cube([10, 5, 10], center = true);

    translate([-10/2 -2, -32.2/2 + 2.5, 10/2 + 1])
    cube([10, 5, 10], center = true);
}
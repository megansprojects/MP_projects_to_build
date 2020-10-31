//                    _
// |\/| _  _  _  _  _|_) __  o _  __| _
// |  |(/_(_|(_|| |_>|  | (_)|(/_(_ |_>
//         _|               _|
//       www.megansprojects.com
// 
// Sun rise set light head

$fn = 128;
wall_thickness = 3;
head_inside_outer_diameter  = 88;
head_inside_inner_diameter  = 65;
head_depth = 10;

head_inside_outer_r = head_inside_outer_diameter / 2;
head_inside_inner_r = head_inside_inner_diameter / 2;

difference()
{
    union()
    {
        // back plate
        translate([0,0,-wall_thickness])
        difference()
        {
            cylinder ( wall_thickness, head_inside_outer_r + wall_thickness ,head_inside_outer_r + wall_thickness );
            cylinder ( wall_thickness, head_inside_inner_r - wall_thickness ,head_inside_inner_r - wall_thickness);
        }
        // outer side
        difference()
        {
            cylinder ( head_depth, head_inside_outer_r + wall_thickness ,head_inside_outer_r + wall_thickness );
            cylinder ( head_depth, head_inside_outer_r ,head_inside_outer_r );
        }
        // inner side    
        difference()
        {
            cylinder ( head_depth, head_inside_inner_r ,head_inside_inner_r  );
            cylinder ( head_depth, head_inside_inner_r - wall_thickness ,head_inside_inner_r - wall_thickness);
        }
        // Swan neck connector block
        translate([0,head_depth + head_inside_outer_r + wall_thickness, head_depth /2 - wall_thickness / 2 ])
            rotate ([90,0,0])
                cylinder ( head_depth , head_depth /2 , head_depth /2 );

    }
    // cutout for top clear plastic ring
    translate([0,0,head_depth - wall_thickness])
        difference()
        {
            cylinder ( wall_thickness, head_inside_outer_r + wall_thickness / 2 ,head_inside_outer_r + wall_thickness / 2 );
            cylinder ( wall_thickness, head_inside_inner_r - wall_thickness / 2 ,head_inside_inner_r - wall_thickness / 2 );
        }
}

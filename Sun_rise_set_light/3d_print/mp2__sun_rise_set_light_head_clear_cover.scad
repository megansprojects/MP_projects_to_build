//                    _
// |\/| _  _  _  _  _|_) __  o _  __| _
// |  |(/_(_|(_|| |_>|  | (_)|(/_(_ |_>
//         _|               _|
//       www.megansprojects.com
// 
// Sun rise set light head clear cover

$fn = 128;
wall_thickness = 3;
head_inside_outer_diameter  = 88;
head_inside_inner_diameter  = 65;
head_depth = 10;

head_inside_outer_r = head_inside_outer_diameter / 2;
head_inside_inner_r = head_inside_inner_diameter / 2;

// top clear plastic ring
difference()
{
    cylinder ( wall_thickness, head_inside_outer_r + wall_thickness / 2 ,head_inside_outer_r + wall_thickness / 2 );
    cylinder ( wall_thickness, head_inside_inner_r - wall_thickness / 2 ,head_inside_inner_r - wall_thickness / 2 );
}

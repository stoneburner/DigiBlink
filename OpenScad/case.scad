

rotate([180,0,0]) difference() {
	translate([0,0,15]) cube([30,30,30],center=true);
	translate([0,0,2.49]) cutout();
} 

module cutout() {
	intersection() {
		cylinder(r=5, h=5, center=true);
		//rotate([0,0,90]) cube([18.5,8.5,8.5],center=true);		
	}
	translate([0,0,-1.5]) cube([5.7,50,2],center=true);
}
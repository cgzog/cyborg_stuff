$fn = 50;               // 50 facets per circle

Font1    = "Liberation Sans";
FontSize = 4;

// all dimensions in mm

PanelW = 130;
PanelH = 40;
PanelT = 3;

PanelA = 30;            // tilt of panel with potentiometers on it

PotHoleD  = 25.4*.260;   // 1/4" + a little extra
PotOffset = 15;         // offset from bottom of pot panel

TextExt    = 1;         // text extrusion thickness
TextOffset = 30;        // offset from bottom of pot panel

union() {
    rotate([90, 0, 0]) {
        translate([0, -1 * (PanelH - PanelT), 0]) {
            cube([PanelW, PanelH, PanelT]);         // base - same size as panel
        }
    }

    cube([PanelW, PanelH / 2, PanelT]);                 // vertical part at below pots

    translate([0, PanelH / 2 - PanelT * sin(PanelA), 0]) {
        rotate([-PanelA, 0, 0]) {
            
            difference() {
 
                cube([PanelW, PanelH, PanelT]);             // angled panel
            
                // holes for potentiometers
            
                translate([20, PotOffset, 0]) {
                    cylinder(d = PotHoleD, h = PanelT);
                }
                translate([50, PotOffset, 0]) {
                    cylinder(d = PotHoleD, h = PanelT);
                }
                translate([80, PotOffset, 0]) {
                    cylinder(d = PotHoleD, h = PanelT);
                }
                translate([110, PotOffset, 0]) {
                    cylinder(d = PotHoleD, h = PanelT);
                }
        
                // place text for each pot
    
                translate ([13, TextOffset, PanelT - TextExt]) {
                    linear_extrude(height = TextExt) {
                        text("TURN", font = Font1, size = FontSize,
                             direction = "ltr", spacing = 1 );
                    }
                }
                translate ([40, TextOffset, PanelT - TextExt]) {
                    linear_extrude(height = TextExt) {
                        text("HEIGHT", font = Font1, size = FontSize,
                             direction = "ltr", spacing = 1 );
                    }
                }
                translate ([70.5, TextOffset, PanelT - TextExt]) {
                    linear_extrude(height = TextExt) {
                        text("REACH", font = Font1, size = FontSize,
                             direction = "ltr", spacing = 1 );
                    }
                }
                translate ([101.5, TextOffset, PanelT - TextExt]) {
                    linear_extrude(height = TextExt) {
                        text("CLAW", font = Font1, size = FontSize,
                             direction = "ltr", spacing = 1 );
                    }
                }
            }
        }
    }
}

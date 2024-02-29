//
//  stringinharmonicity.hpp
//  The Crystal Harpsichord
//
//  Created by Mahon Hughes on 20/04/2022.
//

#ifndef stringinharmonicity_hpp
#define stringinharmonicity_hpp

#include <stdio.h>
#include <cmath>
#include "pal/pal.h"

class StringInharmonicity {
public:
    float getInharmonicity();
    void draw();
private:
    // inharmonicity parameter components
    float length = 2; // Length (metres)
    float radius = 0.0009; // Radius (metres)
    float E = 2.1*powf(10,11); // Young's modulus
    float tention = 81; // Tention on string (kg)
};
#endif /* stringinharmonicity_hpp */

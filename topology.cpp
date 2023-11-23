

#include "topology.h"

void update_simplex3(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, int& simp_dim, glm::vec3& search_dir)
{
    glm::vec3 n = glm::cross(b-a, c-a); //triangle's normal
    glm::vec3 AO = -a; //direction to origin

    //Determine which feature is closest to origin, make that the new simplex

    simp_dim = 2;
    if(dot(cross(b-a, n), AO)>0){ //Closest to edge AB
        c = a;
        //simp_dim = 2;
        search_dir = cross(cross(b-a, AO), b-a);
        return;
    }
    if(dot(cross(n, c-a), AO)>0){ //Closest to edge AC
        b = a;
        //simp_dim = 2;
        search_dir = cross(cross(c-a, AO), c-a);
        return;
    }

    simp_dim = 3;
    if(dot(n, AO)>0){ //Above triangle
        d = c;
        c = b;
        b = a;
        //simp_dim = 3;
        search_dir = n;
        return;
    }
    //else //Below triangle
    d = b;
    b = a;
    //simp_dim = 3;
    search_dir = -n;
}

//Tetrahedral case
bool update_simplex4(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &d, int &simp_dim, glm::vec3 &search_dir){
    // a is peak/tip of pyramid, BCD is the base (counterclockwise winding order)
    //We know a priori that origin is above BCD and below a

    //Get normals of three new faces
    glm::vec3 ABC = cross(b-a, c-a);
    glm::vec3 ACD = cross(c-a, d-a);
    glm::vec3 ADB = cross(d-a, b-a);

    glm::vec3 AO = -a; //dir to origin
    simp_dim = 3; //hoisting this just cause

    //Plane-test origin with 3 faces

    // Note: Kind of primitive approach used here; If origin is in front of a face, just use it as the new simplex.
    // We just go through the faces sequentially and exit at the first one which satisfies dot product. Not sure this
    // is optimal or if edges should be considered as possible simplices? Thinking this through in my head I feel like
    // this method is good enough. Makes no difference for AABBS, should test with more complex colliders.

    if(dot(ABC, AO)>0){ //In front of ABC
        d = c;
        c = b;
        b = a;
        search_dir = ABC;
        return false;
    }
    if(dot(ACD, AO)>0){ //In front of ACD
        b = a;
        search_dir = ACD;
        return false;
    }
    if(dot(ADB, AO)>0){ //In front of ADB
        c = d;
        d = b;
        b = a;
        search_dir = ADB;
        return false;
    }

    //else inside tetrahedron; enclosed!
    return true;
}

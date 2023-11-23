
#ifndef TOPOLOGY_H
#define TOPOLOGY_H

#include "glm/glm.hpp"

void update_simplex3(glm::vec3& a, glm::vec3& b, glm::vec3& c, glm::vec3& d, int& simp_dim, glm::vec3& search_dir);
bool update_simplex4(glm::vec3 &a, glm::vec3 &b, glm::vec3 &c, glm::vec3 &d, int &simp_dim, glm::vec3 &search_dir);


#endif //TOPOLOGY_H

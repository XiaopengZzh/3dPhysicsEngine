

#ifndef AABB_H
#define AABB_H


#include "glm/glm.hpp"
#include <vector>

using pairlist = std::vector<std::pair<unsigned int, unsigned int>>;


enum class ECoordFlag
{
    LOW,
    HIGH
};

struct valTag
{
    float value;
    unsigned int index;
    ECoordFlag flag;

    valTag() : value(0.0f), index(0), flag(ECoordFlag::LOW) {}
    valTag(float _val, unsigned int _idx, ECoordFlag _flag) : value(_val), index(_idx), flag(_flag) {}
};

struct collisionInfolist
{
    pairlist collidedPairs;
    // minimal translation vectors
    std::vector<glm::vec3> mtvList;
};




class AABB
{
public:

    glm::vec3 vmin, vmax;

    AABB() : vmin(glm::vec3(0.0f)), vmax(glm::vec3(0.0f)) {}

    AABB(const glm::vec3& _vmin, const glm::vec3& _vmax) : vmin(_vmin), vmax(_vmax) {}

    static bool intersect(const AABB& a, const AABB& b);

};

void insertionSort(std::vector<valTag>& list);


pairlist findOverlaps(std::vector<valTag>& taglist);
pairlist findPotentialCollidePairs(pairlist& list1, pairlist& list2, pairlist& list3);
pairlist findPotentialCollidePairsInternal(pairlist& list1, pairlist& list2, pairlist& list3);





#endif //AABB_H



#include "AABB.h"


bool AABB::intersect(const AABB &a, const AABB &b)
{
    return (a.vmin.x <= b.vmax.x && a.vmax.x >= b.vmin.x) &&
            (a.vmin.y <= b.vmax.y && a.vmax.y >= b.vmin.y) &&
            (a.vmin.z <= b.vmax.z && a.vmax.z >= b.vmin.z);
}


void insertionSort(std::vector<valTag>& list)
{
    unsigned int i, j;
    valTag key;
    unsigned int size = list.size();
    for(i = 1; i < size; i++)
    {
        key = list[i];
        j = i - 1;

        while(j >= 0 && list[j].value > key.value)
        {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }
}




pairlist findOverlaps(std::vector<valTag>& taglist)
{
    pairlist res;

    std::vector<valTag> lowTags;

    for(auto &tag : taglist)
    {
        if(tag.flag == ECoordFlag::LOW)
        {
            lowTags.push_back(tag);
        }
        else
        {
            unsigned int knockoutIdx = 0;
            for(unsigned int idx = 0; idx < lowTags.size(); idx++)
            {
                if(lowTags[idx].index == tag.index)
                    knockoutIdx = idx;
                else
                {
                    if(lowTags[idx].index < tag.index)
                        res.emplace_back(lowTags[idx].index, tag.index);
                    else
                        res.emplace_back(tag.index, lowTags[idx].index);
                }
            }
            lowTags.erase(lowTags.begin() + knockoutIdx);
        }
    }

    return res;
}

pairlist findPotentialCollidePairs(pairlist& list1, pairlist& list2, pairlist& list3)
{
    unsigned int size1 = list1.size();
    unsigned int size2 = list2.size();
    unsigned int size3 = list3.size();

    if(size1 < size2)
    {
        if(size1 < size3)
        {
            if(size2 < size3)
                return findPotentialCollidePairsInternal(list1, list2, list3);
            else
                return findPotentialCollidePairsInternal(list1, list3, list2);
        }
        else
            return findPotentialCollidePairsInternal(list3, list1, list2);
    }
    else
    {
        if(size2 < size3)
        {
            if(size1 < size3)
                return findPotentialCollidePairsInternal(list2, list1, list3);
            else
                return findPotentialCollidePairsInternal(list2, list3, list1);
        }
        else
            return findPotentialCollidePairsInternal(list3, list2, list1);
    }
}

pairlist findPotentialCollidePairsInternal(pairlist& list1, pairlist& list2, pairlist& list3)
{
    pairlist res;
    pairlist sift;
    for(auto& pair : list1)
    {
        for(auto& _pair : list2)
        {
            if(pair.first != _pair.first)
                continue;
            else if(pair.second == _pair.second)
                sift.push_back(pair);
        }
    }
    for(auto& pair : sift)
    {
        for(auto& _pair : list3)
        {
            if(pair.first != _pair.first)
                continue;
            else if(pair.second == _pair.second)
                res.push_back(pair);
        }
    }
    return res;
}

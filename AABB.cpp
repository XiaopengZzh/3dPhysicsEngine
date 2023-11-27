

#include "AABB.h"


bool AABB::intersect(const AABB &a, const AABB &b)
{
    return (a.vmin.x <= b.vmax.x && a.vmax.x >= b.vmin.x) &&
            (a.vmin.y <= b.vmax.y && a.vmax.y >= b.vmin.y) &&
            (a.vmin.z <= b.vmax.z && a.vmax.z >= b.vmin.z);
}


void insertionSort(std::vector<valTag>& list, int start, int end)
{
    if(end >= list.size())
        end = list.size() - 1;

    int i = start + 1;
    int j;

    valTag key;

    //unsigned int size = list.size();
    for(; i < end + 1; i++)
    {
        key = list[i];
        j = i - 1;

        while(j >= start && list[j].value > key.value)
        {
            list[j + 1] = list[j];
            j = j - 1;
        }
        list[j + 1] = key;
    }
}

void parallelSort(std::vector<valTag>& list, unsigned int threads_count, unsigned int chunkSize)
{
    // making list to be shared will not lose performance since different tasks deal with different parts of the list
    // They will not access the same number so no synchronization is triggered here.
    for(unsigned int i = 0; i < threads_count; i++)
    {
        #pragma omp task shared(list)
        {
            insertionSort(list, i * chunkSize, (i + 1) * chunkSize - 1);
        }
    }
    #pragma omp taskwait

    for (int i = 0; i < threads_count - 1; i++)
    {
        #pragma omp task shared(list)
        {
            insertionSort(list, chunkSize / 2 + i * chunkSize, chunkSize / 2 + (i + 1) * chunkSize - 1);
        }
    }
    #pragma omp taskwait

    for(unsigned int i = 0; i < threads_count; i++)
    {
        #pragma omp task shared(list)
        {
            insertionSort(list, i * chunkSize, (i + 1) * chunkSize - 1);
        }
    }
    #pragma omp taskwait

    for (int i = 0; i < threads_count - 1; i++)
    {
        #pragma omp task shared(list)
        {
            insertionSort(list, chunkSize / 2 + i * chunkSize, chunkSize / 2 + (i + 1) * chunkSize - 1);
        }
    }
    #pragma omp taskwait

    for(unsigned int i = 0; i < threads_count; i++)
    {
        #pragma omp task shared(list)
        {
            insertionSort(list, i * chunkSize, (i + 1) * chunkSize - 1);
        }
    }
    #pragma omp taskwait

    for (int i = 1; i < threads_count; i++) {
        int j = 0;
        while (list[i * chunkSize + j].value <= list[(i - 1) * chunkSize].value && j < chunkSize) {
            int k = chunkSize * i - 1 + j;
            valTag key = list[i * chunkSize + j];
            while (k >= 0 && list[k].value > key.value) {
                list[k + 1] = list[k];
                k--;
            }
            list[k + 1] = key;
            j++;
        }
    }

    for (int i = 0; i < threads_count - 1; i++)
    {
#pragma omp task shared(list)
        {
            insertionSort(list, chunkSize / 2 + i * chunkSize, chunkSize / 2 + (i + 1) * chunkSize - 1);
        }
    }
#pragma omp taskwait

    for(unsigned int i = 0; i < threads_count; i++)
    {
#pragma omp task shared(list)
        {
            insertionSort(list, i * chunkSize, (i + 1) * chunkSize - 1);
        }
    }
#pragma omp taskwait

    for (int i = threads_count - 2; i > 0; i--) {
        int j = 0;
        while (list[i * chunkSize - 1 - j].value > list[i * chunkSize - 1 + chunkSize / 2].value &&
               j < chunkSize) {
            int k = i * chunkSize - j;
            valTag key = list[i * chunkSize - 1 - j];
            while (k < list.size() && list[k].value < key.value) {
                list[k - 1] = list[k];
                k++;
            }
            list[k - 1] = key;
            j++;
        }
    }

    for (int i = 0; i < threads_count - 1; i++)
    {
#pragma omp task shared(list)
        {
            insertionSort(list, chunkSize / 2 + i * chunkSize, chunkSize / 2 + (i + 1) * chunkSize - 1);
        }
    }
#pragma omp taskwait

    for(unsigned int i = 0; i < threads_count; i++)
    {
#pragma omp task shared(list)
        {
            insertionSort(list, i * chunkSize, (i + 1) * chunkSize - 1);
        }
    }
#pragma omp taskwait

    insertionSort(list, 0, list.size() - 1);

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

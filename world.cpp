
#include "world.h"


World::World()
{
    std::cout << "The world is created." << std::endl;
}

void World::CreateObject(std::shared_ptr<Mesh> mesh, EObjectType type, Shader shader)
{
    ObjectsList.emplace_back(mesh, type, shader);
}

void World::Init()
{

}

void World::Draw(Camera &cam)
{
    for(auto & idx : ObjectsList)
    {
        idx.Draw(cam);
    }
}

void World::physicsRegistration()
{
    auto size = ObjectsList.size();

    for(int idx = 0; idx < size; idx++)
    {
        transforms.emplace_back(ObjectsList[idx].location, ObjectsList[idx].rotation);
        movements.emplace_back(glm::vec3(0.0f), glm::vec3(0.0f));

        std::shared_ptr<Mesh> mesh = ObjectsList[idx].meshInstance.lock();

        if(!mesh)
        {
            std::cout << "weak ptr fails to point to mesh" << std::endl;
            continue;
        }

        bodyInstances.emplace_back(ObjectsList[idx].objectType, &(mesh->collision));

    }

}

void World::simulate(float dt)
{

    integration(dt);

#if RENDER_ENABLED
    syncTransform();
#endif //RENDER_ENABLED

    /*
    glm::vec3 sth;
    if(narrowCheck(0, 10, sth))
    {
        std::cout << sth.x << " " << sth.y << " " << sth.z << std::endl;
    }
     */
    glm::vec3 mtv;
    if(narrowCheck(0, 10, mtv))
    {
        glm::vec3 contactPt = calcContactPoint(mtv, 0, 10);
        resolvePenetration(mtv, 0, 10);
        collisionResponse(mtv, 0, 10, contactPt);
    }
}

void World::integration(float dt)
{
    int size = ObjectsList.size();
    for(int idx = 0; idx < size; idx++)
    {
        if(bodyInstances[idx].objectType == EObjectType::STATIC) continue;

        bodyInstances[idx].pendingLinearImpulse += bodyInstances[idx].collision->mass * glm::vec3(0.0f, -GRAVITY_ACC, 0.0f) * dt;

        movements[idx].momentum += bodyInstances[idx].pendingLinearImpulse;
        movements[idx].angularMomentum += bodyInstances[idx].pendingAngularImpulse;

        transforms[idx].position += dt * (movements[idx].momentum * 2.0f - bodyInstances[idx].pendingLinearImpulse) / (2.0f * bodyInstances[idx].collision->mass);

        // calculate rotation
        glm::mat3 Rot = glm::toMat3(transforms[idx].rotation);
        glm::vec3 angularVelocity = glm::transpose(Rot) * bodyInstances[idx].collision->inertiaTensorInv *
                Rot * (movements[idx].angularMomentum - bodyInstances[idx].pendingAngularImpulse / 2.0f);
        //glm::vec3 angularVelocity =
        glm::quat newRot = transforms[idx].rotation + transforms[idx].rotation * glm::exp(0.5f * glm::quat(0.0f, angularVelocity * dt));
        transforms[idx].rotation = glm::normalize(newRot);

        // refresh pending impulse
        bodyInstances[idx].pendingLinearImpulse = glm::vec3(0.0f);
        bodyInstances[idx].pendingAngularImpulse = glm::vec3(0.0f);
    }

}




void World::syncTransform()
{
    for(int idx = 0; idx < ObjectsList.size(); idx++)
    {
        ObjectsList[idx].location = transforms[idx].position;
        ObjectsList[idx].rotation = transforms[idx].rotation;
    }
}



bool World::narrowCheck(unsigned int idx1, unsigned int idx2, glm::vec3& minimalTranslationVector)
{
    // colliders
    std::vector<glm::vec3> coll1;
    std::vector<glm::vec3> coll2;
    coll1.reserve(bodyInstances[idx1].collision->collisionVertices.size());
    coll2.reserve(bodyInstances[idx2].collision->collisionVertices.size());

    for(auto &v : bodyInstances[idx1].collision->collisionVertices)
    {
        glm::quat v_quat(0.0f, v.x, v.y, v.z);
        glm::quat rotated_quat = transforms[idx1].rotation * v_quat * glm::conjugate(transforms[idx1].rotation);
        coll1.push_back(glm::vec3(rotated_quat.x, rotated_quat.y, rotated_quat.z) + transforms[idx1].position);
    }

    for(auto &v : bodyInstances[idx2].collision->collisionVertices)
    {
        glm::quat v_quat(0.0f, v.x, v.y, v.z);
        glm::quat rotated_quat = transforms[idx2].rotation * v_quat * glm::conjugate(transforms[idx2].rotation);
        coll2.push_back(glm::vec3(rotated_quat.x, rotated_quat.y, rotated_quat.z) + transforms[idx2].position);
    }

    //simplex
    glm::vec3 a, b, c, d;
    glm::vec3 searchDir = transforms[idx1].position - transforms[idx2].position;
    c = support(searchDir, idx2) - support(-searchDir, idx1);
    searchDir = -c;

    b = support(searchDir, idx2) - support(-searchDir, idx1);

    if(glm::dot(b, searchDir) < 0.0f)
        return false;

    searchDir = glm::cross( glm::cross(c - b, -b), c - b );

    if( glm::all( glm::epsilonEqual(searchDir, glm::vec3(0.0f), 0.001f)))
    {
        searchDir = glm::cross( c - b, glm::vec3(1.0f, 0.0f, 0.0f) );
    }
    int simpDim = 2; //simplex dimension

    for(int iter = 0; iter < GJK_MAX_NUM_ITERATIONS; iter++)
    {
        a = support(searchDir, idx2) - support(-searchDir, idx1);
        if(glm::dot(a, searchDir) < 0.0f)
            return false;

        simpDim++;

        if(simpDim == 3)
        {
            update_simplex3(a, b, c, d, simpDim, searchDir);
        }
        else if(update_simplex4(a, b, c, d, simpDim, searchDir))
        {
            //EPA
            minimalTranslationVector = EPA(a, b, c, d, idx1, idx2);
            return true;
        }

    }
    return false;
}


glm::vec3 World::support(glm::vec3 dir, unsigned int idx)
{
    glm::quat vQuat(0.0f, dir.x, dir.y, dir.z);
    glm::quat rotatedQuat = glm::conjugate(transforms[idx].rotation) * vQuat * transforms[idx].rotation;
    dir = glm::vec3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);

    glm::vec3 furthestPoint = bodyInstances[idx].collision->collisionVertices[0];
    float max_dot = glm::dot(furthestPoint, dir);

    for(unsigned int i = 1; i < bodyInstances[idx].collision->collisionVertices.size(); i++)
    {
        float d = glm::dot(bodyInstances[idx].collision->collisionVertices[i], dir);
        if(d > max_dot)
        {
            max_dot = d;
            furthestPoint = bodyInstances[idx].collision->collisionVertices[i];
        }
    }

    glm::quat resQuat = transforms[idx].rotation * glm::quat(0.0f, furthestPoint) * glm::conjugate(transforms[idx].rotation);

    return glm::vec3(resQuat.x, resQuat.y, resQuat.z) + transforms[idx].position;

}

// Todo : When two faces confront and collide, contact point may not be correct.
int World::supportRetDiff(glm::vec3 dir, unsigned int idx, float& diff)
{
    dir = glm::normalize(dir);
    glm::quat rotatedDir = glm::conjugate(transforms[idx].rotation) * glm::quat(0.0f, dir) * transforms[idx].rotation;
    dir = glm::vec3(rotatedDir.x, rotatedDir.y, rotatedDir.z);

    int maxIndex = -1, secondMaxIndex = -1;
    float maxVal = std::numeric_limits<float>::min();
    float secondMaxVal = std::numeric_limits<float>::min();

    int size = bodyInstances[idx].collision->collisionVertices.size();

    for(int i = 0; i < size; i++)
    {
        float d = glm::dot( bodyInstances[idx].collision->collisionVertices[i], dir );

        if(d > maxVal)
        {
            secondMaxVal = maxVal;
            secondMaxIndex = maxIndex;
            maxVal = d;
            maxIndex = i;
        }
        else if(d > secondMaxVal)
        {
            secondMaxVal = d;
            secondMaxIndex = i;
        }

    }
    diff = maxVal - secondMaxVal;
    return maxIndex;
}



glm::vec3 World::EPA(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 d, unsigned int idx1, unsigned int idx2){
    glm::vec3 faces[EPA_MAX_NUM_FACES][4]; //Array of faces, each with 3 verts and a normal

    //Init with final simplex from GJK
    faces[0][0] = a;
    faces[0][1] = b;
    faces[0][2] = c;
    faces[0][3] = glm::normalize(glm::cross(b-a, c-a)); //ABC
    faces[1][0] = a;
    faces[1][1] = c;
    faces[1][2] = d;
    faces[1][3] = glm::normalize(glm::cross(c-a, d-a)); //ACD
    faces[2][0] = a;
    faces[2][1] = d;
    faces[2][2] = b;
    faces[2][3] = glm::normalize(glm::cross(d-a, b-a)); //ADB
    faces[3][0] = b;
    faces[3][1] = d;
    faces[3][2] = c;
    faces[3][3] = glm::normalize(glm::cross(d-b, c-b)); //BDC

    int num_faces=4;
    int closest_face;

    for(int iterations=0; iterations < EPA_MAX_NUM_ITERATIONS; iterations++){
        //Find face that's closest to origin
        float min_dist = glm::dot(faces[0][0], faces[0][3]);
        closest_face = 0;
        for(int i=1; i < num_faces; i++){
            float dist = glm::dot(faces[i][0], faces[i][3]);
            if(dist<min_dist){
                min_dist = dist;
                closest_face = i;
            }
        }

        //search normal to face that's closest to origin
        glm::vec3 search_dir = faces[closest_face][3];
        glm::vec3 p = support(search_dir, idx2) - support(-search_dir, idx1);

        if(glm::dot(p, search_dir)-min_dist<EPA_TOLERANCE){
            //Convergence (new point is not significantly further from origin)
            return faces[closest_face][3]* glm::dot(p, search_dir); //dot vertex with normal to resolve collision along normal!
        }

        glm::vec3 loose_edges[EPA_MAX_NUM_LOOSE_EDGES][2]; //keep track of edges we need to fix after removing faces
        int num_loose_edges = 0;

        //Find all triangles that are facing p
        for(int i=0; i<num_faces; i++)
        {
            if(glm::dot(faces[i][3], p-faces[i][0] )>0) //triangle i faces p, remove it
            {
                //Add removed triangle's edges to loose edge list.
                //If it's already there, remove it (both triangles it belonged to are gone)
                for(int j=0; j<3; j++) //Three edges per face
                {
                    glm::vec3 current_edge[2] = {faces[i][j], faces[i][(j+1)%3]};
                    bool found_edge = false;
                    for(int k=0; k<num_loose_edges; k++) //Check if current edge is already in list
                    {
                        if(glm::all( glm::epsilonEqual(loose_edges[k][1], current_edge[0], 0.0001f) ) && glm::all( glm::epsilonEqual( loose_edges[k][0], current_edge[1], 0.0001f ) )){
                            //Edge is already in the list, remove it
                            //THIS ASSUMES EDGE CAN ONLY BE SHARED BY 2 TRIANGLES (which should be true)
                            //THIS ALSO ASSUMES SHARED EDGE WILL BE REVERSED IN THE TRIANGLES (which
                            //should be true provided every triangle is wound CCW)
                            loose_edges[k][0] = loose_edges[num_loose_edges-1][0]; //Overwrite current edge
                            loose_edges[k][1] = loose_edges[num_loose_edges-1][1]; //with last edge in list
                            num_loose_edges--;
                            found_edge = true;
                            k=num_loose_edges; //exit loop because edge can only be shared once
                        }
                    }//endfor loose_edges

                    if(!found_edge){ //add current edge to list
                        // assert(num_loose_edges<EPA_MAX_NUM_LOOSE_EDGES);
                        if(num_loose_edges>=EPA_MAX_NUM_LOOSE_EDGES) break;
                        loose_edges[num_loose_edges][0] = current_edge[0];
                        loose_edges[num_loose_edges][1] = current_edge[1];
                        num_loose_edges++;
                    }
                }

                //Remove triangle i from list
                faces[i][0] = faces[num_faces-1][0];
                faces[i][1] = faces[num_faces-1][1];
                faces[i][2] = faces[num_faces-1][2];
                faces[i][3] = faces[num_faces-1][3];
                num_faces--;
                i--;
            }//endif p can see triangle i
        }//endfor num_faces

        //Reconstruct polytope with p added
        for(int i=0; i<num_loose_edges; i++)
        {
            // assert(num_faces<EPA_MAX_NUM_FACES);
            if(num_faces>=EPA_MAX_NUM_FACES) break;
            faces[num_faces][0] = loose_edges[i][0];
            faces[num_faces][1] = loose_edges[i][1];
            faces[num_faces][2] = p;
            faces[num_faces][3] = glm::normalize(glm::cross(loose_edges[i][0]-loose_edges[i][1], loose_edges[i][0]-p));

            //Check for wrong normal to maintain CCW winding
            float bias = 0.000001f; //in case dot result is only slightly < 0 (because origin is on face)
            if(glm::dot(faces[num_faces][0], faces[num_faces][3])+bias < 0){
                glm::vec3 temp = faces[num_faces][0];
                faces[num_faces][0] = faces[num_faces][1];
                faces[num_faces][1] = temp;
                faces[num_faces][3] = -faces[num_faces][3];
            }
            num_faces++;
        }
    } //End for iterations

    std::cout << "EPA did not converge. " << std::endl;
    //Return most recent closest point
    return faces[closest_face][3] * glm::dot(faces[closest_face][0], faces[closest_face][3]);
}

// Todo : if both objects are static, stop collision detection
glm::vec3 World::calcContactPoint(glm::vec3 minimalTranslationalVector, unsigned int idx1, unsigned int idx2)
{
    float factor1 = (bodyInstances[idx1].objectType == EObjectType::STATIC ? 0.0f : 1.0f);
    float factor2 = (bodyInstances[idx2].objectType == EObjectType::STATIC ? 0.0f : 1.0f);

    float diff1, diff2;

    float mass1 = factor1 / bodyInstances[idx1].collision->mass;
    float mass2 = factor2 / bodyInstances[idx2].collision->mass;

    int contactPtIdx1 = supportRetDiff(-minimalTranslationalVector, idx1, diff1);
    int contactPtIdx2 = supportRetDiff(minimalTranslationalVector, idx2, diff2);

    glm::vec3 contactPt;

    // Todo : check if both are infinite small.
    if(diff1 < diff2)
    {
        contactPt = bodyInstances[idx1].collision->collisionVertices[contactPtIdx1];
        contactPt = transforms[idx1].rotation * contactPt + transforms[idx1].position + minimalTranslationalVector * (mass1 / (mass1 + mass2));
    }
    else
    {
        contactPt = bodyInstances[idx2].collision->collisionVertices[contactPtIdx2];
        contactPt = transforms[idx2].rotation * contactPt + transforms[idx2].position - minimalTranslationalVector * (mass2 / (mass1 + mass2));
    }

    return contactPt;
}

// Todo : linear system
// call resolvePenetration after calcContactPoint
void World::resolvePenetration(glm::vec3 minimalTranslationalVector, unsigned int idx1, unsigned int idx2)
{
    float factor1 = (bodyInstances[idx1].objectType == EObjectType::STATIC ? 0.0f : 1.0f);
    float factor2 = (bodyInstances[idx2].objectType == EObjectType::STATIC ? 0.0f : 1.0f);

    float mass1 = factor1 / bodyInstances[idx1].collision->mass;
    float mass2 = factor2 / bodyInstances[idx2].collision->mass;

    transforms[idx1].position += minimalTranslationalVector * (mass1 / (mass1 + mass2));
    transforms[idx2].position -= minimalTranslationalVector * (mass2 / (mass1 + mass2));
}

void World::collisionResponse(glm::vec3 mtv, unsigned int idx1, unsigned int idx2, glm::vec3 contactPt)
{
    glm::vec3 collisionNormal = glm::normalize(mtv);

    float factor1 = (bodyInstances[idx1].objectType == EObjectType::STATIC ? 0.0f : 1.0f);
    float factor2 = (bodyInstances[idx2].objectType == EObjectType::STATIC ? 0.0f : 1.0f);

    float mass1 = factor1 / bodyInstances[idx1].collision->mass;
    float mass2 = factor2 / bodyInstances[idx2].collision->mass;

    glm::mat3 Rot1 = glm::toMat3(transforms[idx1].rotation);
    glm::mat3 invI1 = factor1 * glm::transpose(Rot1) * bodyInstances[idx1].collision->inertiaTensorInv * Rot1;

    glm::mat3 Rot2 = glm::toMat3(transforms[idx2].rotation);
    glm::mat3 invI2 = factor2 * glm::transpose(Rot2) * bodyInstances[idx2].collision->inertiaTensorInv * Rot2;


    glm::vec3 r_AP = contactPt - transforms[idx1].position;
    glm::vec3 r_BP = contactPt - transforms[idx2].position;

    glm::vec3 vel_AP, vel_BP;
    if(bodyInstances[idx1].objectType == EObjectType::STATIC)
    {
        vel_AP = glm::vec3(0.0f);
    }
    else
    {
        vel_AP = movements[idx1].momentum / bodyInstances[idx1].collision->mass +
                glm::cross(invI1 * movements[idx1].angularMomentum, r_AP);
    }
    if(bodyInstances[idx2].objectType == EObjectType::STATIC)
    {
        vel_BP = glm::vec3(0.0f);
    }
    else
    {
        vel_BP = movements[idx2].momentum / bodyInstances[idx2].collision->mass +
                 glm::cross(invI2 * movements[idx2].angularMomentum, r_BP);
    }

    glm::vec3 impulseJ = -(1 + bodyInstances[idx1].restitution * bodyInstances[idx2].restitution) * collisionNormal;
    impulseJ *= glm::dot(collisionNormal, vel_AP - vel_BP);

    float inertiaA = glm::dot(collisionNormal, glm::cross(invI1 * glm::cross(r_AP, collisionNormal) , r_AP));
    float inertiaB = glm::dot(collisionNormal, glm::cross(invI2 * glm::cross(r_BP, collisionNormal) , r_BP));

    impulseJ *= 1.0f / (mass1 + mass2 + inertiaA + inertiaB);

    bodyInstances[idx1].pendingLinearImpulse += impulseJ;
    bodyInstances[idx2].pendingLinearImpulse -= impulseJ;
    bodyInstances[idx1].pendingAngularImpulse += glm::cross(r_AP, impulseJ);
    bodyInstances[idx2].pendingAngularImpulse -= glm::cross(r_BP, impulseJ);

}


















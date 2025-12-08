// Minimal test program to verify Assimp model loading without OpenGL
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>

bool testLoadModel(const std::string& filepath) {
    std::cout << "Testing: " << filepath << "... ";
    
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filepath,
        aiProcess_Triangulate |
        aiProcess_GenSmoothNormals |
        aiProcess_JoinIdenticalVertices |
        aiProcess_PreTransformVertices);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cout << "FAILED: " << importer.GetErrorString() << std::endl;
        return false;
    }
    
    int totalVertices = 0;
    for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
        totalVertices += scene->mMeshes[i]->mNumVertices;
    }
    
    std::cout << "SUCCESS - " << scene->mNumMeshes << " meshes, " 
              << totalVertices << " vertices" << std::endl;
    return true;
}

int main() {
    std::cout << "===== Assimp Model Loading Test =====" << std::endl;
    std::cout << std::endl;
    
    int passed = 0;
    int total = 0;
    
    // Test 1: Player.blend
    total++;
    if (testLoadModel("models/98-hikerbasemesh/Player.blend")) passed++;
    
    // Test 2: cerca.blend (fence)
    total++;
    if (testLoadModel("models/6od9waw1za0w-fence/fence/cerca.blend")) passed++;
    
    // Test 3: StreetLamp.blend
    total++;
    if (testLoadModel("models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.blend")) passed++;
    
    // Test 4: Rock1.blend
    total++;
    if (testLoadModel("models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.blend")) passed++;
    
    // Test 5: RockSet.blend
    total++;
    if (testLoadModel("models/xvs3wxwo2o-RockSet_MadeByTyroSmith/RockSet/RockSet.blend")) passed++;
    
    // Test 6: farmhouse_obj.obj
    total++;
    if (testLoadModel("models/4vd2sk31doow-farmhouse_maya16/Farmhouse Maya 2016 Updated/farmhouse_obj.obj")) passed++;
    
    // Test 7: Tree1.3ds
    total++;
    if (testLoadModel("models/tree/tree1_3ds/Tree1.3ds")) passed++;
    
    // Test 8: Tree.obj
    total++;
    if (testLoadModel("models/15od5xhlv2jc-Tree_02/Tree 02/Tree.obj")) passed++;
    
    // Test 9: Wheat field OBJ
    total++;
    if (testLoadModel("models/10458_Wheat_Field_v1_L3.123c5ecd0518-ae16-4fee-bf80-4177de196237/10458_Wheat_Field_v1_L3.123c5ecd0518-ae16-4fee-bf80-4177de196237/10458_Wheat_Field_v1_max2010_it2.obj")) passed++;
    
    // Test 10: Carrot OBJ
    total++;
    if (testLoadModel("models/Carrot_v01_l3.123c059c383a-f43b-48c0-b28a-bec318013e17/Carrot_v01_l3.123c059c383a-f43b-48c0-b28a-bec318013e17/10170_Carrot_v01_L3.obj")) passed++;
    
    std::cout << std::endl;
    std::cout << "===== Test Results =====" << std::endl;
    std::cout << "Passed: " << passed << "/" << total << std::endl;
    
    if (passed == total) {
        std::cout << "All tests PASSED! ✓" << std::endl;
        return 0;
    } else {
        std::cout << "Some tests FAILED" << std::endl;
        return 1;
    }
}

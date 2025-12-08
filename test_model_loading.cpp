// Test program to verify Assimp model loading
#include "ModelLoader.h"
#include <iostream>

int main() {
    std::cout << "Testing Assimp model loading..." << std::endl;
    
    // Initialize OpenGL context (minimal)
    // Note: In a real scenario, this would need a proper GL context
    // For testing, we'll just test the loading API
    
    Model testModel;
    
    // Test 1: Load Player.blend
    std::cout << "\nTest 1: Loading Player.blend..." << std::endl;
    if (loadModel("models/98-hikerbasemesh/Player.blend", testModel)) {
        std::cout << "SUCCESS: Player.blend loaded" << std::endl;
        std::cout << "  Meshes: " << testModel.meshes.size() << std::endl;
        for (size_t i = 0; i < testModel.meshes.size(); i++) {
            std::cout << "  Mesh " << i << ": " << testModel.meshes[i].vertices.size() << " vertices" << std::endl;
        }
    } else {
        std::cout << "FAILED: Could not load Player.blend" << std::endl;
    }
    
    // Test 2: Load fence.blend
    std::cout << "\nTest 2: Loading cerca.blend..." << std::endl;
    Model fenceModel;
    if (loadModel("models/6od9waw1za0w-fence/fence/cerca.blend", fenceModel)) {
        std::cout << "SUCCESS: cerca.blend loaded" << std::endl;
        std::cout << "  Meshes: " << fenceModel.meshes.size() << std::endl;
    } else {
        std::cout << "FAILED: Could not load cerca.blend" << std::endl;
    }
    
    // Test 3: Load StreetLamp.blend
    std::cout << "\nTest 3: Loading StreetLamp.blend..." << std::endl;
    Model lampModel;
    if (loadModel("models/s3duldjjt9fk-StreetLampByTyroSmith/Street Lamp/StreetLamp.blend", lampModel)) {
        std::cout << "SUCCESS: StreetLamp.blend loaded" << std::endl;
        std::cout << "  Meshes: " << lampModel.meshes.size() << std::endl;
    } else {
        std::cout << "FAILED: Could not load StreetLamp.blend" << std::endl;
    }
    
    // Test 4: Load Rock1.blend
    std::cout << "\nTest 4: Loading Rock1.blend..." << std::endl;
    Model rockModel;
    if (loadModel("models/1elmla01hh-Rock1_BYTyroSmith/Rock1/Rock1.blend", rockModel)) {
        std::cout << "SUCCESS: Rock1.blend loaded" << std::endl;
        std::cout << "  Meshes: " << rockModel.meshes.size() << std::endl;
    } else {
        std::cout << "FAILED: Could not load Rock1.blend" << std::endl;
    }
    
    // Test 5: Load OBJ file
    std::cout << "\nTest 5: Loading farmhouse_obj.obj..." << std::endl;
    Model houseModel;
    if (loadModel("models/4vd2sk31doow-farmhouse_maya16/Farmhouse Maya 2016 Updated/farmhouse_obj.obj", houseModel)) {
        std::cout << "SUCCESS: farmhouse_obj.obj loaded" << std::endl;
        std::cout << "  Meshes: " << houseModel.meshes.size() << std::endl;
    } else {
        std::cout << "FAILED: Could not load farmhouse_obj.obj" << std::endl;
    }
    
    // Test 6: Load 3DS file
    std::cout << "\nTest 6: Loading Tree1.3ds..." << std::endl;
    Model treeModel;
    if (loadModel("models/tree/tree1_3ds/Tree1.3ds", treeModel)) {
        std::cout << "SUCCESS: Tree1.3ds loaded" << std::endl;
        std::cout << "  Meshes: " << treeModel.meshes.size() << std::endl;
    } else {
        std::cout << "FAILED: Could not load Tree1.3ds" << std::endl;
    }
    
    std::cout << "\nAll tests completed!" << std::endl;
    return 0;
}

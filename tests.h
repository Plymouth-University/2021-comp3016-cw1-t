/**
* DO NOT MODIFY! 
* Test File for the FileReader
* A final version will be relased close to the deadline with mroe detailed tests.
*/
#pragma once 
#include <string>
#include "../FileReader.h" 
//#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"


TEST_CASE( "STD 1: Creating a FileReader object"  ) {
    FileReader reader = FileReader();
    
    CHECK(reader.currentFile().empty());
}

TEST_CASE( "STD 2: Opening File from data folder"  ) {
    FileReader reader = FileReader();

    CHECK( reader.openFile("data/cube.obj") == true );
}

TEST_CASE("STD 3: Closing File from data folder" ) {
    FileReader reader = FileReader();
    reader.openFile("data/cube.obj");
    CHECK(reader.closeFile("data/cube.obj") == true);
    CHECK(reader.closeFile("data/cube.obj") == false);
}

TEST_CASE("STD 4: Closing currently open File" ) {
    FileReader reader = FileReader();
    reader.openFile("data/cube.obj");
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.closeCurrentFile() == false);
}



TEST_CASE("ADV 1: Checking repeated Operations 2") {
    FileReader reader = FileReader();

    reader.openFile("data/cube.obj");
    CHECK(reader.switchCurrentFile("data/cube.obj") == true);
    reader.closeFile("data/cube.obj");
    CHECK(reader.currentFile().empty());
    CHECK(reader.openFile("data/cube.obj") == true);
    CHECK(reader.openFile("data/cube.obj") == false);
    CHECK(reader.currentFile().compare("data/cube.obj") == 0);
    CHECK(reader.openFile("data/boat.gltf") == true);
    CHECK(reader.openFile("data/cube.obj") == false);
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.closeCurrentFile() == true);
}

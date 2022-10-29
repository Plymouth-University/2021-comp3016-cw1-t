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

TEST_CASE("STD 5: Opening and Reading a simple OBJ File") {
    FileReader reader = FileReader();
    std::string fileName = "data/cube.obj";
    reader.openFile(fileName);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    std::vector<std::string> lines = reader.getLines(0, 20);
    std::string comparison = "v 1.000000 -1.000000 1.000000";
    REQUIRE(lines.size() == 21);
    CHECK(lines[14].compare(comparison) == 0);

    CHECK(reader.lineCount() == 73);

    reader.closeCurrentFile();
}

TEST_CASE("STD 6: Opening and Reading a large OBJ File and trying to load lines.") {
    FileReader reader = FileReader();
    std::string fileName = "data/boat.obj";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    CHECK(reader.getLine(211222).length() == 0); //going beyond the length of the file on purpose

    REQUIRE(reader.lineCount() == 119361);

    std::string line = reader.getLine(92205);
    std::string comparison = "o Cylinder.3_2";
    REQUIRE(line.length() > 1);
    CHECK(line.compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("STD 7: Trying to get an element from a simple OBJ file") {
    FileReader reader = FileReader();
    std::string fileName = "data/cube.obj";
    CHECK(reader.openFile(fileName) == true);

    std::vector<std::string> lines = reader.getElement("object");
    std::string comparison = "Cube_Cube.001";
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison) == 0);

    lines = reader.getElement("object");
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison) == 0);

    lines = reader.getNextElement("object"); // moves cursor
    std::string comparison2 = "";
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison2) == 0);

    reader.resetElement("object"); //resetting cursor be beginning of vertrices
    lines = reader.getElement("object");
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("STD 8: Trying to get an element and attribute from a simple OBJ file") {
    FileReader reader = FileReader();
    std::string fileName = "data/cube.obj";
    CHECK(reader.openFile(fileName) == true);

    std::string line = reader.getElementAttribute("object", "vertex");
    REQUIRE(line.length() > 1);
    std::string comparison = "1.000000 1.000000 -1.000000";
    CHECK(line.compare(comparison) == 0);

    line = reader.getElementAttribute("vertex", "vertex-texture");
    REQUIRE(line.length() > 1);
    comparison = "0.625000 0.500000";
    CHECK(line.compare(comparison) == 0);

    line = reader.getElementAttribute("vertex", "vertex-normal");
    REQUIRE(line.length() >= 0);
    comparison = "0.0000 1.0000 0.0000";
    CHECK(line.compare(comparison) == 0);

    reader.closeCurrentFile();
}


TEST_CASE("STD 9: Trying to get an element attribute from an OBJ file") {
    FileReader reader = FileReader();
    std::string fileName = "data/boat.obj";
    CHECK(reader.openFile(fileName) == true);

    std::string value = reader.getElementAttribute("object", "face");
    std::string comparison = "1/1/1 2/2/1 4/4/1";
    value = reader.getNextElementAttribute("object", "face");
    comparison = "1/1/1 4/4/1 6/6/1";
    CHECK(value.compare(comparison) == 0);
    value = reader.getNextElementAttribute("object", "face");
    comparison = "7/7/1 9/9/2 11/11/1";
    CHECK(value.compare(comparison) == 0);
    value = reader.getNextElementAttribute("object", "face");
    comparison = "7/7/1 11/11/1 12/12/1";
    CHECK(value.compare(comparison) == 0);

    reader.getNextElement("object");
    value = reader.getElementAttribute("object", "face");
    comparison = "364/364/11 398/398/12 399/399/13";
    CHECK(value.compare(comparison) == 0);

    reader.resetElement("object");
    value = reader.getElementAttribute("object", "face");
    comparison = "1/1/1 2/2/1 4/4/1";
    CHECK(value.compare(comparison) == 0);

    value = reader.getElementAttribute("object", 3, "face"); // get's the n+1th attribute
    comparison = "7/7/1 11/11/1 12/12/1";
    CHECK(value.compare(comparison) == 0);


    reader.closeCurrentFile();
}

TEST_CASE("STD 10: Opening and Reading a large OBJ File and trying to load too many lines.") {
    FileReader reader = FileReader();
    std::string fileName = "data/pouf.obj";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    CHECK(reader.getLines(0, 2500000).size() > 0); //going beyond the length of the file on purpose
    std::vector<std::string> lines = reader.getLines(0, 2500000);

    CHECK(lines.size() == 207776);
    reader.closeCurrentFile();
}

TEST_CASE("STD 11: Opening an invalid File from data folder") {
    FileReader reader = FileReader();
    //wrong file name supplied
    CHECK(reader.openFile("data/cube.obje") == false);
    CHECK(reader.openFile("data/large..dae") == false);
    CHECK(reader.openFile("datas/cube.obj") == false);
    CHECK(reader.openFile("data/ cube.obj") == false);
}

TEST_CASE("STD 12: Checking repeated Operations") {
    FileReader reader = FileReader();
    reader.openFile("data/cube.obj");
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.closeCurrentFile() == false);
    reader.openFile("data/boat.obj");
    CHECK(reader.closeFile("data/cube.obj") == false);
    CHECK(reader.getLines(0, 2).size() > 0);
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.currentFile().empty());
    CHECK(reader.getLines(0, 1).size() == 0);
}

TEST_CASE("STD 13: Multiple Files Can be Loaded and Inspected for their Elements") {
    FileReader reader = FileReader();

    //inspecting first file
    std::string fileName = "data/boat.obj";
    CHECK(reader.openFile(fileName) == true);

    std::string line = reader.getElementAttribute("object", 2,"vertex");
    REQUIRE(line.length() > 1);
    std::string comparison = "-5.507223 25.197802 38.613228";
    CHECK(line.compare(comparison) == 0);

    //inspecting second file
    reader.openFile("data/cube.obj");
    line = reader.getElementAttribute("object", 0,"vertex-normal");
    REQUIRE(line.length() > 1);
    comparison = "0.0000 1.0000 0.0000";
    CHECK(line.compare(comparison) == 0);


    CHECK(reader.switchCurrentFile("data/boat.obj"));
    line = reader.getElementAttribute("object", "vertex");
    REQUIRE(line.length() > 1);
    comparison = "2.210220 33.775902 38.926983";
    CHECK(line.compare(comparison) == 0);

    reader.closeCurrentFile();
    reader.closeCurrentFile();
}

TEST_CASE("STD 14: Checking Wrong items in File") {
    FileReader reader = FileReader();

    REQUIRE(reader.openFile("data/cube.obj"));
    std::string comparison = "18/21/6 1/23/6 5/24/6";
    
    std::string value = reader.getElementAttribute("object", "red");
    CHECK(value.compare("") == 0);
    
    value = reader.getElementAttribute("object", "faces");
    CHECK(value.compare("") == 0);

    value = reader.getElementAttribute("object", 11, "face");
    CHECK(value.compare(comparison) == 0);

    value = reader.getNextElementAttribute("object", "faces");
    CHECK(value.compare("") == 0);

    reader.closeCurrentFile();
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

TEST_CASE("ADV 2: Checking Additional items in File") {
    FileReader reader = FileReader();

    REQUIRE(reader.openFile("data/pouf.gltf"));
    REQUIRE(reader.openFile("data/cube.obj"));
    REQUIRE(reader.openFile("data/boat.obj"));
    CHECK(reader.switchCurrentFile("data/cube.obj") == true);
    std::string comparison = "18/21/6 1/23/6 5/24/6";

    std::string value = reader.getElementAttribute("object", 11, "face");
    CHECK(value.compare(comparison) == 0);

    value = reader.getElementAttribute("object", "face");
    CHECK(value.compare(comparison) == 0);

    REQUIRE(reader.switchCurrentFile("data/boat.obj") == true);

    value = reader.getElementAttribute("object", 1, "face");
    comparison = "1/1/1 4/4/1 6/6/1";
    CHECK(value.compare(comparison) == 0);

    reader.getNextElement("object");
    value = reader.getElementAttribute("object", 2, "face");
    comparison = "376/376/12 399/399/13 400/400/14";
    CHECK(value.compare(comparison) == 0);

    value = reader.getNextElement("object")[0];
    comparison = "Floor:2";
    CHECK(value.compare(comparison) == 0);

    value = reader.getElementAttribute("object", "vertex");
    comparison = "-0.326423 51.591156 31.248625";
    CHECK(value.compare(comparison) == 0);

    reader.closeCurrentFile();
}
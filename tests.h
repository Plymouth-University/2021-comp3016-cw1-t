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

    CHECK( reader.openFile("data/simple.ply") == true );
}

TEST_CASE("STD 3: Closing File from data folder" ) {
    FileReader reader = FileReader();
    reader.openFile("data/simple.ply");
    CHECK(reader.closeFile("data/simple.ply") == true);
    CHECK(reader.closeFile("data/simple.ply") == false);
}

TEST_CASE("STD 4: Closing currently open File" ) {
    FileReader reader = FileReader();
    reader.openFile("data/simple.ply");
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.closeCurrentFile() == false);
}

TEST_CASE("STD 5: Opening and Reading a simple PLY File") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.ply";
    reader.openFile(fileName);
    std::string checkName = reader.currentFile();
  
    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    std::vector<std::string> lines = reader.getLines(0, 55);
    std::string comparison = "-0.500000 0.500000 -0.500000 -1.000000 0.000000 0.000000 0.595141 0.509132 255 255 255 255";
    CHECK(lines.size() == 55);
    CHECK(lines[40].compare(comparison) == 0);

    CHECK(reader.lineCount() == 57);

    reader.closeCurrentFile();
}

TEST_CASE("STD 6: Opening and Reading a large PLY File and trying to load lines.") {
    FileReader reader = FileReader();
    std::string fileName = "data/large.ply";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    CHECK(reader.getLine(117910).length() == 0); //going beyond the length of the file on purpose
   
    CHECK(reader.lineCount() == 117907);

    std::string line = reader.getLine(117904);
    std::string comparison = "3 44876 44874 44885";
    REQUIRE(line.length() > 1);
    CHECK(line.compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("STD 7: Trying to get an element from a simple PLY file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple.ply";
    CHECK(reader.openFile(fileName) == true);
    
    std::vector<std::string> lines = reader.getElement("vertex");
    std::string comparison = "0.500000 -0.500000 -0.500000 0.000000 0.000000 -1.000000 0.593540 -0.000303 255 255 255 255";
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison)==0); 

    lines = reader.getElement("vertex");
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison) == 0);

    lines = reader.getNextElement("vertex"); // moves cursor
    std::string comparison2 = "-0.500000 0.500000 -0.500000 0.000000 0.000000 -1.000000 0.340409 0.249793 255 255 255 255";
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison2) == 0); 

    reader.resetElement("vertex"); //resetting cursor be beginning of vertrices
    lines = reader.getElement("vertex");
    REQUIRE(lines.size() >= 1);
    CHECK(lines[0].compare(comparison) == 0);
    reader.closeCurrentFile();
}

TEST_CASE("STD 8: Trying to get an element and attribute from a simple PLY file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple3.ply";
    CHECK(reader.openFile(fileName) == true);

    std::string line = reader.getElementAttribute("vertex","red");
    REQUIRE(line.length() > 1);
    std::string comparison = "138";
    CHECK(line.compare(comparison) == 0); 

    line = reader.getElementAttribute("vertex", "green");
    REQUIRE(line.length() > 1);
    comparison = "70";
    CHECK(line.compare(comparison) == 0);

    line = reader.getElementAttribute("vertex", "omega");
    REQUIRE(line.length() >= 0);
    comparison = "";
    CHECK(line.compare(comparison) == 0);

    reader.closeCurrentFile();
}


TEST_CASE("STD 9: Trying to get an element attribute from a simple PLY file") {
    FileReader reader = FileReader();
    std::string fileName = "data/simple3.ply";
    CHECK(reader.openFile(fileName) == true);

    std::string value = reader.getElementAttribute("face", "vertex_indices");
    std::string comparison = "98";
    value = reader.getNextElementAttribute("face", "vertex_indices");
    comparison = "99";
    CHECK(value.compare(comparison) == 0); 
    value = reader.getNextElementAttribute("face", "vertex_indices");
    comparison = "100";
    CHECK(value.compare(comparison) == 0);
    value = reader.getNextElementAttribute("face", "vertex_indices");
    comparison = "";
    CHECK(value.compare(comparison) == 0);

    reader.getNextElement("face");
    value = reader.getElementAttribute("face", "vertex_indices");
    comparison = "101";
    CHECK(value.compare(comparison) == 0);
    reader.resetElement("face");
    value = reader.getElementAttribute("face", "vertex_indices");
    comparison = "98";
    CHECK(value.compare(comparison) == 0);

    value = reader.getElementAttribute("face", 5,"vertex_indices");
    comparison = "113";
    CHECK(value.compare(comparison) == 0);


    reader.closeCurrentFile();
}

TEST_CASE("STD 10: Opening and Reading a large PLY File and trying to load too many lines.") {
    FileReader reader = FileReader();
    std::string fileName = "data/large.ply";
    CHECK(reader.openFile(fileName) == true);
    std::string checkName = reader.currentFile();

    CHECK(checkName.compare(checkName.size() - fileName.size(), fileName.size(), fileName) == 0);
    CHECK(reader.getLines(0, 117910).size() > 0); //going beyond the length of the file on purpose
    std::vector<std::string> lines = reader.getLines(0, 117910);

    CHECK(lines.size() == 117905);
    reader.closeCurrentFile();
}

TEST_CASE("STD 11: Opening an invalid File from data folder") {
    FileReader reader = FileReader();
    //wrong file name supplied
    CHECK(reader.openFile("data/simples.ply") == false);
    CHECK(reader.openFile("data/simple3.pl") == false);
    CHECK(reader.openFile("data/simple.fbx") == false);
    CHECK(reader.openFile("data/ simple3.ply") == false);
}

TEST_CASE("STD 12: Checking repeated Operations") {
    FileReader reader = FileReader();
    reader.openFile("data/simple.ply");
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.closeCurrentFile() == false);
    reader.openFile("data/simple-tex.ply");
    CHECK(reader.closeFile("data/simple.ply") == false);
    CHECK(reader.closeCurrentFile() == true);
    CHECK(reader.currentFile().empty());
    CHECK(reader.getLines(0, 1).size() == 0);
}

TEST_CASE("STD 13: Multiple Files Can be Loaded and Inspected for their Elements") {
    FileReader reader = FileReader();

    //inspecting first file
    std::string fileName = "data/simple3.ply";
    CHECK(reader.openFile(fileName) == true);

    std::string line = reader.getElementAttribute("vertex", "red");
    REQUIRE(line.length() > 1);
    std::string comparison = "97";
    CHECK(line.compare(comparison) == 0);

    //inspecting second file
    reader.openFile("data/simple.ply");
    line = reader.getElementAttribute("vertex", "red");
    REQUIRE(line.length() > 1);
    comparison = "255";
    CHECK(line.compare(comparison) == 0);


    CHECK(reader.switchCurrentFile("data/simple3.ply"));
    line = reader.getElementAttribute("vertex", "alpha");
    REQUIRE(line.length() > 1);
    comparison = "255";
    CHECK(line.compare(comparison) == 0);

    reader.closeCurrentFile();
    reader.closeCurrentFile();
}

TEST_CASE("STD 14: Checking Another Large File") {
    FileReader reader = FileReader();

    REQUIRE(reader.openFile("data/LowPolyBoat-coloured.ply"));

    std::string value = reader.getElementAttribute("face", "red");
    std::string comparison = "57";
    CHECK(value.compare(comparison) == 0);
    reader.closeCurrentFile();
}




TEST_CASE("ADV 1: Checking repeated Operations 2") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.ply");
    CHECK(reader.switchCurrentFile("data/simple.ply") == true);
    reader.closeFile("data/simple.ply");
    CHECK(reader.currentFile().empty());
    CHECK(reader.openFile("data/simple.ply") == true);
    CHECK(reader.openFile("data/simple.ply") == false);
    CHECK(reader.currentFile().compare("data/simple.ply") == 0);
    CHECK(reader.openFile("data/simple3.ply") == true);
    CHECK(reader.openFile("data/simple.ply") == false);

    CHECK(reader.getLines(0, 212)[100].compare("-0.5 -0.5 0.2499999 -0.7071068 -0.7071068 0 80 113 58 255") == 0);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.closeCurrentFile() == true);
}

TEST_CASE("ADV 2: Checking repeated Operations 3") {
    FileReader reader = FileReader();

    reader.openFile("data/LowPolyBoat-coloured.ply");
    CHECK(reader.openFile("data/LowPolyBoat-coloured.ply") == false);
    CHECK(reader.currentFile().compare("data/LowPolyBoat-coloured.ply") == 0);
    CHECK(reader.openFile("data/simple-high-res.ply") == false);
    CHECK(reader.currentFile().compare("data/simple-high-res.ply") == 0);
    CHECK(reader.getLines(0, 1).size() > 0);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.closeCurrentFile());
}

TEST_CASE("ADV 3: Checking repeated Operations 4") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.ply");
    reader.openFile("data/advanced.ply");
    CHECK(reader.closeFile("data/simple.ply"));
    CHECK(reader.currentFile().empty());
    CHECK(reader.closeCurrentFile()==false);
}

TEST_CASE("ADV 4: Checking repeated Operations 5") {
    FileReader reader = FileReader();

    reader.openFile("data/simple.ply");
    reader.openFile("data/simple-highres.ply");
    CHECK(reader.closeFile("data/simple.ply"));
    CHECK(reader.currentFile().compare("data/simple-highres.ply") == 0);
    CHECK(reader.getLines(0, 1).size() > 0);
    CHECK(reader.closeCurrentFile());
    CHECK(reader.currentFile().empty());
}


TEST_CASE("ADV 5: Loading Irregular file") {
    FileReader reader = FileReader();
    std::string fileName = "data/corrupt.ply";
    CHECK(reader.openFile(fileName) == true);
    REQUIRE(reader.getLines(0, 187).size() > 1);
    CHECK(reader.getLines(0, 187)[188].compare("-0.5 -0.2500001 0.3749999 -1 0 0 80 113 58 255") == 0);
    std::string line = reader.getElementAttribute("vertex", "blue");
    REQUIRE(line.length() == 0);
    line = reader.getElementAttribute("vertex", "red");
    std::string comparison = "97";
    CHECK(line.compare(comparison) == 0);
    CHECK(reader.closeCurrentFile());
}
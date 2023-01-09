//standard lib
#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

//gmaz lib
#include "includes/g_math.h"
#include "includes/g_string.h"
#include "includes/g_log.h"

//classes
//#include "level.h"
#include "classes/level.h"
#include "classes/robot.h"

//constants
#define FILE_READ_ERROR (-1)
#define SUCCESS (0)
#define MEMORY_ALLOC_ERROR (-1)
#define MAX_ROBOTS (4)

//function declarations
int txtToString(std::string fileName, std::string& data);
int levelToMatrix(std::string levelStr, Field** matrix);
int getLevelSize(const std::string levelStr, int& width, int& height);
int getMatrixSize(const std::string levelStr, int& matW, int& matH);
int createMatrix(Field** matrix, int matW, int matH);
void displayLevel(Field** matrix);
void showLevelInfo(Field** matrix);
int addRobotToMatrix(Field** matrix, Robots* robot, int width, int height, int counter/*uses robotNames to get symbol*/); 
//adds 1 robot in the already created matrix and creates a Robot object 
int createRobots(Field** matrix, Robots* robot, int robotNum);
//calling addRobotToMatrix for each robot, manual or random coords

//*-----------------------------------------------------------------------------------
//* when calling matrix cords its matrix[y][x], but we call functions like (..., x, y)
//* we should try fixing this so that is allways x,y  
//*-----------------------------------------------------------------------------------

int main()
{
  std::string data;
  Field** lvlMatrix = nullptr;
  Robots* Robot = nullptr;
  Robot = new Robots[4];
  int matW = 0, matH = 0;
  int check_msg = 0;
  int robotNum = 0;

  txtToString("data/lvl1.txt", data);



  

  getMatrixSize(data, matW, matH);
  // check_msg = createMatrix(lvlMatrix, matW, matH);

  //!this needs to be a seperate function
  //*allocating matrix
  lvlMatrix = new Field*[matH];
  for (int i = 0; i<matH; ++i)
  {
    lvlMatrix[i] = new Field[matW];

    if (lvlMatrix[i] == nullptr)
    {
      check_msg =  MEMORY_ALLOC_ERROR;
    }
  }

  if (lvlMatrix == nullptr)
  {
    check_msg = MEMORY_ALLOC_ERROR;
  }

  if(check_msg != SUCCESS)
  {
    gl::displayMessage("Memory failed to allocate");
    return MEMORY_ALLOC_ERROR;
  }
  //*allocating robot
  while ((robotNum < 1) || (robotNum > MAX_ROBOTS))
  {
    gl::displayMessage("enter number of robots (max 4):");
    std::cin>> robotNum; 
  }
  Robot = new Robots[robotNum];
  gl::displayMessage("");
  //!-------------------

  
  levelToMatrix(data, lvlMatrix);
  gl::displayMessage("Printing the matrix after loading the level into it");
  displayLevel(lvlMatrix);
  

  //*test------------
  // Field obj1('#');
  // Field obj2('A');
  
  // std::cout<< "obj1:" << obj1.getSymbol() << std::endl;
  // std::cout<< "obj2:" << obj2.getSymbol() << std::endl;

  // std::cout<< "swap" << std::endl;
  // obj1.swapObj(obj1, obj2);

  // std::cout<< "obj1:" << obj1.getSymbol() << std::endl;
  // std::cout<< "obj2:" << obj2.getSymbol() << std::endl;
  //*---------------

  createRobots(lvlMatrix, Robot, robotNum);

  gl::displayMessage("Printing the matrix after loading robots in it");
  displayLevel(lvlMatrix);
  showLevelInfo(lvlMatrix);

  return 0;
}

void showLevelInfo(Field** matrix)
{
  gl::displayMessage("Showing level info:");

  int width = Field::matrixWidth;
  int height = Field::matrixHeight;
  char curr_symbol;
  int count = 0;
  int inside_tiles = 0;
  int non_wall_tiles = 0;
  int inside_walls = 0;
  
  for(int j = 0; j < height; j++)
  {
    for(int i = 0; i < width; i++)
    {
      curr_symbol = matrix[j][i].getSymbol();
      for(auto robot : robotNames)
      {
        if(robot == curr_symbol)
        {
          count++;
        }
      }

      if((curr_symbol == '#') && (j != 0 && j != 5) && (i != 0 && i != 9))
      {
        inside_walls ++;
      }
    }
  }

  inside_tiles = (width*height) - (2*width + 2*height) + 4;
  non_wall_tiles = inside_tiles - inside_walls;

  std::cout<<"\t- Matrix size: " << width << " x " << height << std::endl;
  std::cout << "\t- Number of robots in level: " << count << std::endl;
  std::cout << "\t- Number of non-wall tiles: " << non_wall_tiles << std::endl;
  std::cout << std::endl;
}


void displayLevel(Field** matrix)
{
  for(int j = 0; j < Field::matrixHeight; j++)
  {
    for(int i = 0; i <Field::matrixWidth; i++)
    {
      std::cout<< matrix[j][i].getSymbol();
    }
    std::cout<<std::endl;	
  }
}


int levelToMatrix(const std::string levelStr, Field** matrix)
{
  int strW = 0, strH = 0;
  getLevelSize(levelStr, strW, strH);

  
  //inserting chars into matrix
  for(int j = 0; j < strH; j++)
  {
    for(int i = 0; i < strW; i += 2)
    {
      matrix[j][i/2] = Field(levelStr[(strW*j)+i], j, i/2);
    }
  }

  Field::matrixWidth = strW/2;
  Field::matrixHeight = strH;

  return SUCCESS;
}


int createMatrix(Field** matrix, int matW, int matH)
{
  matrix = new Field*[matH];
  for (int i = 0; i<matH; ++i)
  {
    matrix[i] = new Field[matW];

    if (matrix[i] == nullptr)
    {
      return MEMORY_ALLOC_ERROR;
    }
  }

  if (matrix == nullptr){
    return MEMORY_ALLOC_ERROR;
  }

  return SUCCESS;
}


int getMatrixSize(const std::string levelStr, int& matW, int& matH)
{
  int a = 0, b = 0;
  getLevelSize(levelStr, a, b);

  matW = a/2;
  matH = b;

  return SUCCESS;
}


int getLevelSize(const std::string levelStr, int& width, int& height)
{
  width = 0;
  int lenghtOfStr = gs::strLen(levelStr);

  while(levelStr[width] != '\n')
  {
    width++;
  }

  width = width + 1;
  height = lenghtOfStr / width;

  return SUCCESS;
}


int txtToString(std::string fileName, std::string& data)
{
  std::ifstream ip;
  ip.open(fileName);

  if (ip.fail()){
    std::cout<<"Fail failed to open!" << std::endl;
    return FILE_READ_ERROR;
  }

  std::stringstream buffer;
  buffer << ip.rdbuf();

  data = buffer.str();


  ip.close();
  return SUCCESS;
}


int addRobotToMatrix(Field** matrix, Robots* robot, int matW, int matH, int counter) 
{
  if ((matrix == nullptr) || (robot == nullptr)){
    return MEMORY_ALLOC_ERROR;
  }

  char symbol = robotNames[counter]; 

  matrix[matH][matW].createRobot(symbol);

  robot[counter] = Robots(symbol, matW, matH);

  return SUCCESS;
}


int createRobots(Field** matrix, Robots* robot, int robotNum)
{
  if ((matrix == nullptr) || (robot == nullptr)){
    return MEMORY_ALLOC_ERROR;
  }

  int counter = 0; //uses robotNames to get symbol
  int coords_x = 0;
  int coords_y = 0;

  //*here we will give option of manual or random coords
  //for now we will do this manual
  //manual coords

  gl::displayMessage("");

  for (counter=0; counter<robotNum; counter++)
  {
    gl::displayMessageChar("ROBOT ", robotNames[counter]);

    do
    {
      gl::displayMessage("enter x and y coords: ");
      std::cin >> coords_x;
      std::cin >> coords_y;

      if ((coords_x < 0) || (coords_y < 0))
      {
        gl::displayMessage("coordinates can't be neagtive!");
        continue;
      }
      else if((coords_x > Field::matrixWidth-1) || (coords_y > Field::matrixHeight-1))
      {
        gl::displayMessage("coordinates are outise of the matrix!");
        gl::displayMessageInt("keep width under ", Field::matrixWidth);
        gl::displayMessageInt("keep height under ", Field::matrixHeight);
        continue;
      }
      else if(!(matrix[coords_y][coords_x].getIsEmpty()))
      {
        gl::displayMessage("coordinates are not empty!");
        continue;
      }

      break;
      
    } while(1);
    
    gl::displayMessage("");
    addRobotToMatrix(matrix, robot, coords_x, coords_y, counter);
  }

  return SUCCESS;
}

//TODO
//
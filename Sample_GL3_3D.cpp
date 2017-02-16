#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#define NUMBER_OF_BLOCKS 20
#define WIDTH 800
#define HEIGHT 800
#define ORTHO_X 9.0f
#define ORTHO_Y 9.0f
#define BLOCK_WIDTH 1
#define BOARD_HEIGHT 0.2
#define NUMBER_OF_LEVELS 8
#define SCORE_WIDTH 0.1
#define NUMBER_OF_VIEWS 5

struct COLOR {
    float r;
    float g;
    float b;
};
typedef struct COLOR COLOR;

COLOR silver = {192/255.0, 192/255.0, 192/255.0};
COLOR red = {255.0/255.0,51.0/255.0,51.0/255.0};
COLOR blue = {0,0,1};
COLOR grey = {168.0/255.0,168.0/255.0,168.0/255.0};
COLOR white = {255/255.0,255/255.0,255/255.0};
COLOR gold = {218.0/255.0,165.0/255.0,32.0/255.0};
COLOR black = {30/255.0,30/255.0,21/255.0};
COLOR green = {0.0/255.0, 255.0/255.0, 0.0/255.0};
COLOR darkgreen = {0.0/255.0, 153.0/255.0, 51.0/255.0};
COLOR orange = {255.0/255.0, 102.0/255.0, 0.0/255.0};

struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
  glm::mat4 projection;
  glm::mat4 model;
  glm::mat4 view;
  GLuint MatrixID;
} Matrices;

GLuint programID;

GLFWwindow* window;

int level0[] = {
    0*NUMBER_OF_BLOCKS + 3,  0*NUMBER_OF_BLOCKS + 4, 0*NUMBER_OF_BLOCKS + 5,
    1*NUMBER_OF_BLOCKS + 3,  1*NUMBER_OF_BLOCKS + 4, 1*NUMBER_OF_BLOCKS + 5, 1*NUMBER_OF_BLOCKS + 6,
    2*NUMBER_OF_BLOCKS + 3,  2*NUMBER_OF_BLOCKS + 4, 2*NUMBER_OF_BLOCKS + 5, 2*NUMBER_OF_BLOCKS + 6,
                             3*NUMBER_OF_BLOCKS + 4, 3*NUMBER_OF_BLOCKS + 5, 3*NUMBER_OF_BLOCKS + 6,
                             4*NUMBER_OF_BLOCKS + 4, 4*NUMBER_OF_BLOCKS + 5, 4*NUMBER_OF_BLOCKS + 6,
                             5*NUMBER_OF_BLOCKS + 4, 5*NUMBER_OF_BLOCKS + 5, 5*NUMBER_OF_BLOCKS + 6, 5*NUMBER_OF_BLOCKS + 7,
                                                     6*NUMBER_OF_BLOCKS + 5, 6*NUMBER_OF_BLOCKS + 6, 6*NUMBER_OF_BLOCKS + 7, 6*NUMBER_OF_BLOCKS + 8,
                                                     7*NUMBER_OF_BLOCKS + 5, 7*NUMBER_OF_BLOCKS + 6, 7*NUMBER_OF_BLOCKS + 7, 7*NUMBER_OF_BLOCKS + 8,
                                                     8*NUMBER_OF_BLOCKS + 5, 8*NUMBER_OF_BLOCKS + 6, 8*NUMBER_OF_BLOCKS + 7, 8*NUMBER_OF_BLOCKS + 8,
                                                                             6*NUMBER_OF_BLOCKS + 6, 6*NUMBER_OF_BLOCKS + 7,
};

int level1[] = {
                             0*NUMBER_OF_BLOCKS + 3,   0*NUMBER_OF_BLOCKS + 4,  0*NUMBER_OF_BLOCKS + 5,   0*NUMBER_OF_BLOCKS + 6,  0*NUMBER_OF_BLOCKS + 7,
                             1*NUMBER_OF_BLOCKS + 3,   1*NUMBER_OF_BLOCKS + 4,  1*NUMBER_OF_BLOCKS + 5,   1*NUMBER_OF_BLOCKS + 6,  1*NUMBER_OF_BLOCKS + 7,
                             2*NUMBER_OF_BLOCKS + 3,   2*NUMBER_OF_BLOCKS + 4,  2*NUMBER_OF_BLOCKS + 5,   2*NUMBER_OF_BLOCKS + 6,  2*NUMBER_OF_BLOCKS + 7,
                             3*NUMBER_OF_BLOCKS + 3,   3*NUMBER_OF_BLOCKS + 4,  3*NUMBER_OF_BLOCKS + 5,   3*NUMBER_OF_BLOCKS + 6,  3*NUMBER_OF_BLOCKS + 7,
                                                                                                          4*NUMBER_OF_BLOCKS + 6,
                                                                                                          5*NUMBER_OF_BLOCKS + 6,
   6*NUMBER_OF_BLOCKS + 2,   6*NUMBER_OF_BLOCKS + 3,   6*NUMBER_OF_BLOCKS + 4,  6*NUMBER_OF_BLOCKS + 5,   6*NUMBER_OF_BLOCKS + 6,  6*NUMBER_OF_BLOCKS + 7,
   7*NUMBER_OF_BLOCKS + 2,   7*NUMBER_OF_BLOCKS + 3,   7*NUMBER_OF_BLOCKS + 4,  7*NUMBER_OF_BLOCKS + 5,   7*NUMBER_OF_BLOCKS + 6,  7*NUMBER_OF_BLOCKS + 7,
   8*NUMBER_OF_BLOCKS + 2,   8*NUMBER_OF_BLOCKS + 3,   8*NUMBER_OF_BLOCKS + 4,  8*NUMBER_OF_BLOCKS + 5,   8*NUMBER_OF_BLOCKS + 6,  8*NUMBER_OF_BLOCKS + 7,
   9*NUMBER_OF_BLOCKS + 2,   9*NUMBER_OF_BLOCKS + 3,   9*NUMBER_OF_BLOCKS + 4,  9*NUMBER_OF_BLOCKS + 5,   9*NUMBER_OF_BLOCKS + 6,  9*NUMBER_OF_BLOCKS + 7,
                                                                                                         10*NUMBER_OF_BLOCKS + 6,
                                                                                                         11*NUMBER_OF_BLOCKS + 6,
  12*NUMBER_OF_BLOCKS + 2,  12*NUMBER_OF_BLOCKS + 3,  12*NUMBER_OF_BLOCKS + 4, 12*NUMBER_OF_BLOCKS + 5,  12*NUMBER_OF_BLOCKS + 6,
  13*NUMBER_OF_BLOCKS + 2,  13*NUMBER_OF_BLOCKS + 3,  13*NUMBER_OF_BLOCKS + 4, 13*NUMBER_OF_BLOCKS + 5,  13*NUMBER_OF_BLOCKS + 6,
  14*NUMBER_OF_BLOCKS + 2,  14*NUMBER_OF_BLOCKS + 3,  14*NUMBER_OF_BLOCKS + 4, 14*NUMBER_OF_BLOCKS + 5,  14*NUMBER_OF_BLOCKS + 6,
};

int level2[] = {
                             0*NUMBER_OF_BLOCKS + 3,  0*NUMBER_OF_BLOCKS + 4,  0*NUMBER_OF_BLOCKS + 5,   0*NUMBER_OF_BLOCKS + 6,
                             1*NUMBER_OF_BLOCKS + 3,  1*NUMBER_OF_BLOCKS + 4,  1*NUMBER_OF_BLOCKS + 5,   1*NUMBER_OF_BLOCKS + 6,
                             2*NUMBER_OF_BLOCKS + 3,  2*NUMBER_OF_BLOCKS + 4,  2*NUMBER_OF_BLOCKS + 5,   2*NUMBER_OF_BLOCKS + 6,
                             3*NUMBER_OF_BLOCKS + 3,  3*NUMBER_OF_BLOCKS + 4,  3*NUMBER_OF_BLOCKS + 5,   3*NUMBER_OF_BLOCKS + 6,
                                                      4*NUMBER_OF_BLOCKS + 4,
                                                      5*NUMBER_OF_BLOCKS + 4,
   6*NUMBER_OF_BLOCKS + 2,   6*NUMBER_OF_BLOCKS + 3,  6*NUMBER_OF_BLOCKS + 4,
   7*NUMBER_OF_BLOCKS + 2,   7*NUMBER_OF_BLOCKS + 3,  7*NUMBER_OF_BLOCKS + 4,
   8*NUMBER_OF_BLOCKS + 2,   8*NUMBER_OF_BLOCKS + 3,  8*NUMBER_OF_BLOCKS + 4,
   9*NUMBER_OF_BLOCKS + 2,
  10*NUMBER_OF_BLOCKS + 2,
  11*NUMBER_OF_BLOCKS + 2,  11*NUMBER_OF_BLOCKS + 3,  11*NUMBER_OF_BLOCKS + 4, 11*NUMBER_OF_BLOCKS + 5,  11*NUMBER_OF_BLOCKS + 6,
  12*NUMBER_OF_BLOCKS + 2,  12*NUMBER_OF_BLOCKS + 3,  12*NUMBER_OF_BLOCKS + 4, 12*NUMBER_OF_BLOCKS + 5,  12*NUMBER_OF_BLOCKS + 6,  12*NUMBER_OF_BLOCKS + 7,
                                                      13*NUMBER_OF_BLOCKS + 4, 13*NUMBER_OF_BLOCKS + 5,  13*NUMBER_OF_BLOCKS + 6,  13*NUMBER_OF_BLOCKS + 7,
                                                      14*NUMBER_OF_BLOCKS + 4, 14*NUMBER_OF_BLOCKS + 5,  14*NUMBER_OF_BLOCKS + 6,  14*NUMBER_OF_BLOCKS + 7,
};

int level3[] = {
                                                    0*NUMBER_OF_BLOCKS + 3,  0*NUMBER_OF_BLOCKS + 4,  0*NUMBER_OF_BLOCKS + 5,  0*NUMBER_OF_BLOCKS + 6,  0*NUMBER_OF_BLOCKS + 7,
                                                    1*NUMBER_OF_BLOCKS + 3,  1*NUMBER_OF_BLOCKS + 4,  1*NUMBER_OF_BLOCKS + 5,  1*NUMBER_OF_BLOCKS + 6,  1*NUMBER_OF_BLOCKS + 7,
                                                    2*NUMBER_OF_BLOCKS + 3,  2*NUMBER_OF_BLOCKS + 4,  2*NUMBER_OF_BLOCKS + 5,  2*NUMBER_OF_BLOCKS + 6,  2*NUMBER_OF_BLOCKS + 7,
  3*NUMBER_OF_BLOCKS + 1,  3*NUMBER_OF_BLOCKS + 2,  3*NUMBER_OF_BLOCKS + 3,
  4*NUMBER_OF_BLOCKS + 1,  4*NUMBER_OF_BLOCKS + 2,
  5*NUMBER_OF_BLOCKS + 1,  5*NUMBER_OF_BLOCKS + 2,                                                                             5*NUMBER_OF_BLOCKS + 6,  5*NUMBER_OF_BLOCKS + 7,  5*NUMBER_OF_BLOCKS + 8,  5*NUMBER_OF_BLOCKS + 9,
  6*NUMBER_OF_BLOCKS + 1,  6*NUMBER_OF_BLOCKS + 2,                                                                             6*NUMBER_OF_BLOCKS + 6,  6*NUMBER_OF_BLOCKS + 7,  6*NUMBER_OF_BLOCKS + 8,  6*NUMBER_OF_BLOCKS + 9,
  7*NUMBER_OF_BLOCKS + 1,  7*NUMBER_OF_BLOCKS + 2,                                                                             7*NUMBER_OF_BLOCKS + 6,  7*NUMBER_OF_BLOCKS + 7,  7*NUMBER_OF_BLOCKS + 8,  7*NUMBER_OF_BLOCKS + 9,
  8*NUMBER_OF_BLOCKS + 1,  8*NUMBER_OF_BLOCKS + 2,                                                                             8*NUMBER_OF_BLOCKS + 6,  8*NUMBER_OF_BLOCKS + 7,
  9*NUMBER_OF_BLOCKS + 1,  9*NUMBER_OF_BLOCKS + 2,  9*NUMBER_OF_BLOCKS + 3,                                                    9*NUMBER_OF_BLOCKS + 6,  9*NUMBER_OF_BLOCKS + 7,
                                                   10*NUMBER_OF_BLOCKS + 3, 10*NUMBER_OF_BLOCKS + 4, 10*NUMBER_OF_BLOCKS + 5, 10*NUMBER_OF_BLOCKS + 6, 10*NUMBER_OF_BLOCKS + 7, 10*NUMBER_OF_BLOCKS + 8, 10*NUMBER_OF_BLOCKS + 9,
                                                   11*NUMBER_OF_BLOCKS + 3, 11*NUMBER_OF_BLOCKS + 4, 11*NUMBER_OF_BLOCKS + 5, 11*NUMBER_OF_BLOCKS + 6, 11*NUMBER_OF_BLOCKS + 7, 11*NUMBER_OF_BLOCKS + 8, 11*NUMBER_OF_BLOCKS + 9,
                                                                                                                              12*NUMBER_OF_BLOCKS + 6, 12*NUMBER_OF_BLOCKS + 7, 12*NUMBER_OF_BLOCKS + 8, 12*NUMBER_OF_BLOCKS + 9,
                                                                                                                              13*NUMBER_OF_BLOCKS + 6, 13*NUMBER_OF_BLOCKS + 7, 13*NUMBER_OF_BLOCKS + 8, 13*NUMBER_OF_BLOCKS + 9,
};

int level4[] = {
                                                                                                                                                                                0*NUMBER_OF_BLOCKS + 9,  0*NUMBER_OF_BLOCKS + 10,  0*NUMBER_OF_BLOCKS + 11,
                           1*NUMBER_OF_BLOCKS + 3,  1*NUMBER_OF_BLOCKS + 4, 1*NUMBER_OF_BLOCKS + 5,  1*NUMBER_OF_BLOCKS + 6,                                                    1*NUMBER_OF_BLOCKS + 9,  1*NUMBER_OF_BLOCKS + 10,  1*NUMBER_OF_BLOCKS + 11,
                           2*NUMBER_OF_BLOCKS + 3,  2*NUMBER_OF_BLOCKS + 4, 2*NUMBER_OF_BLOCKS + 5,  2*NUMBER_OF_BLOCKS + 6,                                                    2*NUMBER_OF_BLOCKS + 9,  2*NUMBER_OF_BLOCKS + 10,  2*NUMBER_OF_BLOCKS + 11,
                           3*NUMBER_OF_BLOCKS + 3,  3*NUMBER_OF_BLOCKS + 4, 3*NUMBER_OF_BLOCKS + 5,  3*NUMBER_OF_BLOCKS + 6,  3*NUMBER_OF_BLOCKS + 7,                                                    3*NUMBER_OF_BLOCKS + 10,  3*NUMBER_OF_BLOCKS + 11,
                           4*NUMBER_OF_BLOCKS + 3,  4*NUMBER_OF_BLOCKS + 4, 4*NUMBER_OF_BLOCKS + 5,  4*NUMBER_OF_BLOCKS + 6,  4*NUMBER_OF_BLOCKS + 7,                                                    4*NUMBER_OF_BLOCKS + 10,
                           5*NUMBER_OF_BLOCKS + 3,                                                                            5*NUMBER_OF_BLOCKS + 7,                                                    5*NUMBER_OF_BLOCKS + 10,
                           6*NUMBER_OF_BLOCKS + 3,                                                                            6*NUMBER_OF_BLOCKS + 7,                                                    6*NUMBER_OF_BLOCKS + 10,
                           7*NUMBER_OF_BLOCKS + 3,                                                                            7*NUMBER_OF_BLOCKS + 7,                                                    7*NUMBER_OF_BLOCKS + 10,
                           8*NUMBER_OF_BLOCKS + 3,                                                                            8*NUMBER_OF_BLOCKS + 7,                                                    8*NUMBER_OF_BLOCKS + 10,
                           9*NUMBER_OF_BLOCKS + 3,                                                                            9*NUMBER_OF_BLOCKS + 7,                                                    9*NUMBER_OF_BLOCKS + 10,
                          10*NUMBER_OF_BLOCKS + 3,                                                                           10*NUMBER_OF_BLOCKS + 7, 10*NUMBER_OF_BLOCKS + 8, 10*NUMBER_OF_BLOCKS + 9, 10*NUMBER_OF_BLOCKS + 10,
 11*NUMBER_OF_BLOCKS + 2, 11*NUMBER_OF_BLOCKS + 3,                                                                           11*NUMBER_OF_BLOCKS + 7, 11*NUMBER_OF_BLOCKS + 8, 11*NUMBER_OF_BLOCKS + 9, 11*NUMBER_OF_BLOCKS + 10,
 12*NUMBER_OF_BLOCKS + 2, 12*NUMBER_OF_BLOCKS + 3, 12*NUMBER_OF_BLOCKS + 4,                                                  12*NUMBER_OF_BLOCKS + 7, 12*NUMBER_OF_BLOCKS + 8, 12*NUMBER_OF_BLOCKS + 9, 12*NUMBER_OF_BLOCKS + 10,
 13*NUMBER_OF_BLOCKS + 2, 13*NUMBER_OF_BLOCKS + 3, 13*NUMBER_OF_BLOCKS + 4,                                                                           13*NUMBER_OF_BLOCKS + 8, 13*NUMBER_OF_BLOCKS + 9,
 14*NUMBER_OF_BLOCKS + 2, 14*NUMBER_OF_BLOCKS + 3, 14*NUMBER_OF_BLOCKS + 4,                                                                           14*NUMBER_OF_BLOCKS + 8, 14*NUMBER_OF_BLOCKS + 9,
};

int level5[] = {
                                                                             0*NUMBER_OF_BLOCKS + 6,
                                                                             1*NUMBER_OF_BLOCKS + 6,
                                                                             2*NUMBER_OF_BLOCKS + 6,
                                                                             3*NUMBER_OF_BLOCKS + 6,
                                                                             4*NUMBER_OF_BLOCKS + 6,  4*NUMBER_OF_BLOCKS + 7,  4*NUMBER_OF_BLOCKS + 8,
  5*NUMBER_OF_BLOCKS + 3,  5*NUMBER_OF_BLOCKS + 4,  5*NUMBER_OF_BLOCKS + 5,  5*NUMBER_OF_BLOCKS + 6,  5*NUMBER_OF_BLOCKS + 7,  5*NUMBER_OF_BLOCKS + 8,
  6*NUMBER_OF_BLOCKS + 3,                                                                             6*NUMBER_OF_BLOCKS + 7,  6*NUMBER_OF_BLOCKS + 8, 6*NUMBER_OF_BLOCKS + 9,  6*NUMBER_OF_BLOCKS + 10,  6*NUMBER_OF_BLOCKS + 11,
  7*NUMBER_OF_BLOCKS + 3,                                                                                                                                                       7*NUMBER_OF_BLOCKS + 10,  7*NUMBER_OF_BLOCKS + 11,  7*NUMBER_OF_BLOCKS + 12,
  8*NUMBER_OF_BLOCKS + 3,  8*NUMBER_OF_BLOCKS + 4,  8*NUMBER_OF_BLOCKS + 5,                                                                                                     8*NUMBER_OF_BLOCKS + 10,  8*NUMBER_OF_BLOCKS + 11,  8*NUMBER_OF_BLOCKS + 12,
  9*NUMBER_OF_BLOCKS + 3,  9*NUMBER_OF_BLOCKS + 4,  9*NUMBER_OF_BLOCKS + 5,                                                                            9*NUMBER_OF_BLOCKS + 9,  9*NUMBER_OF_BLOCKS + 10,  9*NUMBER_OF_BLOCKS + 11,  9*NUMBER_OF_BLOCKS + 12,
 10*NUMBER_OF_BLOCKS + 3, 10*NUMBER_OF_BLOCKS + 4, 10*NUMBER_OF_BLOCKS + 5,                                                                           10*NUMBER_OF_BLOCKS + 9, 10*NUMBER_OF_BLOCKS + 10, 10*NUMBER_OF_BLOCKS + 11,
                                                   11*NUMBER_OF_BLOCKS + 5, 11*NUMBER_OF_BLOCKS + 6, 11*NUMBER_OF_BLOCKS + 7,
                                                   12*NUMBER_OF_BLOCKS + 5, 12*NUMBER_OF_BLOCKS + 6, 12*NUMBER_OF_BLOCKS + 7, 12*NUMBER_OF_BLOCKS + 8,
                                                                            13*NUMBER_OF_BLOCKS + 6, 13*NUMBER_OF_BLOCKS + 7, 13*NUMBER_OF_BLOCKS + 8,
                                                                            14*NUMBER_OF_BLOCKS + 6, 14*NUMBER_OF_BLOCKS + 7, 14*NUMBER_OF_BLOCKS + 8,
};

int level6[] = {
  0*NUMBER_OF_BLOCKS + 3,  0*NUMBER_OF_BLOCKS + 4,  0*NUMBER_OF_BLOCKS + 5,
  1*NUMBER_OF_BLOCKS + 3,  1*NUMBER_OF_BLOCKS + 4,  1*NUMBER_OF_BLOCKS + 5,
  2*NUMBER_OF_BLOCKS + 3,  2*NUMBER_OF_BLOCKS + 4,  2*NUMBER_OF_BLOCKS + 5,
  3*NUMBER_OF_BLOCKS + 3,  3*NUMBER_OF_BLOCKS + 4,  3*NUMBER_OF_BLOCKS + 5,
                                                    4*NUMBER_OF_BLOCKS + 5,
                                                    5*NUMBER_OF_BLOCKS + 5,
                                                    6*NUMBER_OF_BLOCKS + 5,  6*NUMBER_OF_BLOCKS + 6,  6*NUMBER_OF_BLOCKS + 7,
  7*NUMBER_OF_BLOCKS + 3,  7*NUMBER_OF_BLOCKS + 4,  7*NUMBER_OF_BLOCKS + 5,  7*NUMBER_OF_BLOCKS + 6,  7*NUMBER_OF_BLOCKS + 7,
                                                    8*NUMBER_OF_BLOCKS + 5,  8*NUMBER_OF_BLOCKS + 6,  8*NUMBER_OF_BLOCKS + 7,
                                                    9*NUMBER_OF_BLOCKS + 5,
                                                   10*NUMBER_OF_BLOCKS + 5,
 11*NUMBER_OF_BLOCKS + 3, 11*NUMBER_OF_BLOCKS + 4, 11*NUMBER_OF_BLOCKS + 5,
 12*NUMBER_OF_BLOCKS + 3, 12*NUMBER_OF_BLOCKS + 4, 12*NUMBER_OF_BLOCKS + 5,
 13*NUMBER_OF_BLOCKS + 3, 13*NUMBER_OF_BLOCKS + 4, 13*NUMBER_OF_BLOCKS + 5,
 14*NUMBER_OF_BLOCKS + 3, 14*NUMBER_OF_BLOCKS + 4, 14*NUMBER_OF_BLOCKS + 5,
};

int level7[] = {
  0*NUMBER_OF_BLOCKS + 3,  0*NUMBER_OF_BLOCKS + 4,  0*NUMBER_OF_BLOCKS + 5,
  1*NUMBER_OF_BLOCKS + 3,  1*NUMBER_OF_BLOCKS + 4,  1*NUMBER_OF_BLOCKS + 5,
  2*NUMBER_OF_BLOCKS + 3,  2*NUMBER_OF_BLOCKS + 4,  2*NUMBER_OF_BLOCKS + 5,

                                                                                                                                                                                                           4*NUMBER_OF_BLOCKS + 11,  4*NUMBER_OF_BLOCKS + 12,
                           5*NUMBER_OF_BLOCKS + 4,                                                                                                                                                         5*NUMBER_OF_BLOCKS + 11,  5*NUMBER_OF_BLOCKS + 12,
                                                                                                                                                                                                           6*NUMBER_OF_BLOCKS + 11,
                                                                                                                                                                                                           7*NUMBER_OF_BLOCKS + 11,
  8*NUMBER_OF_BLOCKS + 3,  8*NUMBER_OF_BLOCKS + 4,  8*NUMBER_OF_BLOCKS + 5,                                                                                                                                8*NUMBER_OF_BLOCKS + 11,  8*NUMBER_OF_BLOCKS + 12,
  9*NUMBER_OF_BLOCKS + 3,  9*NUMBER_OF_BLOCKS + 4,  9*NUMBER_OF_BLOCKS + 5,  9*NUMBER_OF_BLOCKS + 6,                                                                                                                                 9*NUMBER_OF_BLOCKS + 12,
 10*NUMBER_OF_BLOCKS + 3, 10*NUMBER_OF_BLOCKS + 4, 10*NUMBER_OF_BLOCKS + 5, 10*NUMBER_OF_BLOCKS + 6,                                                                                                                                10*NUMBER_OF_BLOCKS + 12,
 11*NUMBER_OF_BLOCKS + 3, 11*NUMBER_OF_BLOCKS + 4, 11*NUMBER_OF_BLOCKS + 5, 11*NUMBER_OF_BLOCKS + 6, 11*NUMBER_OF_BLOCKS + 7,                                                   11*NUMBER_OF_BLOCKS + 10, 11*NUMBER_OF_BLOCKS + 11, 11*NUMBER_OF_BLOCKS + 12,
 12*NUMBER_OF_BLOCKS + 3, 12*NUMBER_OF_BLOCKS + 4,                                                   12*NUMBER_OF_BLOCKS + 7, 12*NUMBER_OF_BLOCKS + 8, 12*NUMBER_OF_BLOCKS + 9, 12*NUMBER_OF_BLOCKS + 10, 12*NUMBER_OF_BLOCKS + 11, 12*NUMBER_OF_BLOCKS + 12,
 13*NUMBER_OF_BLOCKS + 3, 13*NUMBER_OF_BLOCKS + 4,
};

int fragile_blocks[] = {
    3*NUMBER_OF_BLOCKS + 1,  3*NUMBER_OF_BLOCKS + 2,
    4*NUMBER_OF_BLOCKS + 1,  4*NUMBER_OF_BLOCKS + 2,
    5*NUMBER_OF_BLOCKS + 1,  5*NUMBER_OF_BLOCKS + 2,
    6*NUMBER_OF_BLOCKS + 1,  6*NUMBER_OF_BLOCKS + 2,
    7*NUMBER_OF_BLOCKS + 1,  7*NUMBER_OF_BLOCKS + 2,
    8*NUMBER_OF_BLOCKS + 1,  8*NUMBER_OF_BLOCKS + 2,                                                                             8*NUMBER_OF_BLOCKS + 6,  8*NUMBER_OF_BLOCKS + 7,
    9*NUMBER_OF_BLOCKS + 1,  9*NUMBER_OF_BLOCKS + 2,                                                                             9*NUMBER_OF_BLOCKS + 6,  9*NUMBER_OF_BLOCKS + 7,
                                                                                                                                10*NUMBER_OF_BLOCKS + 6, 10*NUMBER_OF_BLOCKS + 7, 10*NUMBER_OF_BLOCKS + 8, 10*NUMBER_OF_BLOCKS + 9,
                                                                                                                                11*NUMBER_OF_BLOCKS + 6, 11*NUMBER_OF_BLOCKS + 7, 11*NUMBER_OF_BLOCKS + 8, 11*NUMBER_OF_BLOCKS + 9,
                                                                                                                                12*NUMBER_OF_BLOCKS + 6, 12*NUMBER_OF_BLOCKS + 7,                          12*NUMBER_OF_BLOCKS + 9,
                                                                                                                                13*NUMBER_OF_BLOCKS + 6, 13*NUMBER_OF_BLOCKS + 7, 13*NUMBER_OF_BLOCKS + 8, 13*NUMBER_OF_BLOCKS + 9,
};

int level_sizes[] = {
  sizeof(level0)/sizeof(level0[0]), //level0
  sizeof(level1)/sizeof(level1[0]), //level1
  sizeof(level2)/sizeof(level2[0]), //level2
  sizeof(level3)/sizeof(level3[0]), //level3
  sizeof(level4)/sizeof(level4[0]), //level4
  sizeof(level5)/sizeof(level5[0]), //level5
  sizeof(level6)/sizeof(level6[0]), //level6
  sizeof(level7)/sizeof(level7[0]), //level7
};

int* levels[] = {
  level0,
  level1,
  level2,
  level3,
  level4,
  level5,
  level6,
  level7,
};

int block_positions[] = {
  1 * NUMBER_OF_BLOCKS + 4, //level 0
  1 * NUMBER_OF_BLOCKS + 6, //level 1
  1 * NUMBER_OF_BLOCKS + 5, //level 2
  1 * NUMBER_OF_BLOCKS + 6, //level 3
 13 * NUMBER_OF_BLOCKS + 3, //level 4
  0 * NUMBER_OF_BLOCKS + 6, //level 5
  1 * NUMBER_OF_BLOCKS + 4, //level 6
  9 * NUMBER_OF_BLOCKS + 4, //level 7
};

int goal_positions[] = {
   7 * NUMBER_OF_BLOCKS +  7, //level 0
  13 * NUMBER_OF_BLOCKS +  3, //level 1
  13 * NUMBER_OF_BLOCKS +  5, //level 2
   6 * NUMBER_OF_BLOCKS +  8, //level 3
   1 * NUMBER_OF_BLOCKS + 10, //level 4
  13 * NUMBER_OF_BLOCKS +  7, //level 5
   7 * NUMBER_OF_BLOCKS +  6, //level 6
   1 * NUMBER_OF_BLOCKS +  4, //level 7
};

struct BRIDGE{
  int position;
  int bridge_position1;
  int bridge_position2;
};
typedef struct BRIDGE BRIDGE;

struct SPLIT{
  int position;
  int split_position1;
  int split_position2;
};

typedef struct SPLIT SPLIT;

BRIDGE b1 = { 2 * NUMBER_OF_BLOCKS +  4,  4 * NUMBER_OF_BLOCKS +  6,  5 * NUMBER_OF_BLOCKS +  6};
BRIDGE b2 = { 8 * NUMBER_OF_BLOCKS +  3, 10 * NUMBER_OF_BLOCKS +  6, 11 * NUMBER_OF_BLOCKS +  6};
BRIDGE b3 = { 8 * NUMBER_OF_BLOCKS +  3,  5 * NUMBER_OF_BLOCKS +  3,  6 * NUMBER_OF_BLOCKS +  3};
BRIDGE b4 = {14 * NUMBER_OF_BLOCKS +  8,  6 * NUMBER_OF_BLOCKS + 10,  7 * NUMBER_OF_BLOCKS + 10};
BRIDGE b5 = { 5 * NUMBER_OF_BLOCKS + 12,  3 * NUMBER_OF_BLOCKS +  4,  4 * NUMBER_OF_BLOCKS +  4};
BRIDGE b6 = {11 * NUMBER_OF_BLOCKS + 12, 12 * NUMBER_OF_BLOCKS +  5, 12 * NUMBER_OF_BLOCKS +  6};
BRIDGE b7 = {11 * NUMBER_OF_BLOCKS + 12,  6 * NUMBER_OF_BLOCKS +  4,  7 * NUMBER_OF_BLOCKS +  4};

BRIDGE H0[] = {};
BRIDGE H1[] = {b1};
BRIDGE H2[] = {};
BRIDGE H3[] = {};
BRIDGE H4[] = {b3,b4};
BRIDGE H5[] = {};
BRIDGE H6[] = {};
BRIDGE H7[] = {b5};

BRIDGE V0[] = {};
BRIDGE V1[] = {b2};
BRIDGE V2[] = {};
BRIDGE V3[] = {};
BRIDGE V4[] = {};
BRIDGE V5[] = {};
BRIDGE V6[] = {};
BRIDGE V7[] = {b6,b7};

BRIDGE* horizontal_bridges[] = {
  H0, H1, H2, H3, H4, H5, H6, H7,
};

BRIDGE* vertical_bridges[] = {
  V0, V1, V2, V3, V4, V5, V6, V7,
};

SPLIT s1 = { 13 * NUMBER_OF_BLOCKS + 4, 12 * NUMBER_OF_BLOCKS + 4, 2 * NUMBER_OF_BLOCKS + 4 };
SPLIT s2 = { 12 * NUMBER_OF_BLOCKS + 4, 12 * NUMBER_OF_BLOCKS + 4, 9 * NUMBER_OF_BLOCKS + 4 };

SPLIT S0[] = {};
SPLIT S1[] = {};
SPLIT S2[] = {};;
SPLIT S3[] = {};
SPLIT S4[] = {};
SPLIT S5[] = {};
SPLIT S6[] = {s1};
SPLIT S7[] = {s2};

SPLIT* splits[] = {
  S0, S1, S2, S3, S4, S5, S6, S7,
};

int horizontal_bridge_sizes[] = {0, 1, 0, 0, 2, 0, 0, 1};
int vertical_bridge_sizes[] = {0, 1, 0, 1, 0, 0, 0, 2};
int split_sizes[] = {0, 0, 0, 0, 0, 0, 1, 1};
int fragile_blocks_sizes[] = {0, 0, 0, sizeof(fragile_blocks)/sizeof(fragile_blocks[0]), 0, 0, 0, 0};


/* Function to load Shaders - Use it as it is */
GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path) {

  // Create the shaders
  GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

  // Read the Vertex Shader code from the file
  std::string VertexShaderCode;
  std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
  if(VertexShaderStream.is_open())
  {
    std::string Line = "";
    while(getline(VertexShaderStream, Line))
      VertexShaderCode += "\n" + Line;
    VertexShaderStream.close();
  }

  // Read the Fragment Shader code from the file
  std::string FragmentShaderCode;
  std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
  if(FragmentShaderStream.is_open()){
    std::string Line = "";
    while(getline(FragmentShaderStream, Line))
      FragmentShaderCode += "\n" + Line;
    FragmentShaderStream.close();
  }

  GLint Result = GL_FALSE;
  int InfoLogLength;

  // Compile Vertex Shader
  printf("Compiling shader : %s\n", vertex_file_path);
  char const * VertexSourcePointer = VertexShaderCode.c_str();
  glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
  glCompileShader(VertexShaderID);

  // Check Vertex Shader
  glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> VertexShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &VertexShaderErrorMessage[0]);

  // Compile Fragment Shader
  printf("Compiling shader : %s\n", fragment_file_path);
  char const * FragmentSourcePointer = FragmentShaderCode.c_str();
  glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
  glCompileShader(FragmentShaderID);

  // Check Fragment Shader
  glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
  glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
  glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
  fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);

  // Link the program
  fprintf(stdout, "Linking program\n");
  GLuint ProgramID = glCreateProgram();
  glAttachShader(ProgramID, VertexShaderID);
  glAttachShader(ProgramID, FragmentShaderID);
  glLinkProgram(ProgramID);

  // Check the program
  glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
  glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
  std::vector<char> ProgramErrorMessage( max(InfoLogLength, int(1)) );
  glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
  fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);

  return ProgramID;
}

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void quit(GLFWwindow *window)
{
    glfwDestroyWindow(window);
    glfwTerminate();
    //    exit(EXIT_SUCCESS);
}


/* Generate VAO, VBOs and return VAO handle */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat* color_buffer_data, GLenum fill_mode=GL_FILL)
{
    struct VAO* vao = new struct VAO;
    vao->PrimitiveMode = primitive_mode;
    vao->NumVertices = numVertices;
    vao->FillMode = fill_mode;

    // Create Vertex Array Object
    // Should be done after CreateWindow and before any other GL calls
    glGenVertexArrays(1, &(vao->VertexArrayID)); // VAO
    glGenBuffers (1, &(vao->VertexBuffer)); // VBO - vertices
    glGenBuffers (1, &(vao->ColorBuffer));  // VBO - colors

    glBindVertexArray (vao->VertexArrayID); // Bind the VAO
    glBindBuffer (GL_ARRAY_BUFFER, vao->VertexBuffer); // Bind the VBO vertices
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), vertex_buffer_data, GL_STATIC_DRAW); // Copy the vertices into VBO
    glVertexAttribPointer(
                          0,                  // attribute 0. Vertices
                          3,                  // size (x,y,z)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    glBindBuffer (GL_ARRAY_BUFFER, vao->ColorBuffer); // Bind the VBO colors
    glBufferData (GL_ARRAY_BUFFER, 3*numVertices*sizeof(GLfloat), color_buffer_data, GL_STATIC_DRAW);  // Copy the vertex colors
    glVertexAttribPointer(
                          1,                  // attribute 1. Color
                          3,                  // size (r,g,b)
                          GL_FLOAT,           // type
                          GL_FALSE,           // normalized?
                          0,                  // stride
                          (void*)0            // array buffer offset
                          );

    return vao;
}

/* Generate VAO, VBOs and return VAO handle - Common Color for all vertices */
struct VAO* create3DObject (GLenum primitive_mode, int numVertices, const GLfloat* vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode=GL_FILL)
{
    GLfloat* color_buffer_data = new GLfloat [3*numVertices];
    for (int i=0; i<numVertices; i++) {
        color_buffer_data [3*i] = red;
        color_buffer_data [3*i + 1] = green;
        color_buffer_data [3*i + 2] = blue;
    }

    return create3DObject(primitive_mode, numVertices, vertex_buffer_data, color_buffer_data, fill_mode);
}

/* Render the VBOs handled by VAO */
void draw3DObject (struct VAO* vao)
{
    // Change the Fill Mode for this object
    glPolygonMode (GL_FRONT_AND_BACK, vao->FillMode);

    // Bind the VAO to use
    glBindVertexArray (vao->VertexArrayID);

    // Enable Vertex Attribute 0 - 3d Vertices
    glEnableVertexAttribArray(0);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->VertexBuffer);

    // Enable Vertex Attribute 1 - Color
    glEnableVertexAttribArray(1);
    // Bind the VBO to use
    glBindBuffer(GL_ARRAY_BUFFER, vao->ColorBuffer);

    // Draw the geometry !
    glDrawArrays(vao->PrimitiveMode, 0, vao->NumVertices); // Starting from vertex 0; 3 vertices total -> 1 triangle
}

/**************************
 * Customizable functions *
 **************************/
double last_update_time, current_time;
float camera_rotation_angle = 0;
float zoom = 1;
float pan = 0;
bool flag = 1;
float camera_location_x = 2.0;
float camera_location_y = 7.0;
float camera_location_z = 7.0;
float camera_target_x = 0.0;
float camera_target_y = 0.0;
float camera_target_z = 0.0;
int level = 0;
bool split = 0; // 1 if cuboid is splitted
bool selected_cube = 0;  // 0 if cube1 is selected amd 1 is cube2 is selected
bool left_mouse_button_clicked = 0;
bool right_mouse_button_clicked = 0;
double mouse_x,mouse_y;       //mouse coordinates
int current_state = 0, future_state = -1;
float theta = 0;
float rotation_speed = 10;
bool perspective = 0;
int score = 0;
int view = 0; // 
bool left_segment_visibility[]={0,0,0,0,0,0,0},right_segment_visibility[]={0,0,0,0,0,0,0},segment_visibility[]={0,0,0,0,0,0,0};
float left_digit_x = 7;
float right_digit_x = -2+left_digit_x;
float digit_x = -4+left_digit_x;

float left_digit_z = 8;
float right_digit_z = 8;
float digit_z = 8;

float left_digit_y= 5;
float right_digit_y = 5;
float digit_y = 5;
bool start = 1;
//converts mouse coordinates to OpenGL coordinates
float get_ogl_x(float x){
  return (2*ORTHO_X*x)/WIDTH-ORTHO_X;
}

float get_ogl_y(float y){
  return -(2*ORTHO_Y*y)/HEIGHT+ORTHO_Y;
}

void toggle_flag()
{
  if(flag == 1)
    flag = 0;
  else
    flag = 1;
}

/* Executed when window is resized to 'width' and 'height' */
/* Modify the bounds of the screen here in glm::ortho or Field of View in glm::Perspective */
void reshapeWindow (GLFWwindow* window, int width, int height)
{
    int fbwidth=WIDTH, fbheight=HEIGHT;
    /* With Retina display on Mac OS X, GLFW's FramebufferSize
     is different from WindowSize */
    glfwGetFramebufferSize(window, &fbwidth, &fbheight);

    GLfloat fov = 90.0f;

    // sets the viewport of openGL renderer
    glViewport (0, 0, (GLsizei) fbwidth, (GLsizei) fbheight);

    // set the projection matrix as perspective
    /* glMatrixMode (GL_PROJECTION);
       glLoadIdentity ();
       gluPerspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1, 500.0); */
    // Store the projection matrix in a variable for future use
    // Perspective projection for 3D views
    if(perspective)
      Matrices.projection = glm::perspective (fov, (GLfloat) fbwidth / (GLfloat) fbheight, 0.1f, 500.0f);

    // Ortho projection for 2D views
    else
      Matrices.projection = glm::ortho((-ORTHO_X + pan )/zoom, (ORTHO_X+pan)/zoom, -ORTHO_Y/zoom, ORTHO_Y/zoom, 0.1f, 500.0f);
}

class Object
{
  public:
    float x ;
    float y ;
    float z ;
    float rotation_angle ;
    glm::vec3 RotateVector;
    float centre_x;
    float centre_y;
    bool visibility;
    int cube;  // 2 if object is cube2 , 1 if object is cube1
    COLOR color;

  public:
    Object(float X = 0, float Y = 0, float Z = 0, float R = 0, glm::vec3 Vector = glm::vec3 (0,0,1)){
      x = X;
      y = Y;
      z = Z;
      rotation_angle = R;
      RotateVector = Vector;
      visibility = 1;
      cube = 0;
    }

    void draw(VAO* object ){
      glm::mat4 VP = Matrices.projection * Matrices.view;
      glm::mat4 MVP;  // MVP = Projection * View * Model
      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Translate = glm::translate (glm::vec3(x, y, z));        // glTranslatef
      glm::mat4 Rotate = glm::rotate ((float)(rotation_angle * (M_PI/180.0f)), RotateVector);
      Matrices.model *= (Translate*Rotate);
      MVP = VP * Matrices.model;
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(object);
    }

    void translate(float X = 0, float Y = 0, float Z = 0){
      x+=X;
      y+=Y;
      z+=Z;
    }

    void rotate(float R, int f = 0){
      rotation_angle += R;
      cube = f;
    }

    void setRotationAngle(float r){
      rotation_angle = r;
    }

    void setCoor(float X = 0,float Y = 0, float Z = 0 ){
      x = X;
      y = Y;
      z = Z;
    }

    void setRotateVector(glm::vec3 Vector = glm::vec3(0, 0, 1))
    {
      RotateVector = Vector;
    }
};

VAO *cube1, *cube2, *cube1_edges, *cube2_edges;
Object Cube1, Cube2, Cube1Edges, Cube2Edges;

VAO *board[NUMBER_OF_BLOCKS*NUMBER_OF_BLOCKS];
Object Board[NUMBER_OF_BLOCKS*NUMBER_OF_BLOCKS];

VAO *left_digit_segment[7],*right_digit_segment[7],*digit_segment[7];


VAO* createCube(float l, float b, float h, COLOR color = silver, bool fill = 1)
{

  GLfloat vertex_buffer_data[] = {

    //Front face
    0, b, 0,
    l, b, 0,
    l, b, h,

    l, b, h,
    0, b, h,
    0, b, 0,

    //Back face
    0, 0, 0,
    l, 0, 0,
    l, 0, h,

    l, 0, h,
    0, 0, h,
    0, 0, 0,

    //Left face
    0, 0, 0,
    0, b, 0,
    0, b, h,

    0, b, h,
    0, 0, h,
    0, 0, 0,

    //Right face
    l, 0, 0,
    l, b, 0,
    l, b, h,

    l, b, h,
    l, 0, h,
    l, 0, 0,

    //Top face
    0, 0, h,
    0, b, h,
    l, b, h,

    l, b, h,
    l, 0, h,
    0, 0, h,

    //Bottom face
    0, 0, 0,
    0, b, 0,
    l, b, 0,

    l, b, 0,
    l, 0, 0,
    0, 0, 0,
  };

  GLfloat color_buffer_data[] = {

    //Front face
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    //Back face
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    //Left face
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    //Right face
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    //Top face
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    //Bottom face
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
  };

  if(fill)
    return create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data, GL_FILL);
  return create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color_buffer_data, GL_LINE);
}

VAO *createCubeEdges(float l, float b, float h,COLOR color = silver)
{
  GLfloat vertex_buffer_data[] = {

    //Back Face
    0, 0, 0,
    l, 0, 0,
    0, 0, 0,
    l, 0, 0,
    l, 0, h,
    l, 0, 0,
    l, 0, h,
    0, 0, h,
    l, 0, h,
    0, 0, 0,
    0, 0, h,
    0, 0, 0,

    //Front face
    0, b, 0,
    l, b, 0,
    0, b, 0,
    l, b, 0,
    l, b, h,
    l, b, 0,
    l, b, h,
    0, b, h,
    l, b, h,
    0, b, 0,
    0, b, h,
    0, b, 0,

    //Top face
    0, 0, h,
    l, 0, h,
    0, 0, h,
    l, 0, h,
    l, b, h,
    l, 0, h,
    0, b, h,
    l, b, h,
    0, b, h,
    0, 0, h,
    0, b, h,
    0, 0, h,

    //Bottom face
    0, 0, 0,
    l, 0, 0,
    0, 0, 0,
    l, 0, 0,
    l, b, 0,
    l, 0, 0,
    0, b, 0,
    l, b, 0,
    0, b, 0,
    0, 0, 0,
    0, b, 0,
    0, 0, 0,

    //Left face
    0, 0, 0,
    0, 0, h,
    0, 0, 0,
    0, 0, h,
    0, b, h,
    0, 0, h,
    0, b, 0,
    0, b, h,
    0, b, 0,
    0, 0, 0,
    0, b, 0,
    0, 0, 0,

    //Right face
    l, 0, 0,
    l, 0, h,
    l, 0, 0,
    l, 0, h,
    l, b, h,
    l, 0, h,
    l, b, 0,
    l, b, h,
    l, b, 0,
    l, 0, 0,
    l, b, 0,
    l, 0, 0,
  };

  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,

    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
  };

  return create3DObject(GL_TRIANGLES, 72, vertex_buffer_data, color_buffer_data, GL_LINE);
}

/*
  7-Segment LED Display

  **0**
  1   3
  **2**
  4   6
  **5**

*/
VAO* Segment0(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    -l/2,             l,              0,
    l/2+SCORE_WIDTH,  l,              0,
    l/2+SCORE_WIDTH,  l-SCORE_WIDTH,  0,
    l/2+SCORE_WIDTH,  l-SCORE_WIDTH,  0,
    -l/2,             l,              0,
    -l/2,             l-SCORE_WIDTH,  0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
  };

  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* Segment1(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    -l/2,               l, 0,
    -l/2+SCORE_WIDTH,   l, 0,
    -l/2+SCORE_WIDTH,   0, 0,
    -l/2+SCORE_WIDTH,   0, 0,
    -l/2,               l, 0,
    -l/2,               0, 0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b
  };
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* Segment2(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    -l/2,               0,          0,
     l/2+SCORE_WIDTH,   0,          0,
     l/2+SCORE_WIDTH, -SCORE_WIDTH, 0,
     l/2+SCORE_WIDTH, -SCORE_WIDTH, 0,
    -l/2,             -SCORE_WIDTH, 0,
    -l/2,               0,          0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b
  };
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* Segment3(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    l/2,             l,   0,
    l/2+SCORE_WIDTH, l,   0,
    l/2+SCORE_WIDTH, 0,   0,
    l/2+SCORE_WIDTH, 0,   0,
    l/2,             0,   0,
    l/2,             l,   0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b
  };
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* Segment4(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    -l/2,             0,   0,
    -l/2+SCORE_WIDTH, 0,   0,
    -l/2+SCORE_WIDTH,-l,   0,
    -l/2+SCORE_WIDTH,-l,   0,
    -l/2,            -l,   0,
    -l/2,             0,   0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b
  };
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* Segment5(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    -l/2,             -l,             0,
     l/2+SCORE_WIDTH, -l,             0,
     l/2+SCORE_WIDTH, -l-SCORE_WIDTH, 0,
     l/2+SCORE_WIDTH, -l-SCORE_WIDTH, 0,
    -l/2,             -l,             0,
    -l/2,             -l-SCORE_WIDTH, 0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b
  };
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* Segment6(float l, COLOR color = red){
  GLfloat vertex_buffer_data[] = {
    l/2,             0,              0,
    l/2+SCORE_WIDTH, 0,              0,
    l/2+SCORE_WIDTH, -l-SCORE_WIDTH, 0,
    l/2+SCORE_WIDTH, -l-SCORE_WIDTH, 0,
    l/2,             -l-SCORE_WIDTH, 0,
    l/2,              0,             0
  };
  GLfloat color_buffer_data[] = {
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b,
    color.r, color.g, color.b
  };
  return create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color_buffer_data, GL_FILL);
}

VAO* createSegment(int i,float l,COLOR color=red){
  switch(i){
    case 0:
      return Segment0(l,color);
    case 1:
      return Segment1(l,color);
    case 2:
      return Segment2(l,color);
    case 3:
      return Segment3(l,color);
    case 4:
      return Segment4(l,color);
    case 5:
      return Segment5(l,color);
    case 6:
      return Segment6(l,color);
  }
}

void draw_segment(VAO *object, int f){

  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;  // MVP = Projection * View * Model

  Matrices.model = glm::mat4(1.0f);
  if(f == 0){
    glm::mat4 Translate = glm::translate (glm::vec3(left_digit_x, digit_y, 0));        // glTranslatef
    Matrices.model *= Translate;
  }
  else if(f == 1){
    glm::mat4 Translate = glm::translate (glm::vec3(right_digit_x, digit_y, 0));        // glTranslatef
    Matrices.model *= Translate;
  }
  else{
    glm::mat4 Translate = glm::translate (glm::vec3(digit_x,digit_y,0));        // glTranslatef
    Matrices.model *= Translate;
  }
  MVP = VP * Matrices.model;
  glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
  draw3DObject(object);
}

void draw_score(){

  switch(score % 10){
    case 0:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 1;
      left_segment_visibility[2] = 0;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 1;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 1;
      break;
    case 1:
      left_segment_visibility[0] = 0;
      left_segment_visibility[1] = 0;
      left_segment_visibility[2] = 0;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 0;
      left_segment_visibility[5] = 0;
      left_segment_visibility[6] = 1;
      break;
    case 2:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 0;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 1;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 0;
      break;
    case 3:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 0;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 0;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 1;
      break;
    case 4:
      left_segment_visibility[0] = 0;
      left_segment_visibility[1] = 1;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 0;
      left_segment_visibility[5] = 0;
      left_segment_visibility[6] = 1;
      break;
    case 5:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 1;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 0;
      left_segment_visibility[4] = 0;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 1;
      break;
    case 6:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 1;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 0;
      left_segment_visibility[4] = 1;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 1;
      break;
    case 7:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 0;
      left_segment_visibility[2] = 0;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 0;
      left_segment_visibility[5] = 0;
      left_segment_visibility[6] = 1;
      break;
    case 8:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 1;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 1;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 1;
      break;
    case 9:
      left_segment_visibility[0] = 1;
      left_segment_visibility[1] = 1;
      left_segment_visibility[2] = 1;
      left_segment_visibility[3] = 1;
      left_segment_visibility[4] = 0;
      left_segment_visibility[5] = 1;
      left_segment_visibility[6] = 1;
      
      break;
  }

  if(score >= 10)
    switch((score/10) % 10){
      case 0:
          right_segment_visibility[0] = 1;
          right_segment_visibility[1] = 1;
          right_segment_visibility[2] = 0;
          right_segment_visibility[3] = 1;
          right_segment_visibility[4] = 1;
          right_segment_visibility[5] = 1;
          right_segment_visibility[6] = 1;
          break;
      case 1:
        right_segment_visibility[0] = 0;
        right_segment_visibility[1] = 0;
        right_segment_visibility[2] = 0;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 0;
        right_segment_visibility[5] = 0;
        right_segment_visibility[6] = 1;
        break;
      case 2:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 0;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 1;
        right_segment_visibility[5] = 1;
        right_segment_visibility[6] = 0;
        break;
      case 3:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 0;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 0;
        right_segment_visibility[5] = 1;
        right_segment_visibility[6] = 1;
        break;
      case 4:
        right_segment_visibility[0] = 0;
        right_segment_visibility[1] = 1;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 0;
        right_segment_visibility[5] = 0;
        right_segment_visibility[6] = 1;
        break;
      case 5:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 1;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 0;
        right_segment_visibility[4] = 0;
        right_segment_visibility[5] = 1;
        right_segment_visibility[6] = 1;
        break;
      case 6:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 1;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 0;
        right_segment_visibility[4] = 1;
        right_segment_visibility[5] = 1;
        right_segment_visibility[6] = 1;
        break;
      case 7:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 0;
        right_segment_visibility[2] = 0;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 0;
        right_segment_visibility[5] = 0;
        right_segment_visibility[6] = 1;
        break;
      case 8:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 1;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 1;
        right_segment_visibility[5] = 1;
        right_segment_visibility[6] = 1;
        break;
      case 9:
        right_segment_visibility[0] = 1;
        right_segment_visibility[1] = 1;
        right_segment_visibility[2] = 1;
        right_segment_visibility[3] = 1;
        right_segment_visibility[4] = 0;
        right_segment_visibility[5] = 1;
        right_segment_visibility[6] = 1;
        break;
      }

  if(score >= 100)
    switch((score/100) % 10){
    case 0:
        segment_visibility[0] = 1;
        segment_visibility[1] = 1;
        segment_visibility[2] = 0;
        segment_visibility[3] = 1;
        segment_visibility[4] = 1;
        segment_visibility[5] = 1;
        segment_visibility[6] = 1;
        break;
    case 1:
      segment_visibility[0] = 0;
      segment_visibility[1] = 0;
      segment_visibility[2] = 0;
      segment_visibility[3] = 1;
      segment_visibility[4] = 0;
      segment_visibility[5] = 0;
      segment_visibility[6] = 1;
      break;
    case 2:
      segment_visibility[0] = 1;
      segment_visibility[1] = 0;
      segment_visibility[2] = 1;
      segment_visibility[3] = 1;
      segment_visibility[4] = 1;
      segment_visibility[5] = 1;
      segment_visibility[6] = 0;
      break;
    case 3:
      segment_visibility[0] = 1;
      segment_visibility[1] = 0;
      segment_visibility[2] = 1;
      segment_visibility[3] = 1;
      segment_visibility[4] = 0;
      segment_visibility[5] = 1;
      segment_visibility[6] = 1;
      break;
    case 4:
      segment_visibility[0] = 0;
      segment_visibility[1] = 1;
      segment_visibility[2] = 1;
      segment_visibility[3] = 1;
      segment_visibility[4] = 0;
      segment_visibility[5] = 0;
      segment_visibility[6] = 1;
      break;
    case 5:
      segment_visibility[0] = 1;
      segment_visibility[1] = 1;
      segment_visibility[2] = 1;
      segment_visibility[3] = 0;
      segment_visibility[4] = 0;
      segment_visibility[5] = 1;
      segment_visibility[6] = 1;
      break;
    case 6:
      segment_visibility[0] = 1;
      segment_visibility[1] = 1;
      segment_visibility[2] = 1;
      segment_visibility[3] = 0;
      segment_visibility[4] = 1;
      segment_visibility[5] = 1;
      segment_visibility[6] = 1;
      break;
    case 7:
      segment_visibility[0] = 1;
      segment_visibility[1] = 0;
      segment_visibility[2] = 0;
      segment_visibility[3] = 1;
      segment_visibility[4] = 0;
      segment_visibility[5] = 0;
      segment_visibility[6] = 1;
      break;
    case 8:
      segment_visibility[0] = 1;
      segment_visibility[1] = 1;
      segment_visibility[2] = 1;
      segment_visibility[3] = 1;
      segment_visibility[4] = 1;
      segment_visibility[5] = 1;
      segment_visibility[6] = 1;
      break;
    case 9:
      segment_visibility[0] = 1;
      segment_visibility[1] = 1;
      segment_visibility[2] = 1;
      segment_visibility[3] = 1;
      segment_visibility[4] = 0;
      segment_visibility[5] = 1;
      segment_visibility[6] = 1;
      break;
  }

  for(int i=0; i<= 6; i++)
  {
    if(left_segment_visibility[i]==1)
      draw_segment(left_digit_segment[i],0);
    if(right_segment_visibility[i]==1)
      draw_segment(right_digit_segment[i],1);
    if(segment_visibility[i]==1)
      draw_segment(digit_segment[i],2);
  }
}



void set_cubes()
{
  cube1 = createCube(BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH,green);
  cube1_edges = createCubeEdges(BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH,darkgreen);
  Cube1.color = green;
  Cube1Edges.color = darkgreen;

  cube2 = createCube(BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH,green);
  cube2_edges = createCubeEdges(BLOCK_WIDTH, BLOCK_WIDTH, BLOCK_WIDTH,darkgreen);
  Cube2.color = green;
  Cube2Edges.color = darkgreen;

  Cube1.setCoor(Board[block_positions[level]].x, 0, Board[block_positions[level]].z);
  Cube2.setCoor(Board[block_positions[level]].x, 0, Board[block_positions[level]].z);

  Cube2.translate(0, BLOCK_WIDTH);

  Cube1.translate(0, BOARD_HEIGHT);
  Cube2.translate(0, BOARD_HEIGHT);

  Cube1.y = 10.0f;
  Cube2.y = 10.0f + BLOCK_WIDTH;
}

void reset_cubes()
{
  set_cubes();
}


bool check_if_invalid(float x, float z)
{
  for (int i = 0; i < NUMBER_OF_BLOCKS * NUMBER_OF_BLOCKS; ++i)
  {
    if(Board[i].visibility)
    {
      if(x == Board[i].x && z == Board[i].z)
        return 0;
    }
  }

  return 1;
}

bool check_if_fragile(float x, float z)
{
  for (int i = 0; i < fragile_blocks_sizes[level]; ++i)
  {
    if(Board[fragile_blocks[i]].x == x && Board[fragile_blocks[i]].z == z)
      return 1;
  }

  return 0;
}

void check_bridges_and_splits()
{
  for (int i = 0; i < horizontal_bridge_sizes[level]; ++i)
  {
    if((Cube1.z == Cube2.z && Cube1.x != Cube2.x) || (Cube1.x == Cube2.x && Cube1.z != Cube2.z || (Cube1.x != Cube2.x && Cube1.z != Cube2.z) )){
      int position = horizontal_bridges[level][i].position;

      if((Cube1.x == Board[position].x && Cube1.z == Board[position].z) || (Cube2.x == Board[position].x && Cube2.z == Board[position].z)){

        int block1 = horizontal_bridges[level][i].bridge_position1;
        int block2 = horizontal_bridges[level][i].bridge_position2;

        if(Board[block1].visibility == 1)
          Board[block1].visibility = 0;
        else
          Board[block1].visibility = 1;

        if(Board[block2].visibility == 1)
          Board[block2].visibility = 0;
        else
          Board[block2].visibility = 1;

        start = 0;
      }
    }
  }

  for (int i = 0; i < vertical_bridge_sizes[level] && split == 0; ++i)
  {
    if(Cube1.z == Cube2.z && Cube1.x == Cube2.x){
      int position = vertical_bridges[level][i].position;
      if(Cube1.x == Board[position].x && Cube1.z == Board[position].z){
        int block1 = vertical_bridges[level][i].bridge_position1;
        int block2 = vertical_bridges[level][i].bridge_position2;

        if(Board[block1].visibility)
          Board[block1].visibility = 0;
        else
          Board[block1].visibility = 1;

        if(Board[block2].visibility)
          Board[block2].visibility = 0;
        else
          Board[block2].visibility = 1;
        start = 0;
      }
    }
  }

  for (int i = 0; i < split_sizes[level] && split == 0; ++i)
  {
    if(Cube1.z == Cube2.z && Cube1.x == Cube2.x){
      int position = splits[level][i].position;
      if(Cube1.x == Board[position].x && Cube1.z == Board[position].z){
        int block1 = splits[level][i].split_position1;
        int block2 = splits[level][i].split_position2;

        Cube1.setCoor(Board[block1].x, BOARD_HEIGHT, Board[block1].z);
        Cube2.setCoor(Board[block2].x, BOARD_HEIGHT, Board[block2].z);
        split = 1;

        start = 0;
      }
    }
  }
}

void check_split()
{
  if( (abs(Cube1.x - Cube2.x) == 1 && Cube1.z == Cube2.z) || ( abs(Cube1.z - Cube2.z) == 1 && Cube1.x == Cube2.x ) ){
      split = 0;
      if(Cube1.x == Cube2.x)
        current_state = 2;
      else
        current_state = 1;
      flag = 0;
  }
}

void handle_up_key_split()
{
  float x,y,z;
  if(selected_cube == 0){
    x = Cube1.x ;
    y = Cube1.y ;
    z = Cube1.z ;
  }

  else{
    x = Cube2.x ;
    y = Cube2.y ;
    z = Cube2.z ;
  }

  if(check_if_invalid(x, z - BLOCK_WIDTH))
    return;

  if(selected_cube == 0)
    Cube1.setCoor(x, y, z - BLOCK_WIDTH);
  else
    Cube2.setCoor(x, y, z - BLOCK_WIDTH);

  check_bridges_and_splits();
  check_split();
}

void handle_down_key_split()
{

  float x,y,z;
  if(selected_cube == 0){
    x = Cube1.x ;
    y = Cube1.y ;
    z = Cube1.z ;
  }

  else{
    x = Cube2.x ;
    y = Cube2.y ;
    z = Cube2.z ;
  }

  if(check_if_invalid(x, z + BLOCK_WIDTH))
    return;

  if(selected_cube == 0)
    Cube1.setCoor(x, y, z + BLOCK_WIDTH);
  else
    Cube2.setCoor(x, y, z + BLOCK_WIDTH);

  check_bridges_and_splits();
  check_split();
}

void handle_left_key_split()
{

  float x,y,z;
  if(selected_cube == 0){
    x = Cube1.x ;
    y = Cube1.y ;
    z = Cube1.z ;
  }

  else{
    x = Cube2.x ;
    y = Cube2.y ;
    z = Cube2.z ;
  }

  if(check_if_invalid(x - BLOCK_WIDTH, z))
    return;

  if(selected_cube == 0)
    Cube1.setCoor(x - BLOCK_WIDTH, y, z);
  else
    Cube2.setCoor(x - BLOCK_WIDTH, y, z);

  check_bridges_and_splits();
  check_split();
}

void handle_right_key_split()
{

  float x,y,z;

  if(selected_cube == 0){
    x = Cube1.x ;
    y = Cube1.y ;
    z = Cube1.z ;
  }

  else{
    x = Cube2.x ;
    y = Cube2.y ;
    z = Cube2.z ;
  }

  if(check_if_invalid(x + BLOCK_WIDTH, z))
    return;

  if(selected_cube == 0)
    Cube1.setCoor(x + BLOCK_WIDTH, y, z);
  else
    Cube2.setCoor(x + BLOCK_WIDTH, y, z);
  check_bridges_and_splits();
  check_split();
}

/*
  State 0 ===>   V
  State 1 ===>   H along X-axis
  State 2 ===>   H along Z-axis

  (1,0) right (h->v)
  (2,2) right (h->h)
  (0,1) right v->h

  (2,3) left (h->h)
  (0,3) left (v->h)
  (1,3) left (h->v)

  (2,1) up (h->v)
  (1,1) up (h->h)
  (0,4) up (v->h)

  (2,0) down (h->v)
  (0,2) down v->h
  (1,2) down (h->h)
*/

void draw_cubes()
{
  Cube1Edges.setCoor(Cube1.x, Cube1.y, Cube1.z);
  Cube2Edges.setCoor(Cube2.x, Cube2.y, Cube2.z);
  glm::mat4 VP = Matrices.projection * Matrices.view;
  glm::mat4 MVP;	// MVP = Projection * View * Model

  if(current_state == 0)
  {
    if(future_state == 1)  // right v->h
    {
      if(theta == 90){
        if(Cube2.y > Cube1.y)
        {
          Cube1.translate(BLOCK_WIDTH);
          Cube2.translate(2*BLOCK_WIDTH, -BLOCK_WIDTH);
        }

        else
        {
          Cube2.translate(BLOCK_WIDTH);
          Cube1.translate(2*BLOCK_WIDTH, -BLOCK_WIDTH);
        }

        future_state = -1;
        theta = 0;
        current_state = 1;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate = glm::translate ( glm::vec3 (-BLOCK_WIDTH,0,0)); // glTranslatef
      glm::mat4 rotator = glm::rotate((float)(-theta*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x+BLOCK_WIDTH,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate1 = glm::translate ( glm::vec3 (0,BLOCK_WIDTH,0)); // glTranslatef
      glm::mat4 translate2 = glm::translate ( glm::vec3 (0,-BLOCK_WIDTH,0)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube2.x+BLOCK_WIDTH,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*translate2*rotator*translate1*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 2)  //down v->h
    {
      if(theta == 90){
        if(Cube2.y > Cube1.y)
        {
          Cube1.translate(0, 0, BLOCK_WIDTH);
          Cube2.translate(0, -BLOCK_WIDTH, 2*BLOCK_WIDTH);
        }

        else
        {
          Cube2.translate(0, 0, BLOCK_WIDTH);
          Cube1.translate(0, -BLOCK_WIDTH, 2*BLOCK_WIDTH);
        }

        future_state = -1;
        theta = 0;
        current_state = 2;
        // Cube1.z += BLOCK_WIDTH;
        // Cube2.z += 2*BLOCK_WIDTH;
        // Cube2.y -= BLOCK_WIDTH;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate = glm::translate ( glm::vec3 (0,0,-BLOCK_WIDTH)); // glTranslatef
      glm::mat4 rotator = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z+BLOCK_WIDTH)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate1 = glm::translate ( glm::vec3 (0,BLOCK_WIDTH,0)); // glTranslatef
      glm::mat4 translate2 = glm::translate ( glm::vec3 (0,-BLOCK_WIDTH,0)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z+BLOCK_WIDTH)); // glTranslatef
      Matrices.model *= translate*translate2*rotator*translate1*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 3) //left v->h
    {
      if(theta == 90){
        if(Cube2.y > Cube1.y)
        {
          Cube1.translate(-BLOCK_WIDTH);
          Cube2.translate(-2*BLOCK_WIDTH, -BLOCK_WIDTH);
        }

        else
        {
          Cube2.translate(-BLOCK_WIDTH);
          Cube1.translate(-2*BLOCK_WIDTH, -BLOCK_WIDTH);
        }

        future_state = -1;
        theta = 0;
        current_state = 1;
        // Cube1.x -= BLOCK_WIDTH;
        // Cube2.x -= 2*BLOCK_WIDTH;
        // Cube2.y -= BLOCK_WIDTH;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 rotator = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate1 = glm::translate ( glm::vec3 (0,BLOCK_WIDTH,0)); // glTranslatef
      Matrices.model *= translate*rotator*translate1;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 4) //up v->h
    {
      if(theta == 90){
        if(Cube2.y > Cube1.y)
        {
          Cube1.translate(0, 0, -BLOCK_WIDTH);
          Cube2.translate(0, -BLOCK_WIDTH, -2*BLOCK_WIDTH);
        }

        else
        {
          Cube2.translate(0, 0, -BLOCK_WIDTH);
          Cube1.translate(0, -BLOCK_WIDTH, -2*BLOCK_WIDTH);
        }

        future_state = -1;
        theta = 0;
        current_state = 2;
        // Cube1.z -= BLOCK_WIDTH;
        // Cube2.y -= BLOCK_WIDTH;
        // Cube2.z -= 2*BLOCK_WIDTH;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 rotator = glm::rotate((float)(-theta*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate1 = glm::translate ( glm::vec3 (0,BLOCK_WIDTH,0)); // glTranslatef
      glm::mat4 translate2 = glm::translate ( glm::vec3 (0,-BLOCK_WIDTH,0)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*translate2*rotator*translate1;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else
    {
      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);
    }
  }

  else if(current_state == 1)
  {
    if(future_state == 0) //right
    {
      if(theta == 90){
        //Assuming Cube1.x > Cube2.x
        Cube1.translate(BLOCK_WIDTH);
        Cube2.translate(2*BLOCK_WIDTH, BLOCK_WIDTH);
        future_state = -1;
        theta = 0;
        current_state = 0;
        // Cube1.x += BLOCK_WIDTH;
        // Cube2.x += 2*BLOCK_WIDTH;
        // Cube2.y += BLOCK_WIDTH;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate = glm::translate ( glm::vec3 (-BLOCK_WIDTH,0,0)); // glTranslatef
      glm::mat4 rotator = glm::rotate((float)(-theta*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x+BLOCK_WIDTH,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate1 = glm::translate ( glm::vec3 (BLOCK_WIDTH,0,0)); // glTranslatef
      glm::mat4 translate1 = glm::translate ( glm::vec3 (0,BLOCK_WIDTH,0)); // glTranslatef
      glm::mat4 translate2 = glm::translate ( glm::vec3 (0,-BLOCK_WIDTH,0)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube2.x+BLOCK_WIDTH,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*Itranslate1*rotator*Itranslate*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 1) //up h->h
    {
      if(theta == 90){
        //Assuming Cube1.z > Cube2.z
        Cube1.translate(0, 0, -BLOCK_WIDTH);
        Cube2.translate(0, 0, -BLOCK_WIDTH);
        // Cube1.z -= BLOCK_WIDTH;
        // Cube2.z -= BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 1;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 rotator = glm::rotate((float)(-theta*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 2) //down
    {
      if(theta == 90){
        //Assuming Cube1.x > Cube2.x
        Cube1.translate(0, 0, BLOCK_WIDTH);
        Cube2.translate(0, 0, BLOCK_WIDTH);

        // Cube1.z += BLOCK_WIDTH;
        // Cube2.z += BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 1;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate = glm::translate ( glm::vec3 (0,0,-BLOCK_WIDTH)); // glTranslatef
      glm::mat4 rotator = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z+BLOCK_WIDTH)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z+BLOCK_WIDTH)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 3)
    {
      if(theta == 90){
        //Assuming Cube1.x > Cube2.x
        Cube1.translate(-2*BLOCK_WIDTH, BLOCK_WIDTH);
        Cube2.translate(-BLOCK_WIDTH);

        // Cube1.x -= 2*BLOCK_WIDTH;
        // Cube2.x -= BLOCK_WIDTH;
        // Cube1.y += BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 0;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 rotator = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate1 = glm::translate ( glm::vec3 (BLOCK_WIDTH,0,0)); // glTranslatef
      glm::mat4 translate2 = glm::translate ( glm::vec3 (-BLOCK_WIDTH,0,0)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*translate2*rotator*translate1;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else
    {
      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);
    }
  }

  else if(current_state == 2)
  {
    if(future_state == 0) //down
    {
      if(theta == 90){
        //Assuming Cube1.z > Cube2.z
        Cube1.translate(0, 0, BLOCK_WIDTH);
        Cube2.translate(0, BLOCK_WIDTH, 2*BLOCK_WIDTH);

        // Cube1.z += BLOCK_WIDTH;
        // Cube2.z += 2*BLOCK_WIDTH;
        // Cube2.y += BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 0;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate = glm::translate ( glm::vec3 (0,0,-BLOCK_WIDTH)); // glTranslatef
      glm::mat4 rotator = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z+BLOCK_WIDTH)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate1 = glm::translate ( glm::vec3 (0,0,BLOCK_WIDTH)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z+BLOCK_WIDTH)); // glTranslatef
      Matrices.model *= translate*Itranslate1*rotator*Itranslate*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 1) //up h->v
    {
      if(theta == 90){
        //Assuming Cube1.z > Cube2.z
        Cube1.translate(0, BLOCK_WIDTH, -2*BLOCK_WIDTH);
        Cube2.translate(0, 0, -BLOCK_WIDTH);

        // Cube1.z -= 2*BLOCK_WIDTH;
        // Cube2.z -= BLOCK_WIDTH;
        // Cube1.y += BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 0;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);

      glm::mat4 rotator = glm::rotate((float)(-theta*M_PI/180.0f), glm::vec3(1,0,0));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate1 = glm::translate ( glm::vec3 (0,0,BLOCK_WIDTH)); // glTranslatef
      glm::mat4 translate2 = glm::translate ( glm::vec3 (0,0,-BLOCK_WIDTH)); // glTranslatef
      translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*translate2*rotator*translate1;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 2) //right h->h
    {
      if(theta == 90){
        //Assuming Cube1.z > Cube2.z
        Cube1.translate(BLOCK_WIDTH);
        Cube2.translate(BLOCK_WIDTH);

        // Cube1.x += BLOCK_WIDTH;
        // Cube2.x += BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 2;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 Itranslate = glm::translate ( glm::vec3 (-BLOCK_WIDTH,0,0)); // glTranslatef
      glm::mat4 rotator = glm::rotate((float)(-theta*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x+BLOCK_WIDTH,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x+BLOCK_WIDTH,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*rotator*Itranslate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else if(future_state == 3)
    {
      if(theta == 90){
        //Assuming Cube1.z > Cube2.z
        Cube1.translate(-BLOCK_WIDTH);
        Cube2.translate(-BLOCK_WIDTH);

        // Cube1.x -= BLOCK_WIDTH;
        // Cube2.x -= BLOCK_WIDTH;
        future_state = -1;
        theta = 0;
        current_state = 2;
        check_bridges_and_splits();
      }

      Matrices.model = glm::mat4(1.0f);
      glm::mat4 rotator = glm::rotate((float)(theta*M_PI/180.0f), glm::vec3(0,0,1));  // rotate about vector (1,0,0)
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate*rotator;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);

      if(theta < 90.0f)
        theta += rotation_speed;
    }

    else
    {
      Matrices.model = glm::mat4(1.0f);
      glm::mat4 translate = glm::translate ( glm::vec3 (Cube1.x,Cube1.y,Cube1.z)); // glTranslatef
      Matrices.model *= translate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube1);
      draw3DObject(cube1_edges);

      Matrices.model = glm::mat4(1.0f);
      translate = glm::translate ( glm::vec3 (Cube2.x,Cube2.y,Cube2.z)); // glTranslatef
      Matrices.model *= translate;
      MVP = VP * Matrices.model; // MVP = p * V * M
      glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
      draw3DObject(cube2);
      draw3DObject(cube2_edges);
    }
  }
}


void keyboard (GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Function is called first on GLFW_PRESS.
  if (action == GLFW_RELEASE) {
      switch (key) {
        case GLFW_KEY_S:
          view = (view + 1)%NUMBER_OF_VIEWS;
          switch(view)
          {
            case 0: // NORMAL VIEW
              camera_location_x = 2.0f;
              camera_location_y = 7.0f;
              camera_location_z = 7.0f;
              camera_target_x = 0.0f;
              camera_target_y = 0.0f;
              camera_target_z = 0.0f;
              break;

            case 1: // TOWER VIEW
              camera_location_x = 8.0f;
              camera_location_y = 8.0f;
              camera_location_z = 8.0f;
              camera_target_x = 0.0f;
              camera_target_y = 0.0f;
              camera_target_z = 0.0f;
              break;

            case 2: //TOP VIEW
              camera_location_x = 0.001f;
              camera_location_y = 7.0f;
              camera_location_z = 0.001f;
              camera_target_x = 0.0f;
              camera_target_y = 0.0f;
              camera_target_z = 0.0f;              
              break;

            case 3: //BLOCK VIEW            
              break;

            case 4: //FOLLOW CAM VIEW
              break;

            default: //NORMAL VIEW
              camera_location_x = 2.0f;
              camera_location_y = 7.0f;
              camera_location_z = 7.0f;
              camera_target_x = 0.0f;
              camera_target_y = 0.0f;
              camera_target_z = 0.0f;
              break;

          }
          break;

        case GLFW_KEY_P:
          if(perspective)
            perspective = 0;
          else
            perspective = 1;
          reshapeWindow(window, WIDTH, HEIGHT);
          break;

        case GLFW_KEY_LEFT:
          score++;
          if(split)
          {
            handle_left_key_split();
            return;
          }
          system("mpg123 -n 30 -i -q movement.mp4 &");
          
          future_state = 3;           

          if(view == 3)
          {
            camera_location_x = min(Cube1.x , Cube2.x) - 2*BLOCK_WIDTH;
            camera_location_y = BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = (Cube1.z + Cube2.z)/2 ;
            camera_target_x = min(Cube1.x , Cube2.x) - 4*BLOCK_WIDTH;
            camera_target_y = 0;
            camera_target_z = (Cube1.z + Cube2.z)/2;
          }                    

          else if(view == 4)
          {
            camera_location_x = min(Cube1.x , Cube2.x) + 2*BLOCK_WIDTH;
            camera_location_y = 2*BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = (Cube1.z + Cube2.z)/2 ;
            camera_target_x = min(Cube1.x , Cube2.x) - 4*BLOCK_WIDTH;
            camera_target_y = 0;
            camera_target_z = (Cube1.z + Cube2.z)/2;
          }

          break;

        case GLFW_KEY_RIGHT:
          score++;
          if(split)
          {
            handle_right_key_split();
            return;
          }
          if(current_state == 0)
            future_state = 1;
          else if(current_state == 1)
            future_state = 0;
          else if(current_state == 2)
            future_state = 2;
          system("mpg123 -n 30 -i -q movement.mp4 &");

          if(view == 3)
          {
            camera_location_x = max(Cube1.x , Cube2.x) + 2*BLOCK_WIDTH;
            camera_location_y = BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = (Cube1.z + Cube2.z)/2 ;
            camera_target_x = max(Cube1.x , Cube2.x) + 4*BLOCK_WIDTH;
            camera_target_y = 0;
            camera_target_z = (Cube1.z + Cube2.z)/2;
          }

          else if(view == 4)
          {
            camera_location_x = max(Cube1.x , Cube2.x) - 2*BLOCK_WIDTH;
            camera_location_y = 2*BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = (Cube1.z + Cube2.z)/2 ;
            camera_target_x = max(Cube1.x , Cube2.x) + 4*BLOCK_WIDTH;
            camera_target_y = 0;
            camera_target_z = (Cube1.z + Cube2.z)/2;
          }

          break;

        case GLFW_KEY_UP:
          score++;
          if(split)
          {
            handle_up_key_split();
            return;
          }
          if(current_state == 0)
            future_state = 4;
          else if(current_state == 1)
            future_state = 1;
          else if(current_state == 2)
            future_state = 1;
          system("mpg123 -n 30 -i -q movement.mp4 &");
          if(view == 3)
          {
            camera_location_x = (Cube1.x + Cube2.x)/2;
            camera_location_y = BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = min(Cube1.z , Cube2.z) - 2*BLOCK_WIDTH ;
            camera_target_x = (Cube1.x + Cube2.x)/2;
            camera_target_y = 0;
            camera_target_z = min(Cube1.z , Cube2.z) - 4*BLOCK_WIDTH;
          }
          else if(view == 4)
          {
            camera_location_x = (Cube1.x + Cube2.x)/2;
            camera_location_y = 2*BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = min(Cube1.z , Cube2.z) + 2*BLOCK_WIDTH ;
            camera_target_x = (Cube1.x + Cube2.x)/2;
            camera_target_y = 0;
            camera_target_z = min(Cube1.z , Cube2.z) - 4*BLOCK_WIDTH;
          }

          break;

        case GLFW_KEY_DOWN:
          score++;
          if(split)
          {
            handle_down_key_split();
            return;
          }
          if(current_state == 0)
            future_state = 2;
          else if(current_state == 1)
            future_state = 2;
          else if(current_state == 2)
            future_state = 0;
          system("mpg123 -n 30 -i -q movement.mp4 &");

          if(view == 3)
          {
            camera_location_x = (Cube1.x + Cube2.x)/2;
            camera_location_y = BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = max(Cube1.z , Cube2.z) + 2*BLOCK_WIDTH ;
            camera_target_x = (Cube1.x + Cube2.x)/2;
            camera_target_y = 0;
            camera_target_z = max(Cube1.z , Cube2.z) + 4*BLOCK_WIDTH;
          }
          else if(view == 4)
          {
            camera_location_x = (Cube1.x + Cube2.x)/2;
            camera_location_y = 2*BLOCK_WIDTH + BOARD_HEIGHT;
            camera_location_z = max(Cube1.z , Cube2.z) - 2*BLOCK_WIDTH ;
            camera_target_x = (Cube1.x + Cube2.x)/2;
            camera_target_y = 0;
            camera_target_z = max(Cube1.z , Cube2.z) + 4*BLOCK_WIDTH;
          }

          break;

        case GLFW_KEY_SPACE:
          if(split){
            if(selected_cube)
              selected_cube = 0;
            else
              selected_cube = 1;
          }
          break;

        default:
            break;
    }
  }
  else if (action == GLFW_PRESS) {
      switch (key) {
        case GLFW_KEY_ESCAPE:
            quit(window);
            break;
        default:
            break;
      }
  }
}

/* Executed for character input (like in text boxes) */
void keyboardChar (GLFWwindow* window, unsigned int key)
{
  switch (key) {
    case 'Q':
    case 'q':
            quit(window);
            break;
    default:
      break;
  }
}

void handle_left_mouse_button_click(){
  left_mouse_button_clicked = 1;
}

void handle_left_mouse_button_release(){
  left_mouse_button_clicked = 0;
}

void handle_right_mouse_button_click(){
    right_mouse_button_clicked = 1;
    glfwGetCursorPos(window, &mouse_x, &mouse_y);
}

void handle_right_mouse_button_release(){
    right_mouse_button_clicked = 0;
}

//executed on mouseButton click or release
void mouseButton (GLFWwindow* window, int button, int action, int mods){
    switch (button) {
        case GLFW_MOUSE_BUTTON_LEFT:
            if(action == GLFW_PRESS)
              handle_left_mouse_button_click();
            else if(action == GLFW_RELEASE)
              handle_left_mouse_button_release();
            break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            if(action == GLFW_PRESS)
              handle_right_mouse_button_click();
            else if(action == GLFW_RELEASE)
              handle_right_mouse_button_release();
            break;

        default:
            break;
    }
}

//executed on mouseScroll up or down
void mouseScroll(GLFWwindow* window, double xoffset, double yoffset){
  if(yoffset == 1){      //if mouse is scrolled up
    if(zoom<2)
      zoom+=0.1;
    reshapeWindow(window, WIDTH,HEIGHT);
  }
  if(yoffset == -1){     //if mouse is scrolled down
    if(zoom>1)
      zoom-=0.1;
    if(zoom == 1)
      pan = 0;
    reshapeWindow(window, WIDTH,HEIGHT);
  }
}

GLFWwindow* initGLFW (int width, int height)
{
    GLFWwindow* window; // window desciptor/handle

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()) {
      //        exit(EXIT_FAILURE);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, "Sample OpenGL 3.3 Application", NULL, NULL);

    if (!window) {
        glfwTerminate();
        //        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval( 1 );

    /* --- register callbacks with GLFW --- */

    /* Register function to handle window resizes */
    /* With Retina display on Mac OS X GLFW's FramebufferSize
     is different from WindowSize */
    glfwSetFramebufferSizeCallback(window, reshapeWindow);
    glfwSetWindowSizeCallback(window, reshapeWindow);

    /* Register function to handle window close */
    glfwSetWindowCloseCallback(window, quit);

    /* Register function to handle keyboard input */
    glfwSetKeyCallback(window, keyboard);      // general keyboard input
    glfwSetCharCallback(window, keyboardChar);  // simpler specific character handling

    /* Register function to handle mouse click */
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetMouseButtonCallback(window, mouseButton);  // mouse button clicks
    glfwSetScrollCallback(window, mouseScroll); // mouse scroll

    return window;
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL (GLFWwindow* window, int width, int height)
{
  for(int i=0;i<=6;i++){
    left_digit_segment[i] = createSegment(i, 1);
    right_digit_segment[i] = createSegment(i, 1);
    digit_segment[i] = createSegment(i, 1);
  }


  int k = 0;
  float z_counter = 0, x_counter = 0;

  for (int i = 0; i < NUMBER_OF_BLOCKS; ++i,z_counter += BLOCK_WIDTH, x_counter = 0)
  {
    for (int j = 0; j < NUMBER_OF_BLOCKS; j+=2,x_counter+=2*BLOCK_WIDTH)
    {
      if(i % 2)
      {
          board[k] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH);
          Board[k].color = silver;
          Board[k++].setCoor(x_counter, 0, z_counter);

          board[k] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH, red);
          Board[k].color = red;
          Board[k++].setCoor(x_counter+BLOCK_WIDTH, 0, z_counter);
      }

      else
      {
          board[k] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH, red);
          Board[k++].setCoor(x_counter, 0, z_counter);
          board[k] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH);
          Board[k++].setCoor(x_counter+BLOCK_WIDTH, 0, z_counter);
      }
    }
  }

  for (int i = 0; i < NUMBER_OF_BLOCKS * NUMBER_OF_BLOCKS; ++i)
  {
    Board[i].translate(-7.0f, -15.0f, -7.0f);
    Board[i].visibility = 0;
  }

  for (int i = 0; i < fragile_blocks_sizes[level]; ++i)
  {
    board[fragile_blocks[i]] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH, orange);
    Board[fragile_blocks[i]].color = orange;
  }

  for (int i = 0; i < level_sizes[level]; ++i)
  {
    Board[levels[level][i]].visibility = 1;
  }

  for (int i = 0; i < horizontal_bridge_sizes[level]; ++i)
  {
    int block1 = horizontal_bridges[level][i].bridge_position1;
    int block2 = horizontal_bridges[level][i].bridge_position2;
    Board[block1].visibility = 0;
    Board[block2].visibility = 0;
    int position = horizontal_bridges[level][i].position;
    board[position] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH, black);
    Board[position].color = black;
  }

  for (int i = 0; i < vertical_bridge_sizes[level]; ++i)
  {
    int block1 = vertical_bridges[level][i].bridge_position1;
    int block2 = vertical_bridges[level][i].bridge_position2;
    Board[block1].visibility = 0;
    Board[block2].visibility = 0;
    int position = vertical_bridges[level][i].position;
    board[position] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH, blue);
    Board[position].color = blue;
  }

  for (int i = 0; i < split_sizes[level]; ++i)
  {
    int position = splits[level][i].position;
    board[position] = createCube(BLOCK_WIDTH, BOARD_HEIGHT, BLOCK_WIDTH, gold);
    Board[position].color = gold;
  }

  set_cubes();

  // Create and compile our GLSL program from the shaders
  programID = LoadShaders( "Sample_GL.vert", "Sample_GL.frag" );
  // Get a handle for our "MVP" uniform
  Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


  reshapeWindow (window, WIDTH, HEIGHT);

  // Background color of the scene
  glClearColor (0.3f, 0.3f, 0.3f, 0.0f); // R, G, B, A
  glClearDepth (1.0f);

  glEnable (GL_DEPTH_TEST);
  glDepthFunc (GL_LEQUAL);

  cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
  cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
  cout << "VERSION: " << glGetString(GL_VERSION) << endl;
  cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}

bool is_stage_building()
{
  for (int i = 0; i < NUMBER_OF_BLOCKS*NUMBER_OF_BLOCKS; ++i)
  {
      if(Board[i].visibility)
        if(Board[i].y < 0)
          return 1;     
  }

  if(Cube1.y > BOARD_HEIGHT)
    return 1;

  return 0;
}

void build_stage()
{
  for (int i = 0; i < NUMBER_OF_BLOCKS*NUMBER_OF_BLOCKS; ++i)
  {
    if(Board[i].y < 0 && Board[i].visibility){
        if(Board[i].y + 2 <= 0)
          Board[i].y += 5.0f;
        else
          Board[i].y += 1.0f;
        return;
    }

    if(start && i == NUMBER_OF_BLOCKS * NUMBER_OF_BLOCKS - 1)
    {
        Cube1.y -= 1.0f;
        Cube2.y -= 1.0f;
        
    }
  }
}

void fall_block()
{
  if(Cube1.y > -25.0 || Cube2.y > -25.0)
  {
    Cube1.y--;
    Cube2.y--;
  } 
  else
  {
    glfwTerminate();
  }
}

void draw ()
{
  
  if(Cube1.x == Cube2.x && Cube1.z == Cube2.z)
  {
    if(Cube1.y == Cube2.y + BLOCK_WIDTH)
    {
      float x = Cube1.x, y = Cube1.y, z = Cube1.z;
      Cube1.setCoor(Cube2.x, Cube2.y, Cube2.z);
      Cube2.setCoor(x, y, z);
    }
  }

  else if(Cube1.x == Cube2.x)
  {
    if(Cube1.z + BLOCK_WIDTH == Cube2.z)
    {
      float x = Cube1.x, y = Cube1.y, z = Cube1.z;
      Cube1.setCoor(Cube2.x, Cube2.y, Cube2.z);
      Cube2.setCoor(x, y, z);
    }
  }

  else if(Cube1.z == Cube2.z)
  {
    if(Cube1.x + BLOCK_WIDTH == Cube2.x)
    {
      float x = Cube1.x, y = Cube1.y, z = Cube1.z;
      Cube1.setCoor(Cube2.x, Cube2.y, Cube2.z);
      Cube2.setCoor(x, y, z);
    }
  }

  if(check_if_invalid(Cube1.x, Cube1.z) || check_if_invalid(Cube2.x, Cube2.z))
  {
    fall_block();
  }


  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glUseProgram (programID);

  // Eye - Location of camera. Don't change unless you are sure!!
  glm::vec3 eye ( camera_location_x, camera_location_y, camera_location_z );
  // Target - Where is the camera looking at.  Don't change unless you are sure!!
  glm::vec3 target (camera_target_x, camera_target_y, camera_target_z);
  // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
  glm::vec3 up (0, 1, 0);
  Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D

  //panning
  if(right_mouse_button_clicked){
    double new_mouse_x,new_mouse_y;
    glfwGetCursorPos(window, &new_mouse_x, &new_mouse_y);
    pan = get_ogl_x(mouse_x) - get_ogl_x(new_mouse_x);
    reshapeWindow(window, WIDTH,HEIGHT);
  }

  if(left_mouse_button_clicked)
  {
    double new_mouse_x, new_mouse_y,a,b;
    glfwGetCursorPos(window, &new_mouse_x, &new_mouse_y);
    b = get_ogl_x(new_mouse_x) - get_ogl_x(mouse_x);
    a = (get_ogl_x(new_mouse_y) - get_ogl_x(mouse_y));
    camera_location_x = 8.0 * cos(7*a*(M_PI/180.0f)) * sin(7*b*(M_PI/180.0f));
    camera_location_y = 8.0 * sin(7*a*(M_PI/180.0f)) * sin(7*b*(M_PI/180.0f));
    camera_location_z = 8.0 * cos(7*b*(M_PI/180.0f));
    // camera_location_x = 8 * cos(10*b *  (M_PI/180.0f));
    // camera_location_z = 8 * sin(10*b * (M_PI/180.0f));
    camera_target_x = 0.0f;
    camera_target_y = 0.0f;
    camera_target_z = 0.0f;
  }

  if(is_stage_building())
  {
    build_stage();
  }


  for (int i = 0; i < NUMBER_OF_BLOCKS*NUMBER_OF_BLOCKS ; ++i)
  {
      if(Board[i].visibility && i != goal_positions[level])
          Board[i].draw(board[i]);              
  }

  if(Cube1.x == Cube2.x && Cube1.z == Cube2.z && fragile_blocks_sizes[level] > 0)
  {
    if(check_if_fragile(Cube1.x, Cube1.z))
    {
      reset_cubes();
    }
  }
  draw_cubes();

  if(Cube1.x == Cube2.x && Cube1.z == Cube2.z && Cube1.x == Board[goal_positions[level]].x && Cube1.z == Board[goal_positions[level]].z )
  {
    level = (level + 1) % NUMBER_OF_LEVELS;
    score = 0;  
    start = 1;
    initGL (window, WIDTH, HEIGHT);
  }
  draw_score();
}

int main (int argc, char** argv)
{
  window = initGLFW(WIDTH, HEIGHT);

  initGL (window, WIDTH, HEIGHT);

  last_update_time = glfwGetTime();

  /* Draw in loop */
  while (!glfwWindowShouldClose(window)) {

      // OpenGL Draw commands
      draw();
      // Swap Frame Buffer in double buffering
      glfwSwapBuffers(window);

      // Poll for Keyboard and mouse events
      glfwPollEvents();

      // Control based on time (Time based transformation like 5 degrees rotation every 0.5s)
      current_time = glfwGetTime(); // Time in seconds
      if ((current_time - last_update_time) >= 1.0f) { // atleast 0.5s elapsed since last frame
          // do something every 0.5 seconds ..
          // draw();
          last_update_time = current_time;
      }
  }

  glfwTerminate();
}

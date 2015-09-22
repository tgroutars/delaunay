#include <string>

#include "main.h"
#include "mesh.h"


using namespace std;

const char *filename = "10.in";

int main(int argc, char *argv[]) {

  Mesh *mesh = new Mesh;
  mesh->read_file(filename);

  delete mesh;
}

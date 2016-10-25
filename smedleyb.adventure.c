#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

int true = 1;
int false = 0;
const int SIZE = 3;


// accepts an array of integers representing taken name indices
// this array must 
int getRandomNameIndex(int* takenIndices){
  // generate random number between 0 and 6 (7 rooms)
  int i, r;
  int taken, size;

  size = (sizeof takenIndices / sizeof *takenIndices);

  // generate a random number and test it against the list of index numbers
  do{
    taken = false;
    r = rand() % 10;
    for (i = 0; i < size; i++){
      if (sizeof takenIndices[i] == 0)
	taken = false;
	break;
      if (takenIndices[i] == r){
	taken = true;
	break;
      }
    }
  }while (taken);

  return r;
}

// creates a temporary room file and returns its name
char* createRoomFile(int roomNum, char* roomName, char* connections[]){
  char* fileName;
  int numConnections;
 
  fileName = roomName;
  numConnections = sizeof connections / sizeof *connections;

  int i = 0;
  for (; i < numConnections; i++){
    // create temp file
    int file_descriptor;
    file_descriptor = open(fileName, O_WRONLY | O_CREAT);

    if (file_descriptor < 0)
      printf("Could not open %s\n", fileName);
    else
      printf("created file %s\n", fileName);
    // populate file with info
  }

  return fileName;
}

// delete all temporary files created
void deleteRoomFiles(char* fileNames[], int numRooms){
  int res, i = 0;
  printf("numRooms: %d\n", numRooms);
  for (; i < numRooms; i++){
    // delete file associated with name
    printf("deleting file \"%s\" \n", fileNames[i]);
    res = remove(fileNames[i]);

    if (res != 0)
	printf("error: could not delete file \"%s\"\n", fileNames[i]);
  }
}

char** createConnections(int roomNum, char* fileNames[], int numConnections){
}

int main(){
  //generate random seed
  srand(time(NULL));

  //create list of room names
  char* names[10] = {"BODUP", "QEBOP", "EGNIAZ", "FLINKUS", "GREMBO", "SLIZBAR", "YICHNAZ", "WAGNUS", "SCHLEMBOB", "QUAZMO"};

  int takenNameIndices[SIZE];
  char* takenRoomNames[SIZE];

  int i;
  for (i = 0; i < SIZE; i++){
    // get a random number to assign a room name
    int roomNum = i;
    int nameIndex = getRandomNameIndex(takenNameIndices);

    printf("nameIndex: %d\n", nameIndex);

    char* roomName = names[nameIndex];
    char* roomType;
    
    takenNameIndices[i] = nameIndex;
    takenRoomNames[i] = roomName;
    
    if (i == 0)
      roomType = "START";
    else if (i == SIZE - 1)
      roomType = "END";
    else
      roomType = "MID";

    printf("roomName: %s\n", roomName);

    createRoomFile(i, roomName, NULL);
  }

  printf("takenRoomNames:\n");
  for (i = 0; i < SIZE; i++)
    printf("%s\n", takenRoomNames[i]);


  // SEGFAULT-ing
  deleteRoomFiles(takenRoomNames, SIZE);

  return 0;
}


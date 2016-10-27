#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>

int true = 1;
int false = 0;
const int SIZE = 5;


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
      if (sizeof takenIndices[i] == 0){
	taken = false;
	break;
      }
      if (takenIndices[i] == r){
	taken = true;
	break;
      }
    }
  }while (taken);

  return r;
}

// create data file
// format:
// <roomNum>&<roomName>&<type>&<conn1>,<conn2>,<conn3>
char* assembleData(int roomNum, char* roomName, char* roomType, char* connections[]){
  char* data = malloc(420);
  //char buffer[420];
  
  // convert roomNum to string
  char rn[15];
  sprintf(rn, "%d", roomNum); 
 
  // add roomNum to data
  strcpy(data, rn);
  
  // add roomName to data
  strcat(data, "&");
  strcat(data, roomName);

  // add type to data
  strcat(data, "&");
  strcat(data, roomType);
  
  return data;
}

void populateRoomFileData(int roomNum, char* roomName, char* roomType, char* connections[], int file_descriptor){
  char* data = assembleData(roomNum, roomName, roomType, connections);// "FUCKIN SHIT BRO THIS IS AN EXAMPLE";
  int size = 512;// = sizeof data / sizeof *data;
  char readBuffer[size];
  
  ssize_t nread, nwritten;

  // write the file shit
  nwritten = write(file_descriptor, data, strlen(data) * sizeof(char));

  // read the file shit
  memset(readBuffer, '\0', size);
  lseek(file_descriptor, 0, SEEK_SET);
  nread = read(file_descriptor, readBuffer, size);
  printf("File contents: \n%s\n", readBuffer);

  printf("nwritten: %d\n", nwritten);
}

// creates a temporary room file and returns its name
char* createRoomFile(int roomNum, char* roomName, char* roomType, char* connections[]){
  char* fileName;
  int numConnections;
 
  fileName = roomName;
  numConnections = sizeof connections / sizeof *connections;

  int i = 0;
  for (; i < numConnections; i++){
    // create temp file
    int file_descriptor;
    file_descriptor = open(fileName, O_RDWR | O_CREAT | O_TRUNC);

    if (file_descriptor < 0)
      printf("Could not open file \"%s\" \n", fileName);
    /*else
      printf("O_CREAT created file \"%s\" \n", fileName);
      */
    
    // populate file with info
    populateRoomFileData(roomNum, roomName, roomType, connections, file_descriptor);
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

    createRoomFile(i, roomName, roomType, NULL);
  }

  printf("takenRoomNames:\n");
  for (i = 0; i < SIZE; i++)
    printf("%s\n", takenRoomNames[i]);


  // SEGFAULT-ing
  deleteRoomFiles(takenRoomNames, SIZE);

  return 0;
}


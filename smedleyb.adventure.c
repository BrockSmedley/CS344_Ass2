#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/stat.h>

int true = 1;
int false = 0;
const int SIZE = 7;
char *dir, *path;
// path is just ./<dir>/
char* dirp = "smedleyb.rooms.";

// accepts an array of integers representing taken name indices
// this array must 
int getRandomNameIndex(int* takenIndices){
  // generate random number between 0 and 6 (7 rooms)
  int i, r;
  int taken, size;

  // generate a random number and test it against the list of index numbers
  do{
    taken = false;
    r = rand() % 10;
    for (i = 0; i < 10; i++){
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
// <roomNum>&<roomName>&<type>&[connections will go here later]
char* assembleData(int roomNum, char* roomName, char* roomType){
  char* data = malloc(420);
  
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

  // prepare for adding connections
  strcat(data, "&");
  
  return data;
}

// appends connections to data string
char* appendConnectionData(char* data, char** connections, int numConnections){
  char* output = malloc(1337);

  strcpy(output, data);

  printf("numConnections: %d\n", numConnections);

  int i = 0;
  for (; i < numConnections; i++){
    strcat(output, connections[i]);
    if (i < numConnections - 1)
      strcat(output, ",");
  }

  return output;
}

void populateRoomFileData(int roomNum, char* roomName, char* roomType, int file_descriptor){
  char* data = assembleData(roomNum, roomName, roomType);// "FUCKIN SHIT BRO THIS IS AN EXAMPLE";
    
  ssize_t nwritten;

  // write the file shit
  nwritten = write(file_descriptor, data, strlen(data) * sizeof(char));

}

char* readDataFile(int file_descriptor){
  ssize_t nread;
  int size = 512;
  static char readBuffer[512];

  // read the file shit
  memset(readBuffer, '\0', size);
  lseek(file_descriptor, 0, SEEK_SET);
  nread = read(file_descriptor, readBuffer, size);

  return readBuffer;
}

// creates a temporary room file and returns its descriptor
int createRoomFile(int roomNum, char* roomName, char* roomType){
  char* fileName;

  fileName = malloc(420);

  strcpy(fileName, dir);
  strcat(fileName, "/");
  strcat(fileName, roomName);

  int i = 0;
  // create temp file
  int file_descriptor;
  file_descriptor = open(fileName, O_RDWR | O_CREAT | O_TRUNC);

  // make sure the file was able to open, else error
  if (file_descriptor < 0){
    printf("Could not open file \"%s\" \n", fileName);
  }

  // populate file with info
  populateRoomFileData(roomNum, roomName, roomType, file_descriptor);

  return file_descriptor;
}

// delete all temporary files created
void deleteRoomFiles(char* fileNames[], int numRooms){
  printf("dir: '%s'\n", dir);
  printf("path: %s\n", path);

  int res, i = 0;
  for (; i < numRooms; i++){
    // delete file associated with name 
    char* filename = malloc(420);
    strcpy(filename, path);
    strcat(filename, fileNames[i]);
    res = remove(filename);

    printf("deleting file: %s\n", filename);
    if (res != 0)
	printf("error: could not delete file \"%s\"\n", filename);
  }

  rmdir(dir);
}

// creates connections for a room
// returns array of room names to connect to
char** createConnections(char* roomName, char* roomType, char* roomNames[], int numRooms){
  static char* conns[6]; // just using max value for EZ-ness
  int shift;
  
  if (roomType == "START")
    shift = 3;
  else
    shift = 2;
  
  int numConnections = rand() % 3; // get random number [0, 3) aka [0, 2]
  	// since all rooms have bidirectional connections, decrease range by 1
  numConnections += shift; // add shift to effectively make the range [3,5]
 
  printf("Creating connections for %s\n", roomName);
  printf("numConnections requested: %d\n", numConnections);

  int randRoom, j, i = 0, taken = false;

  for (; i < numConnections; i++){ // for the number of connections we need to make
    do{ // find a random room to connect to
      // random room index from 0 to SIZE (number of available rooms)
      randRoom = rand() % SIZE;

      // check that we haven't chosen this room already
      j = 0;
      taken = false;
      for (; j < SIZE; j++){
	if (roomName == roomNames[randRoom] || conns[j] == roomNames[randRoom]){
	  taken = true;
	  break;
	}
      }
    }while(taken);

    conns[i] = roomNames[randRoom];
    printf("new connection: %s -- %s\n", roomName, conns[i]);
  }
  return conns;
}

int main(){
  dir = malloc(420);
  path = malloc(420);
  // make qualified subdir name
  strcpy(dir, dirp);
  strcpy(path, "./");

  // get process id for subdir
  int pid = getppid();
  // turn pid into a string
  char* pidc = malloc(16);
  sprintf(pidc, "%d", pid);

  // concat the process id to the subdir
  strcat(dir, pidc);
  //printf("%s\n", dir); 
  strcat(path, dir);
  strcat(path, "/");
  
  // make our subdirectory
  mkdir(dir, S_IRUSR | S_IWUSR | S_IXUSR);
 
  int file_descriptors[7]; // keeps track of all our files

  //generate random seed
  srand(time(NULL));

  //create list of room names
  char* names[] = {"Murica", "Kazakhstan", "Djibouti", "Kiribati", "Rwanda", "Tajikistan", "Abkhazia", "Nagorno-Karabakh", "Transnistria", "Ceskoslovensko"};

  int takenNameIndices[SIZE];
  char* takenRoomNames[SIZE];

  int i;
  // create all the rooms
  for (i = 0; i < SIZE; i++){
    char* roomType;

    // get a random number to assign a room name
    int roomNum = i;
    int nameIndex = getRandomNameIndex(takenNameIndices);

    // get a name from the list
    char* roomName = names[nameIndex];

    // keep track of which names are taken
    takenNameIndices[i] = nameIndex;
    takenRoomNames[i] = roomName;
    
    if (i == 0)
      roomType = "START";
    else if (i == SIZE - 1)
      roomType = "END";
    else
      roomType = "MID";

    printf("\nroomName: %s\n", roomName);

    // make connections for the room
    char** conns;
    
    int file_descriptor = createRoomFile(i, roomName, roomType);
    file_descriptors[i] = file_descriptor;
  }

  int numConnections;
  char** connections;
  char* roomType;

  for (i = 0; i < SIZE; i++){
    numConnections = 0;
    if (i == 0)
      roomType = "START";
    else if (i == SIZE-1)
      roomType = "END";
    else
      roomType = "MID";


    char* oldData = readDataFile(file_descriptors[i]);
    printf("Old file contents:\n%s\n", oldData);
    printf("wtf\n");

    
    printf("que\n");

    connections = createConnections(takenRoomNames[i], roomType, takenRoomNames, SIZE);

    while (connections[numConnections] != NULL){
      printf("supposed connection: %s\n", connections[numConnections]);
      numConnections++;
    }

    printf("numConnections_main: %d\n", numConnections);

    char* newData = appendConnectionData(oldData, connections, numConnections);
    memset(connections, 0, numConnections * (sizeof connections[0]));

    int j = 0;
        
    printf("New file contents:\n%s\n", newData);
    
  }
  
  // deletes files
  deleteRoomFiles(takenRoomNames, SIZE);

  return 0;
}



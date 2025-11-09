#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

struct NODE* searchChild(struct NODE* parent, char* name) {
    	if (!parent || !parent->childPtr) { 
	    	return NULL;
	}//end viability if

    	struct NODE* current = parent->childPtr;
    	while (current) {
        	if (strcmp(current->name, name) == 0) {
            		return current;
        	}//end child traversal if
        	current = current->siblingPtr;
    	}//end child traversal while
    	return NULL;
}//end searchChild

//make directory
void mkdir(char pathName[]){
	 
    	char baseName[64];
    	char dirName[256];
    
    	if (strcmp(pathName, "/") == 0) {
        	printf("MKDIR ERROR: no path provided\n");
        	return;
    	}//end inputcheck if
    
    	struct NODE* parent = splitPath(pathName, baseName, dirName);

    	if (parent == NULL) {
        	return;
    	}//end error if
    
    
    	if (searchChild(parent, baseName) != NULL) {
        	printf("MKDIR ERROR: directory %s already exists\n", baseName);
        	return;
    	}//end dupecheck if

	//directory set
    	struct NODE* newDir = (struct NODE*)malloc(sizeof(struct NODE));
    	strncpy(newDir->name, baseName, 63);
    	newDir->name[63] = '\0';
    	newDir->fileType = 'D';
    	newDir->parentPtr = parent;
    	newDir->childPtr = NULL;
    	newDir->siblingPtr = NULL;

    	
    	if (parent->childPtr == NULL) {
        	parent->childPtr = newDir;
    	} else {
        	struct NODE* currentChild = parent->childPtr;
        	while (currentChild->siblingPtr != NULL) {
            		currentChild = currentChild->siblingPtr;
        	}//end child traversal while
        	currentChild->siblingPtr = newDir;
    	}//end child if

    	printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
}//end mkdir

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED SOLUTION EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR
    	

	char tempPath[256];
 	char traversePath[256];
    
 	strncpy(tempPath, pathName, 256);
 	tempPath[255] = '\0';
    
 	char* lastSlash = strrchr(tempPath, '/');
    
 	if (lastSlash == NULL) {
        	strcpy(dirName, "");
        	strcpy(baseName, pathName);
 	} else {
        	strcpy(baseName, lastSlash + 1);
        	if (lastSlash == tempPath) {
            		strcpy(dirName, "/");
        	} else {
            		*lastSlash = '\0';
            		strcpy(dirName, tempPath);
            		*lastSlash = '/';
        	}//end inner lastslash if
    	}//end outer lastslash if

    	if (strcmp(pathName, "/") == 0) {
        	strcpy(dirName, "/");
        	strcpy(baseName, "");
        	return root;
    	}//end root case if

    
    	struct NODE* current = NULL;
    

    	if (dirName[0] == '/') {
        	current = root;
        	strcpy(traversePath, dirName + 1);
    	} else {
        	current = cwd;
        	if (strlen(dirName) == 0) {
            		return cwd;
        	}// end inner if
        
        	strcpy(traversePath, dirName);
    	}//end outer if
    
    	char* token = strtok(traversePath, "/");

    	while (token) {
        	if (current->fileType != 'D') {
             		printf("ERROR: directory %s does not exist\n", token);
             		return NULL;
        	}//end inner directoryerror if
		
        	struct NODE* next = searchChild(current, token);

        	if (next == NULL || next->fileType != 'D') {
            		printf("ERROR: directory %s does not exist\n", token);
            		return NULL;
        	}//end inner directoryerror if

        	current = next;
        	token = strtok(NULL, "/");
    	}// end error while

    	return current;

}//end splitPath

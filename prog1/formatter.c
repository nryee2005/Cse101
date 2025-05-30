/**
 * @file formatter.c
 * @brief A text formatting tool that reads a file of words and produces formatted output files.
 * 
 * This program uses a stack and a list to manage and format text:
 * 
 * - A **list** (`ListPtr`) is used to store all the words read from the input file.
 *   This list enables word-by-word access and dynamic formatting line by line.
 * 
 * - A **stack** (`StackPtr`) is used to track the history of formatted output filenames.
 *   Each time the text is formatted and a new output file is created, its name is pushed onto the stack.
 *   This structure supports an "undo" feature by popping the most recent file and deleting it.
 * 
 * The user interacts with the program through commands: "format", "undo", and "quit"/"exit".
 */

 #include "List.h"
 #include "Stack.h"
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 
 #define MAX_WORD_LENGTH 100
 
 typedef enum {
	 LEFT,
	 RIGHT,
	 CENTER
 } FormatType;
 
 extern char *strdup (const char *__s);
 
 /**
  * @brief Print a string to the standard output.
  * @param str  A pointer pointing to the string to be printed
  */
 void printString(void *str) {
	 char *s = str;         // Convert/cast the void pointer back to a char pointer.
	 printf("%s    ", s);   // Print the string followed by padding spaces.
 }
 
 /**
  * @brief Print a formatted line of text to the specified output.
  * 
  * This function prints a line of text by arranging the provided words according to the specified formatting.
  * The available formats are left, right, and center justification. It then writes the formatted text to the given output.
  *
  * @param words       An array of pointers to the words that need to be printed.
  * @param count       The number of words in the 'words' array.
  * @param width       The width of the output line
  * @param usedWidth   The combined length of the words, excluding spaces.
  * @param format      The formatting type (LEFT, RIGHT, or CENTER).
  * @param output      A pointer to the FILE structure where the output will be written.
  */
 void printFormattedLine(char** words, int count, int width, int usedWidth, FormatType format, FILE* output) {
	 // Your implementation
 
	 switch (format) {
		 case LEFT: {
			 // Your implementation
		 }
		 case RIGHT: {
			 // Your implementation
		 }
		 case CENTER: {
			 // Your implementation
		 }
	 }
 }
 
 /**
  * @brief Formats and writes text from a list to a file according to the specified justification.
  * 
  * This function takes a list of words and formats the text based on the specified justification
  * (left-aligned, right-aligned, or centered). The formatted text is then written to a file
  * specified by the 'filename' parameter.
  *
  * @param list      Pointer to the list of words to be formatted.
  * @param width     Maximum width of each line in characters.
  * @param format    Justification format (LEFT, RIGHT, or CENTER).
  * @param filename  Name of the file where the formatted text will be written.
  */
 void formatText(ListPtr list, int width, FormatType format, const char* filename) {
	 // Your implementation
 
	 // Close the file after reading
 }
 
 /**
  * @brief Deletes the specified file, effectively undoing its creation.
  *
  * This function attempts to remove the file with the given filename from the filesystem.
  * If the removal succeeds, it prints a confirmation message. If it fails (for example,
  * if the file does not exist or permissions are insufficient), it reports the error.
  *
  * @param filename  Name of the file to be deleted (the â€œundoâ€ target).
  */
 void undo(const char *filename) {
	 // Your implementation
 }
 
 int main(int argc, char **argv) {
	 /** 1. Create a list to hold all words read from the input file */
	 ListPtr wordsList = createList(printString);
 
	 /** 2. Declare variables for file handling and word storage */
	 FILE* file = NULL;
	 char word[MAX_WORD_LENGTH];  // Adjust the size as needed
	 char filename[100];
 
	 /** 3. Prompt the user for the input filename and open it for reading
	  *    - Loop until fopen succeeds
	  *    - On error, print an error message and prompt again
	  *    - On success, print a confirmation
	 */
	 // Your implementation
	 
 
	 /** 4. Read words from the file one at a time
	  *    - After reading all words, close the file
	 */
	 // Your implementation
 
	 /** 5. Create a stack to keep track of formatted output filenames for undo */
	 StackPtr filenameList = createStack(printString);
 
	 /** 6. Enter a command loop that runs until the user types "quit" or "exit". Handle "format" "undo" "quit" or "exit" command
	  *  Note handling error command 
	 */
	// Your implementation
 
 
	 /** 7. After exiting the loop, clean up all dynamic memory and data structures */
	 // Your implementation
 
	 return 0;
 }
 
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

void anchorify(char *title, char *list_of_anchors, int counter) {

  char buffer[500]; //the title read from stdinput
  char anchor[500]; //the achor tag corresponding to the title
  char anchor_template[1100]; //Markdown list item
  int j=0;
  int i=0;

  strcpy(buffer, title); //copy
  while(buffer[j] != '\n') //iterate over buffer
  {
    if(buffer[j] == ' ') {
      anchor[i++] = '-';
    } //replace space with '-'
    else if((buffer[j]>='a' && buffer[j] <='z')||(buffer[j]>='A' && buffer[j] <='Z')){
      anchor[i++]  = tolower(buffer[j]);
    } //make everything lowercase
    else if(buffer[j] >='0' && buffer[j] <='9'){
      anchor[i++] = buffer[j];
    }
    j++; //else ignore other characters and increment j
  }
  anchor[i] = '\0'; //strip newline and space characters from anchor
  buffer[j] = '\0'; //strip newline and space characters from buffer

  sprintf(anchor_template, "%d. [%s]({{page.url}}#%s)\n", counter+1, buffer, anchor); //Make the corresponding markdown list structure
  strcat(list_of_anchors, anchor_template);
}


int main(int argc, char *argv[]) {

  char list_of_anchors[10000] = ""; //the final list
  int no_of_titles;
  char *p;
  char title[500] = " ";

  //Error Handling
  if( argc == 1) {
      printf("Desription: TitleToAnchor is a simple C script that takes as input from keyboard a list of titles and prints out the corresponding markdown anchor title. Each title has to be on a separate line, after using the command.\nUsage: ./TitleToAnchor <number of titles>\n");
      return 0;
  }
  else {
      no_of_titles = strtol(argv[1], &p, 10); //convert argv[1] from string to int
      if (no_of_titles == 0) {
          printf("You need to provide a number.\n");
          return 0;
      }
      else {
        for(int i=0; i<no_of_titles; i++)
        {
          fgets(title, sizeof title, stdin);
          anchorify(title, list_of_anchors, i);
        }

        printf("%s", list_of_anchors);


        }
    }
}

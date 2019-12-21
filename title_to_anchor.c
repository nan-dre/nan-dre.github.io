#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <errno.h>

void anchorify(char *title, char *list_of_anchors, int counter) {
  
  char buffer[500]; //the title read from stdinput
  char anchor[500]; //the achor tag corresponding to the title
  char anchor_template[1100]; //Markdown list item
  int j=0;
  
  strcpy(buffer, title); //copy
  while(buffer[j]) //iterate over buffer
  {
    if(buffer[j] == ' ') {
      anchor[j] = '-';
      j++;
    } //replace space with '-'
    else if((buffer[j]>='a' && buffer[j] <='z')||(buffer[j]>='A' && buffer[j] <='Z')){
      anchor[j]  = tolower(buffer[j]);
      j++ ;
    } //make everything lowercase
    
  }

  j=0;
  while(j<50 && anchor[j] != '\n')
  {
    j++;
  }
  anchor[j] = '\0'; //strip newline and space characters from anchor
  buffer[j] = '\0'; //strip newline and space characters from buffer

  sprintf(anchor_template, "%d. [%s]({{page.url}}#%s)\n", counter+1, buffer, anchor); //Make the corresponding markdown list structure
  strcat(list_of_anchors, anchor_template);
}


int main(int argc, char *argv[]) {
  
  char list_of_anchors[10000] = ""; //the final list
  int no_of_titles; 
  char *p;
  char title[500] = " ";
  errno = 0;
  no_of_titles = strtol(argv[1], &p, 10);


  if(errno != 0 || *p != '\0') {
      if (*p == '\0') {
        printf("Usage: TitleToAnchor number_of_titles\nDescription: Convert plain text header titles to a Markdown list with corresponding title and link");
      }
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

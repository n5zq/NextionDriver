/*
 *   Copyright (C) 2017,2018 by Lieven De Samblanx ON7LDS
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h> 
#include <stdbool.h>
#include <ctype.h>

#include "NextionDriver.h"
#include "basicFunctions.h"
#include "helpers.h"


//============================================================================
//
//   If needed, put your code in the subroutine below
//
//============================================================================
void processCommands() {

//-------------------------------
//       process commands
//-------------------------------

//---------------------------------------------------------------------
//
//   You can study the examples in basicFunctions.c to get an idea
//    of how to program some extra functionality
//
//---------------------------------------------------------------------
//
//   When using colors, you can use the function
//    RGBtoNextionColor( RGBcolor)
//
//   The 'page' variable (int) holds the last selected page number
//    (0=MMDVM, 1=Dstar, 2=DMR, ...)
//
//---------------------------------------------------------------------
      // YSF Linked Reflector
      char text[100];
  
      char logpath[] = "/var/log/pi-star";
      struct dirent *de;
      char * line = NULL;
      size_t len = 0;
      ssize_t read;
      char * strfound; 
      char reflector[75]; 

      DIR *dr = opendir(logpath); 
  
      if (dr == NULL)
      {
          sprintf(text, "t35.txt=\"?\"");
      } else {
    	    while ((de = readdir(dr)) != NULL)
	        { 
	          if (strncmp(de->d_name, "YSF", strlen("YSF")) == 0)
	          {
	            char *fullpath = malloc(strlen(logpath) + strlen(de->d_name) + 2);
	            if (fullpath == NULL) 
	            { 
		            sprintf(text, "t35.txt=\"?\"");
	            } else {
	    	          sprintf(fullpath, "%s/%s", logpath, de->d_name);

	    	          deviceInfoFile = fopen (fullpath, "r");  
	    
	    	          while ((read = getline(&line, &len, deviceInfoFile)) != -1) 
                  {
	      	            strfound = strstr(line, "Linked");
	      	            if (strfound) 
	      	            {
			                  strfound = strtok(strfound, "\n");
			                  strfound = trimwhitespace(strfound);
			                  sprintf(reflector, "%s", strfound);
	      	            }
			            }

	    	          fclose(deviceInfoFile);
		              sprintf(text, "t35.txt=\"%s\"", reflector);
	            }
	         } 
        }
      }
      closedir(dr); 
    
      sendCommand(text);
}

char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace((unsigned char)*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace((unsigned char)*end)) end--;

  // Write new null terminator character
  end[1] = '\0';

  return str;
}

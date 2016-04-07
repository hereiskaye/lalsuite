/*
*  Copyright (C) 2007 Badri Krishnan
*
*  This program is free software; you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 2 of the License, or
*  (at your option) any later version.
*
*  This program is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with with program; see the file COPYING. If not, write to the
*  Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston,
*  MA  02111-1307  USA
*/

/**
 * \file
 * \ingroup lalapps_pulsar_Hough
 */

#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]){

  int i, arg;
  float h0list[173][2] = {
    {9.000000e-23, 1.000000e-22},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {8.000000e-23, 9.000000e-23},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {8.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {8.000000e-23, 1.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 7.000000e-23},
    {5.000000e-23, 7.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {5.000000e-23, 7.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 7.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {5.000000e-23, 7.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 7.000000e-23},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {6.000000e-23, 7.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {6.000000e-23, 7.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {8.000000e-23, 9.000000e-23},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {7.000000e-23, 9.000000e-23},
    {6.000000e-23, 8.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {7.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {8.000000e-23, 9.000000e-23},
    {8.000000e-23, 9.000000e-23},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {7.000000e-23, 8.000000e-23},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {8.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
    {9.000000e-23, 1.000000e-22},
    {9.000000e-23, 5.000000e-22},
  };
  i = 0; 
  if (argc > 1) i = atoi( argv[argc-1] );
  if ((i>=1)&&(i<=173)) printf("%e  %e\n", h0list[i-1][0], h0list[i-1][1]);
  
  return 0;
}

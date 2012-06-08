/************************************************************************
 * This file is part of Wizznic.                                        *
 * Copyright 2009-2012 Jimmy Christensen <dusted@dusted.dk>             *
 * Wizznic is free software: you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by *
 * the Free Software Foundation, either version 3 of the License, or    *
 * (at your option) any later version.                                  *
 *                                                                      *
 * Wizznic is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of       *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        *
 * GNU General Public License for more details.                         *
 *                                                                      *
 * You should have received a copy of the GNU General Public License    *
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.      *
 ************************************************************************/

#include "teleport.h"
#include "pixel.h"
#include "defs.h"
#include "strings.h"

void teleFreeList( listItem* l )
{
  listItem* it = l;
  while( (it=it->next) )
  {
    free( it->data );
  }
  freeList( l );
}

void teleAddToList( listItem* l, int sx, int sy, int dx, int dy )
{
  telePort_t* t = malloc( sizeof(telePort_t) );

  t->sx=sx;
  t->sy=sy;
  t->dx=dx;
  t->dy=dy;

  listAddData(l, (void*)t );
}
void teleAddFromString(listItem* l, const char* str)
{
  char* srcStr = malloc( sizeof(char)*10 );
  char* dstStr = malloc( sizeof(char)*10 );
  char* tempStrX = malloc( sizeof(char)*5 );
  char* tempStrY = malloc( sizeof(char)*5 );
  int fail=1;
  int sx,sy,dx,dy;

  if(splitVals(':',str, srcStr, dstStr) )
  {
    if(splitVals(',', srcStr, tempStrX, tempStrY))
    {
      sx = atoi(tempStrX);
      sy = atoi(tempStrY);
      if(splitVals(',', dstStr, tempStrX, tempStrY))
      {
        fail=0;
        dx = atoi(tempStrX);
        dy = atoi(tempStrY);
        teleAddToList(l,sx,sy,dx,dy);
      }
    }
  }

  if(fail)
    printf("teleAddFromString failed because of string '%s'\n", str);

  free(srcStr);
  free(dstStr);
  free(tempStrX);
  free(tempStrY);
}

void teleRemoveFromList( listItem* l, int sx, int sy )
{
  listItem* it = l;
  telePort_t* t;
  while( (it=it->next) )
  {
    t = (telePort_t*)it->data;
    if(t->sx==sx && t->sy==sy)
    {
      listRemoveItem(l, it);
    }
  }
}

char* teleMkStrings(listItem* l)
{
  if(!listSize(l)) return(0);

  listItem* it = l;
  telePort_t* t;
  int bytes =  sizeof(char)*(listSize(l)*strlen("teleport=10,10:10,10\n"));
  char* str = malloc( bytes );
  memset(str,0,bytes);

  while( (it=it->next) )
  {
    t = (telePort_t*)it->data;
    sprintf(str, "%steleport=%i,%i:%i,%i\n",str, t->sx,t->sy,t->dx,t->dy);
  }

  return(str);
}

int_fast8_t telePresent( listItem* l, int sx,int sy)
{
  listItem* it = l;
  telePort_t* t;
  while( (it=it->next) )
  {
    t = (telePort_t*)it->data;
    if( t->sx==sx && t->sy==sy)
    {
      return(1);
    }
  }
  return(0);
}

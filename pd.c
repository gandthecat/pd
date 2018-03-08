#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct p_dubs
{
  int p;
  int x;
  int y;
  struct p_dubs *next;
} pdoubles;

void pd (int x, int y);
void pa (int x, int y);
float point_add (int x, int y);
int numerator (int y);
int squared (int i);
int mod (int y, int p);
int mod2 (int a, int b);
void point_double (int n, int x, int y, int p);
float intersect (int i, int j, int k, int l);
pdoubles *add_xy_points (pdoubles * c, int *x, int *y);
void delete_xy_points (pdoubles * delc);
void setFirstPoint (pdoubles * pd);

const int A;//to define
const int B;//to define
int p;
  
int rx;//px
int ry;//py
int xr;//qx
int xy;//qy 

pdoubles *head;//last inserted point 
pdoubles *tail;//first inserted point 
pdoubles *temp2;//temp for iteration

int main (int argc, char *argv[])
{
  int n = 0;
  int x1 = 0; 
  int y1 = 0; 
  if (argc != 5)
    {
      printf ("Order n, Point x, Point y and Prime p required (n,x,y,p)");
      return 0;
    }
  else
    {
      n = strtol (argv[1], NULL, 10);
      x1 = strtol (argv[2], NULL, 10);
      y1 = strtol (argv[3], NULL, 10);
      p = strtol (argv[4], NULL, 10);
    }

  if ((mod (pow (y1, 2), p)) != mod ((pow (x1, 3) + A * x1 + B), p))
    {
      printf ("Point (%d,%d) not on curve", x1, y1);
      return 0;
    }

   point_double (n, x1, y1, p);

  return 0;
}

void pd (int x, int y)
{ 
  int z = (ry - y) * mod ((rx - x), p);
  int hl = mod2 (z, p);
  xr = pow (hl, 2) - x - rx;
  xy = (hl * (x - xr)) - y;
  xr = mod2 (xr, p);
  xy = mod2 (xy, p);
}

void pa (int x, int y)
{
  int d = (denominator (x) * mod (numerator (y), p)) % p;
  rx = mod2 ((squared (d) - x - x), p);
  ry = mod2 (((d * (x - rx)) - y), p);
}

int squared (int i)
{
  return i * i;
}

float point_add (int x, int y)
{
  return (float) (3 * squared (x) + A) / (float) (2 * y);
}

int denominator (int x)
{
  return (3 * squared (x) + A);
}

int numerator (int y)
{
  return 2 * y;
}

float intersect (int i, int j, int k, int l)
{
  return (float) (i - j) / (k - l);
}

int mod (int y, int p)
{
  int g;

  for (g = 1; g <= p; g++)
    {
      if ((g * y) % (p) == (1 % p))
	{
	  return g;
	}
    }
  return 0;
}

void setFirstPoint (pdoubles * pd)
{
  if (head == NULL)
    {
      head = pd;
      head->next = NULL;
    }
  else
    {
      if (temp2 == NULL)
	{
	  temp2 = head;
	} 
      pd->next = NULL;
      head->next = pd;
      head = pd;
    }
}

pdoubles * add_xy_points (pdoubles * c, int *x, int *y)
{
  pdoubles *addXYPoints;

  if (c == NULL)
    {
      addXYPoints = (pdoubles *) malloc (sizeof (pdoubles));
      addXYPoints->x = *x;
      addXYPoints->y = *y;
      addXYPoints->next = NULL;
    }
  else
    {
      addXYPoints = (pdoubles *) malloc (sizeof (pdoubles));
      addXYPoints->x = *x;
      addXYPoints->y = *y;
      addXYPoints->next = c;
    }

  setFirstPoint (addXYPoints);
  return addXYPoints;
}

void delete_xy_points (pdoubles * c)
{
  while (c)
    {
      pdoubles *next = c->next;
      free (c);
      c = next;
    }
}

int mod2 (int a, int b)
{
  int r = a % b;
  return r < 0 ? r + b : r;
}

void point_double (int n, int x, int y, int p)
{
  pdoubles *pds = (pdoubles *) malloc (sizeof (pdoubles));
  pds = add_xy_points (pds, &x, &y);
  pa (x, y);
  if (!tail)
    {
      tail = pds;
    }
  pds = add_xy_points (pds, &rx, &ry);

  for (int j = 2; j <= n; j++)
    {
      if (j < 5) // generate first 4 points by point add/point multiply
	{
	  if (j % 2 != 0)
	    {
	      pa (x, y);
	      pds = add_xy_points (pds, &rx, &ry);
	    }
	  else
	    {
	      pd (x, y);
	      x = rx;
	      y = ry;

	      if (j < 4)
		{
		  pds = add_xy_points (pds, &xr, &xy);
		}
	    }
	}
      if (j >= 5) //generate the rest based on first point and j point where j >= 5
	{
	  rx = (tail->x > head->x) ? tail->x : head->x;
	  ry = (tail->x > head->x) ? tail->y : head->y; 
	  x = (tail->x > head->x) ? head->x : tail->x;
	  y = (tail->x > head->x) ? head->y : tail->y;
	  
	  pd (x, y);
	  pds = add_xy_points (pds, &xr, &xy);
	  head = pds;
	}
    }

  while (temp2 != NULL)
    {
      printf ("(%d,%d) \n", temp2->x, temp2->y);
      temp2 = temp2->next;
    }
  delete_xy_points (head);
}

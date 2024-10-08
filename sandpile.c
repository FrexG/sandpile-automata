#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<raylib.h>

#define GRID_SIZE 300  
#define CELL_SIZE 1 

#define GRID_DIM (GRID_SIZE / CELL_SIZE) 

Color colors[] = {WHITE,RED,BLUE,MAROON};

typedef struct{
  size_t grain_count;
  Color color;
}Vertex;

void deallocVertice(Vertex **v){
  for(size_t i = 0; i < GRID_DIM*GRID_DIM; ++i)
    free(v[i]);
  free(v);
}

void initVertices(Vertex **vertices){
  // randomly initialize all vertices
  for(size_t i = 0; i < GRID_DIM; ++i){
    for(size_t j = 0; j < GRID_DIM; ++j){
      size_t c = 2;
      Vertex *v = calloc(1,sizeof(Vertex));
      if(i == GRID_DIM/2 && j == GRID_DIM/2 )
        c = 40000;
      v->grain_count = c;
      if(c >= 4)
        v->color = colors[0]; 
      else
        v->color = colors[c]; 
      vertices[i + j*GRID_DIM] = v;
    }
  }
}

void renderPixels(Vertex **vertices){
  for(size_t i = 0; i < GRID_DIM; ++i){
    for(size_t j = 0; j < GRID_DIM; ++j){
      Vertex *v = vertices[i + j* GRID_DIM];
      DrawPixel(i,j,v->color);
      //DrawRectangle(i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE, v->color);
    }
  }
}

void updateState(Vertex **vertices){
  Vertex **new_vertices = calloc(GRID_DIM*GRID_DIM, sizeof(Vertex));
  memcpy(new_vertices,vertices, GRID_DIM*GRID_DIM*sizeof(Vertex)); 

  for(size_t i = 0; i < GRID_DIM; ++i){
    for(size_t j = 0; j < GRID_DIM; ++j){

      Vertex *c = vertices[i + j*GRID_DIM]; // current
      Vertex *n = new_vertices[i + j*GRID_DIM]; // new
      //
      size_t count = c->grain_count;
      if(count < 4){
        continue; // its stable do nothing
      }
      // check the 4 neighbours
      n->grain_count = (count - 4);
      // Check and update the four neighbors
      if(i > 0) new_vertices[i-1 + j*GRID_DIM]->grain_count++;
      if(i < GRID_DIM-1) new_vertices[i+1 + j*GRID_DIM]->grain_count++;
      if(j > 0) new_vertices[i + (j-1)*GRID_DIM]->grain_count++;
      if(j < GRID_DIM-1) new_vertices[i + (j+1)*GRID_DIM]->grain_count++;
    }
  }
  for(size_t i = 0; i < GRID_DIM; ++i){
    for(size_t j = 0; j < GRID_DIM; ++j){
      Vertex *c = vertices[i + j*GRID_DIM];
      Vertex *n = new_vertices[i + j*GRID_DIM];
      size_t count = c->grain_count;
      c->grain_count = n->grain_count;

      if(count >= 4)
        c->color = colors[0];
      else
        c->color = colors[count];
    }
  }
  free(new_vertices);
}


int main(void){
  Vertex **vertices = calloc(GRID_DIM*GRID_DIM, sizeof(Vertex*));
  initVertices(vertices);

  InitWindow(GRID_SIZE, GRID_SIZE, "Sandpiles");
  SetTargetFPS(30);

  while(!WindowShouldClose()){
    BeginDrawing();
      ClearBackground(BLACK);
      for(int i = 0; i < 50; ++i)
        updateState(vertices);
      renderPixels(vertices);
    EndDrawing();
  }


  CloseWindow();
  deallocVertice(vertices);
  return 0;
}

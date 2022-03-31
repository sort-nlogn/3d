#include <iostream>
#include <graphics.h>
#include "quat.hpp"

#define f 100.0
#define n 1.0
#define num_vertices 6
#define num_faces 8
#define sc(x) (int)round(x)

#define width 620
#define height 480

void cross(float u[], float v[], float *buff){
    buff[0] = u[1] * v[2] - v[1] * u[2];
    buff[1] = v[0] * u[2] - u[0] * v[2];
    buff[2] = u[0] * v[1] - v[0] * u[1];
}

float dot(float u[], float v[]){
    float result = u[0] * v[0] + u[1] * v[1] + u[2] * v[2];
    return result;
}

void rasterize(float pos[], int ind[]){
    for(int i = 0; i < num_vertices; i++){
        pos[4 * i] = (pos[4 * i] * 0.5 + 0.5) * width;
        pos[4*i + 1] = (pos[4*i + 1] * 0.5 + 0.5) * height;
        // printf("%.3f %.3f\n", pos[4 * i], pos[4 * i + 1]);
    }

    for(int i = 0; i < num_faces; i++){
        float v1[2], v2[2], v3[2];
        v1[0] = pos[4 * ind[3 * i]];
        v1[1] = pos[4 * ind[3 * i] + 1];

        v2[0] = pos[4 * ind[3 * i + 1]];
        v2[1] = pos[4 * ind[3 * i + 1] + 1];

        v3[0] = pos[4 * ind[3 * i + 2]];
        v3[1] = pos[4 * ind[3 * i + 2] + 1];

        line(sc(v1[0]), sc(v1[1]), sc(v2[0]), sc(v2[1]));
        line(sc(v2[0]), sc(v2[1]), sc(v3[0]), sc(v3[1]));
        line(sc(v3[0]), sc(v3[1]), sc(v1[0]), sc(v1[1]));

    }
}

void vertex(float coords[], float proj[4][4], int indeces[]){
    float pos[4 * num_vertices];
    for(int i = 0; i < num_vertices; i++){
        for(int j = 0; j < 3; j++) // copy coords + w addition
            pos[4 * i + j] = coords[3 * i + j];
        pos[4 * i + 3] = 1.0;

        float new_coords[4] = {0.0};
        for(int j = 0; j < 4; j++){ // multiplying by projection matrix
            float dot = 0.0;
            for(int k = 0; k < 4; k++){
                dot += proj[j][k] * pos[4 * i + k];
            }
            new_coords[j] = dot;
        }

        for(int j = 0; j < 4; j++) // w-divide
            pos[4 * i + j] = new_coords[j] / new_coords[3]; 
    }

    rasterize(pos, indeces);
}

void process_keyboard(float coords[], float c[]){
    char ch = (char)getch();
    float axis[3] = {0.0};
    float theta = 0.1;

    if(ch == 75 || ch == 77){
        axis[1] = 1.0;
        if(ch == 77)
            theta *= -1.0;
    }else if(ch == 72 || ch == 80){
        axis[0] = 1.0;
        if(ch == 80)
            theta *= -1.0; 
    }else{
        return;
    }

    for(int i = 0; i < num_vertices; i++){
        float v[3] = {coords[3 * i] - c[0],
            coords[3 * i + 1] - c[1],
            coords[3 * i + 2] - c[2]};

        q_rotate(v, theta, axis, v);

        coords[3 * i] = c[0] + v[0];
        coords[3 * i + 1] = c[1] + v[1];
        coords[3 * i + 2] = c[2] + v[2];
    }
}

int main(){
    int win = initwindow(620, 480, "3d");

    float projection[4][4] = {{n, 0, 0, 0},
                               {0, n, 0, 0},
                               {0, 0, -(f+n) / (f-n), -2*f*n / (f-n)},
                               {0, 0, -1, 0}};

    float coords[] = {-0.5, -0.5, -2.0,
                      0.5, -0.5, -2.0,
                      0.0, -0.5, -3.0,
                     -0.5,  0.5, -2.0,
                      0.5,  0.5, -2.0,
                      0.0,  0.5, -3.0};

    int indeces[] = {2, 1, 0, 0, 4, 3,
                     0, 1, 4, 2, 0, 5,
                     0, 3, 5, 1, 2, 4,
                     4, 2, 5, 3, 4, 5};


    float centroid[3] = {-0.5, -0.5, -2.0};

    while(1){
        vertex(coords, projection, indeces);
        if(kbhit()){
            process_keyboard(coords, centroid);
        }
        swapbuffers();
        clearviewport();
        Sleep(1);
    }

    return 0;
}
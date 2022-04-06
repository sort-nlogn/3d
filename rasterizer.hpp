#pragma once
using namespace std;

void fill_flat_bottom(float v1[], float v2[], float v3[]){
    float slope1 = (v2[0] - v1[0]) / (v2[1] - v1[1]);
    float slope2 = (v3[0] - v1[0]) / (v3[1] - v1[1]);

    float x1 = v1[0], x2 = v1[0];

    for(int y = (int)v1[1]; y <= (int)v2[1]; y++){
        line((int)x1, (int)y, (int)x2, (int)y);
        x1 += slope1;
        x2 += slope2;
    }
}

void fill_flat_top(float v1[], float v2[], float v3[]){
    float slope1 = (v3[0] - v1[0]) / (v3[1] - v1[1]);
    float slope2 = (v3[0] - v2[0]) / (v3[1] - v2[1]);

    float x1 = v3[0], x2 = v3[0];

    for(int y = (int)v3[1]; y >= (int)v1[1]; y--){
        line((int)x1, (int)y, (int)x2, (int)y);
        x1 -= slope1;
        x2 -= slope2;
    }
}

void rasterize_triangle(float v1[], float v2[], float v3[]){
    if(v1[1] > v2[1])
        swap(v1, v2);
    if(v1[1] > v3[1])
        swap(v1, v3);
    if(v2[1] > v3[1])
        swap(v2, v3);

    if(v1[1] == v2[1]){
        fill_flat_top(v1, v2, v3);
    }else if(v2[1] == v3[1]){
        fill_flat_bottom(v1, v2, v3);
    }else{
        float v4[] = {v1[0] + ((v2[1] - v1[1]) / 
                       (v3[1] - v1[1])) * (v3[0] - v1[0]), 
                       v2[1]};
        fill_flat_bottom(v1, v2, v4);
        fill_flat_top(v2, v4, v3);
    }
}

// void lined_fill(float v1[], float v2[], float v3[]){
//     float y_from = min(v1[1], min(v2[1], v3[1]));
//     float y_to = max(v1[1], max(v2[1], v3[1]));

//     int y_from_i = (int)y_from;
//     int y_to_i = (int)y_to;

//     for(int y = y_from_i; y < y_to_i; y++){
//         int x_from = 
//     }
// }
/*implementation of sobel filter for image manipulation*/

#pragma once
#include <math.h>

void sobel(float** input_image, float** output_image,int start_row, int start_col, int end_row, int end_col) {
	int gx[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int gy[3][3] = { {1, 2, 1}, {0, 0, 0}, {-1, -2, -1} };
	
	//prolazak kroz matricu
	for (int i = start_row; i < end_row; i++) {
		for (int j = start_col; j < end_col; j++) {			
			//suma po x osi
			float sumx = 
				input_image[i - 1][j - 1] * gx[0][0] + input_image[i - 1][j + 1] * gx[0][2] + 
				input_image[i][j - 1] * gx[1][0] + input_image[i][j + 1] * gx[1][2] +
				input_image[i + 1][j - 1] * gx[2][0] + input_image[i + 1][j + 1] * gx[2][2];
			//suma po y osi
			float sumy =
				input_image[i - 1][j - 1] * gy[0][0] + input_image[i - 1][j] * gy[0][1] + input_image[i - 1][j + 1] * gy[0][2] +
				input_image[i + 1][j - 1] * gy[2][0] + input_image[i + 1][j] * gy[2][1] + input_image[i + 1][j + 1] * gy[2][2];

			float result = pow(sumx, 2) + pow(sumy, 2);
			result = sqrtf(result);

			output_image[i][j] = result;
		}
	}	
}
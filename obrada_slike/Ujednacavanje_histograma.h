/*Algorithm for histogram equalization*/

void calculate_histogram(float* histogram, float** input_image, int row_start, int row_end, int col_start, int col_end) {	
	
	for (int i = row_start; i < row_end; i++) {
		for (int j = col_start; j < col_end; j++) {
			histogram[ (int)input_image[i][j] ]++;
		}
	}
}

void calculate_sum_of_histogram(float* histogram, int color_depth) {
	for (int i = 1; i < color_depth; i++) {
		histogram[i] += histogram[i - 1];
	}
}

void calculate_normalised_sum_of_histogram(float* histogram,int start, int end, int rows, int cols, int color_depth) {
	for (int i = start; i < end; i++) {
		histogram[i] = histogram[i] / (rows * cols) * color_depth;
	}
}

void set_new_values_to_image(float** input_image, float* histogram, int row_start, int row_end, int col_start, int col_end) {
	for (int i = row_start; i < row_end; i++) {
		for (int j = col_start; j < col_end; j++) {
			input_image[i][j] = histogram[(int)input_image[i][j]];
			
			//making sure that values stay in monocromatic ranges (0-255)
			if (input_image[i][j] > 255) {
				input_image[i][j] = 255;
			}
			else if (input_image[i][j] < 0) {
				input_image[i][j] = 0;
			}
		}
	}

}
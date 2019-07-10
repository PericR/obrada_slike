#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <cmath>
#include <cfloat>
#include <exception>
#include <algorithm>
#include "CImg.h"
#include "Sobel.h"
#include "Ujednacavanje_histograma.h"
#include "Mjerenje.h"
#include <thread>
// u linuxu treba biti 
//#include <CImg.h>

using namespace cimg_library;
using namespace std;

int main()
{
	const int COLOR_DEPTH = 256;//monocromatic images
	CImg<float> image("lena.pgm");

	int rows = image.height();
	int cols = image.width();
	
	int size = image.size();  //uraèunata sva tri kanala pa moramo dijelit sa 3

	float** sImg;	
	//alokacija matrice
	sImg = new float* [rows];
	for (int i = 0; i < rows; i++) {
		sImg[i] = new float[cols];
	}
	
	//niz za histogram
	float histogram[COLOR_DEPTH];
	for (int i = 0; i < COLOR_DEPTH; i++) {
		histogram[i] = 0;
	}
	//prebacivanje iz Cimg objekta u prostu matricu
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			sImg[i][j] = image(i * cols + j);
		}

	/////////////////////////
	///korisni rad
	std::cout << "rows" << rows << std::endl;
	std::cout << "cols" << cols << std::endl;
	
	//sequential	
	/*double start_seq = get_wall_time();
	calculate_histogram(histogram, sImg, 0, rows, 0, cols);
	calculate_sum_of_histogram(histogram, COLOR_DEPTH);
	calculate_normalised_sum_of_histogram(histogram, 0, COLOR_DEPTH, rows, cols, COLOR_DEPTH);
	set_new_values_to_image(sImg, histogram, 0, rows, 0, cols);
	double end_seq = get_wall_time();

	cout << "Vrijeme obrade sekvencijalno: " << end_seq - start_seq << endl;*/



	////paralell sa 2 jezgre
	//double start_par2 = get_wall_time();
	//
	//thread h1, h2;
	//h1 = thread(calculate_histogram, histogram, sImg, 0, rows / 2, 0, cols);
	//h2 = thread(calculate_histogram, histogram, sImg, rows / 2, rows, 0, cols);
	//h1.join();
	//h2.join();

	//calculate_sum_of_histogram(histogram, COLOR_DEPTH);

	//thread sn1, sn2;
	//sn1 = thread(calculate_normalised_sum_of_histogram, histogram, 0, COLOR_DEPTH / 2, rows, cols, COLOR_DEPTH);
	//sn2 = thread(calculate_normalised_sum_of_histogram, histogram, COLOR_DEPTH / 2, COLOR_DEPTH, rows, cols, COLOR_DEPTH);
	//sn1.join();
	//sn2.join();

	//thread t1, t2;
	//t1 = thread(set_new_values_to_image, sImg, histogram, 0, rows / 2, 0, cols);
	//t2 = thread(set_new_values_to_image, sImg, histogram, rows / 2, rows, 0, cols);

	//t1.join();
	//t2.join();

	//double end_par2 = get_wall_time();
	//cout << "Vrijeme obrade paralelno sa dvije jezgre: " << end_par2 - start_par2 << endl;



	//paralell sa 4 jezgre
	double start_par4 = get_wall_time();

	thread h1, h2, h3, h4;
	h1 = thread(calculate_histogram, histogram, sImg, 0, rows / 4, 0, cols);
	h2 = thread(calculate_histogram, histogram, sImg, rows / 4, rows / 2, 0, cols);
	h3 = thread(calculate_histogram, histogram, sImg, rows / 2, 3 * (rows / 4), 0, cols);
	h4 = thread(calculate_histogram, histogram, sImg, 3 * (rows / 4), rows, 0, cols);
	h1.join();
	h2.join();
	h3.join();
	h4.join();

	calculate_sum_of_histogram(histogram, COLOR_DEPTH);

	thread sn1, sn2, sn3, sn4;
	sn1 = thread(calculate_normalised_sum_of_histogram, histogram, 0, COLOR_DEPTH / 4, rows, cols, COLOR_DEPTH);
	sn2 = thread(calculate_normalised_sum_of_histogram, histogram, COLOR_DEPTH / 4, COLOR_DEPTH / 2, rows, cols, COLOR_DEPTH);
	sn3 = thread(calculate_normalised_sum_of_histogram, histogram, COLOR_DEPTH / 2, 3 * (COLOR_DEPTH / 4), rows, cols, COLOR_DEPTH);
	sn4 = thread(calculate_normalised_sum_of_histogram, histogram, 3 * (COLOR_DEPTH / 4), COLOR_DEPTH, rows, cols, COLOR_DEPTH);
	sn1.join();
	sn2.join();
	sn3.join();
	sn4.join();

	thread t1, t2, t3, t4;
	t1 = thread(set_new_values_to_image, sImg, histogram, 0, rows / 4, 0, cols);
	t2 = thread(set_new_values_to_image, sImg, histogram, rows / 4, rows / 2, 0, cols);
	t3 = thread(set_new_values_to_image, sImg, histogram, rows / 2, 3 * (rows / 4), 0, cols);
	t4 = thread(set_new_values_to_image, sImg, histogram, 3 * (rows / 4), rows, 0, cols);
	t1.join();
	t2.join();
	t3.join();
	t4.join();

	double end_par4 = get_wall_time();
	cout << "Vrijeme obrade paralelno sa cetiri jezgre: " << end_par4 - start_par4 << endl;

	////////////////////

    //prebacivanje iz proste matrice u Cimg objekt
	CImg<float> outImg(cols, rows);
	for (int i = 0; i < rows; i++)
		for (int j = 0; j < cols; j++)
		{
			outImg(i * cols + j) = sImg[i][j];
		}

	//spašavanje slike na disk
	outImg.save("outImg.bmp");
		
	return 0;
}

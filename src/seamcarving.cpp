#include<iostream>
#include<opencv2/opencv.hpp>
#include<math.h>
using namespace cv;
using namespace std;

//compute energy matrix for current image using dual gradient energy function
void computeEnergy(int h, int w, int c, int*** rgb, double** energy){
    for(int i=0; i<h; i++){
        for(int j=0; j<w; j++){
            int x_grad=0, y_grad=0;
            for(int k=0; k<c; k++){
                //calculate x-gradient for current pixel
                if(i==0){
                    x_grad += pow((rgb[1][j][k] - rgb[h-1][j][k]),2);
                }
                else if(i==h-1){
                    x_grad += pow((rgb[0][j][k] - rgb[h-2][j][k]),2);
                }
                else{
                    x_grad += pow((rgb[i-1][j][k] - rgb[i+1][j][k]),2);
                }
                //calculate y-gradient for current pixel
                if(j==0){
                    y_grad += pow((rgb[i][1][k] - rgb[i][w-1][k]),2);
                }
                else if(j==w-1){
                    y_grad += pow((rgb[i][0][k] - rgb[i][w-2][k]),2);
                }
                else{
                    y_grad += pow((rgb[i][j-1][k] - rgb[i][j+1][k]),2);
                }
            }
            //store in energy matrix
            energy[i][j] = sqrt((x_grad + y_grad));
        }
    }
}

//calculate cost matrix for vertical seam
void getVerticalCost(double** cost , int h , int w){
    for(int i = 1 ; i < h ; i++){
        for(int j = 0 ; j < w ; j++){
            if(j == 0)
                cost[i][j] += min(cost[i-1][j] , cost[i-1][j+1]);
            else if(j == w-1)
                cost[i][j] += min(cost[i-1][j-1] , cost[i-1][j]);
            else
                cost[i][j] += min(cost[i-1][j-1] , min(cost[i-1][j] , cost[i-1][j+1]));
        }
    }
}

//calculate cost matrix for horizontal seam
void getHorizontalCost(double** cost , int h , int w){
    for(int j = 1 ; j < w ; j++){
        for(int i = 0 ; i < h ; i++){
            if(i == 0)
                cost[i][j] += min(cost[i][j-1] , cost[i+1][j-1]);
            else if(i == h-1)
                cost[i][j] += min(cost[i][j-1] , cost[i-1][j-1]);
            else
                cost[i][j] += min(cost[i-1][j-1] , min(cost[i][j-1] , cost[i+1][j-1]));
        }
    }
}

//function to identify and remove vertical seam from image till required image width
void VerticalSeam(int ***rgb, int h, int w, int c, int op_W){
    while(w > op_W){

        //create matrix to store energy
        double** energy;
        energy = new double *[h];
        for(int i = 0; i < h; i++)
            energy[i] = new double[w];
        computeEnergy(h,w,c,rgb,energy); //compute energy matrix

        double** cost = energy;
        getVerticalCost(cost,h,w); //calculate cost matrix

        //identify min cost pixel from last row for current image
        int min_ind = 0 , min_cost = cost[h-1][0];
        for(int j = 1; j < w; j++){
            if(cost[h-1][j] <= min_cost){
                min_cost = cost[h-1][j];
                min_ind = j;
            }
        }

        //remove min cost pixels for vertical seam
        for(int i=h-1; i>=1; i--){
            for(int j=min_ind; j< w-1; j++){
                for(int k=0; k<c; k++){
                    rgb[i][j][k] = rgb[i][j+1][k];
                }
            }
            int j = min_ind;
            if(min_ind == 0){
                min_ind = (cost[i-1][j] < cost[i-1][j+1] ? j : j+1);
            }
            else if(min_ind == w-1){
                min_ind = (cost[i-1][j-1] < cost[i-1][j] ? j-1 : j);
            }
            else{
                double a = cost[i-1][j-1], b = cost[i-1][j];
                if(a < b){
                    if(a < cost[i-1][j+1])
                        min_ind = j-1;
                    else
                        min_ind = j+1;
                }
                else{
                    if(b < cost[i-1][j+1])
                        min_ind = j;
                    else
                        min_ind = j+1;
                }
            }
        }

        //delete energy matrix
        for(int i = 0; i < h; i++){
            delete [] energy[i];
        }
        delete [] energy;
        w--;
    }

}

//function to identify and remove horizontal seam from image till required image height
void HorizontalSeam(int ***rgb, int h, int w, int c, int op_H){
    while (h > op_H){

        //create matrix to store energy
        double** energy;
        energy = new double *[h];
        for(int i = 0 ; i < h ; i++)
            energy[i] = new double[w];
        computeEnergy(h,w,c,rgb,energy); //compute energy matrix

        double** cost = energy;
        getHorizontalCost(cost,h,w); //calculate cost matrix

        ////identify min cost pixel from first column for current image
        int min_ind = 0 , min_cost = cost[0][0];
        for(int i = 0 ; i < h ; i++){
            if(cost[i][0] < min_cost){
                min_cost = cost[i][0];
                min_ind = i;
            }
        }

        //remove min cost pixels for horizontal seam
        for(int j=0; j<w-1; j++){
            for(int i=min_ind; i<h-1; i++){
                for(int k=0; k<c; k++){
                    rgb[i][j][k] = rgb[i+1][j][k];
                }
            }
            int i = min_ind;
            if(min_ind == 0){
                min_ind = (cost[i][j+1] < cost[i+1][j+1] ? i : i+1);
            }
            else if(min_ind == h-1){
                min_ind = (cost[i-1][j+1] < cost[i][j+1] ? i-1 : i);
            }
            else{
                double a = cost[i-1][j], b = cost[i][j];
                if(a < b){
                    if(a < cost[i+1][j])
                        min_ind = i-1;
                    else
                        min_ind = i+1;
                }
                else{
                    if(b < cost[i+1][j])
                        min_ind = i;
                    else
                        min_ind = i+1;
                }
            }
        }

        //delete energy matrix
        for(int i = 0 ; i < h ; i++){
            delete [] energy[i];
        }
        delete [] energy;
        h--;
    }
}

int*** SeamCarving(Mat img, int in_H, int in_W, int op_H, int op_W){

    //store input image in rgb array
    int ***rgb;
    rgb = new int **[in_H];
    for(int i=0; i<in_H; i++){
        rgb[i] = new int* [in_W];
        for(int j=0; j<in_W; j++){
            rgb[i][j] = new int [3];
        }
    }
    for(int i=0; i<in_H; i++){
        for(int j=0; j<in_W; j++){
            rgb[i][j][0] = img.at<Vec3b>(i,j)[0];
            rgb[i][j][1] = img.at<Vec3b>(i,j)[1];
            rgb[i][j][2] = img.at<Vec3b>(i,j)[2];
            
        }
    }

    int curr_H = in_H, curr_W = in_W;
    //resize width of image by removing vertical seam
    VerticalSeam(rgb, curr_H, curr_W, 3, op_W);
    curr_W = op_W;

    //resize height of image by removing horizontal seam
    HorizontalSeam(rgb, curr_H, curr_W, 3, op_H);
    curr_H = op_H;
    return rgb;
}


int main(int argc, char** argv)
{
    // get inputs
    string in_Img_Path; //input image path
    string out_Img_Path; //output image path
    int op_height, op_width;

    cout<<"Enter input image path:"<<endl;
    cin >> in_Img_Path;
    
    cout<<"Enter output image path:"<<endl;
    cin >> out_Img_Path;

    Mat image = imread(in_Img_Path,IMREAD_COLOR); // read input image
    if (image.empty()) {
        cout << "Input image not found" << endl;
        exit(EXIT_FAILURE);
    }

    int in_height = image.rows, in_width = image.cols; //get image height and width

    //get height and width for output image
    cout<<"Enter output image height [<="<<in_height<<"] and width [<="<<in_width<<"]"<<endl;
    cin >> op_height >> op_width;
    if(op_height > in_height || op_width > in_width)
        cout<<"Invalid image height/width. For output image, height must be less than "<<in_height+1<<" and width must be less than "<<in_width+1<<endl;

    //resize image using Seam Carving
    int ***rgb = SeamCarving(image, in_height, in_width, op_height, op_width);

    //create output image
    Mat op_img =  Mat(op_height, op_width, image.type());
    for(int i=0; i<op_height; i++){
        for(int j=0; j<op_width; j++){
            for(int k=0; k<3; k++){
                op_img.at<Vec3b>(i,j)[k] = rgb[i][j][k];
            }
        }
    }
    bool saved = imwrite(out_Img_Path, op_img);
    // if the image is not saved
    if (!saved)
        cout << "Error while saving output image" << endl;
    return 0;
}

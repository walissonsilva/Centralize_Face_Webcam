#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Porta com a qual o RS232 se comunicará
#define COM 24

using namespace std;
using namespace cv;

void detectAndDisplay(Mat &frame);

/** Variáveis Globais */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Centralize Face";
char bytes[6], b[4];
stringstream ss;

/** Função Main **/
int main(){
    VideoCapture cap(1);
    Mat frame;

    /// 1. Abrindo o arquivo de detecção de face
    if( !face_cascade.load(face_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };

    /// 2. Lendo o vídeo
    if(cap.isOpened()){
        while(true){
            cap >> frame;

            /// 3. Lançando o frame na função
            if(!frame.empty()){
                detectAndDisplay(frame);
            } else {
                cout << "Frame não capturada.\n";
                break;
            }

            int c = waitKey(10);
            if((char) c == 'c'){
                break;
            }
        }
    }

    cout << "Fim do programa...\n";
    return 0;
 }

void detectAndDisplay(Mat &frame){
    static vector<Rect> faces;
    static Mat frame_gray;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    size_t tam = faces.size();

    for(size_t i = 0; i < tam; i++){
        Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);

        /// Cria uma elipse em volta do pixel central da face para destacá-la
        //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 255, 255 ), 4, 8, 0 );
        rectangle(frame, faces[i], Scalar(255, 0, 0), 5);
    }

    cvtColor(frame, frame, COLOR_BGR2GRAY);
    //threshold(frame, frame, 100, 255, THRESH_BINARY_INV);
    Laplacian(frame, frame, frame.depth(), 3);

      /// Exibir a frame do vídeo que acabou de ser processada
      imshow( window_name, frame );
}

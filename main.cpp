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

// Biblioteca que permite a comunicação serial
#include "rs232.h"

// Porta com a qual o RS232 se comunicará
#define COM 24

// ACM0 -> 24; ACM1 -> 25

using namespace std;
using namespace cv;

void detectAndDisplay(Mat &frame);
static void onMouse(int event, int x, int y, int, void*);

/** Variáveis Globais */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;
string window_name = "Centralize Face";
string window2_name = "Face";
char bytes[6], b[4];
stringstream ss;
Mat frame;

/** Função Main **/
int main(){
    VideoCapture cap(0);

    /// Testa a abertura da porta serial do Arduino
    /// Se a porta serial não for carregada a função retorna 1 e o programa é encerrado
    if(RS232_OpenComport(COM, 115200, "8N1")){
        printf("Erro ao carregar a porta.\n");
        //return 1;
    }

    /// 1. Abrindo o arquivo de detecção de face
    if( !face_cascade.load(face_cascade_name) ){ printf("--(!)Error loading\n"); return -1; };

    /// 2. Lendo o vídeo
    namedWindow(window_name, WINDOW_AUTOSIZE);
    namedWindow(window2_name, WINDOW_AUTOSIZE);
    setMouseCallback(window_name, onMouse, 0);
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

    RS232_CloseComport(COM);
    destroyAllWindows();

    cout << "Fim do programa...\n";
    return 0;
 }

void detectAndDisplay(Mat &frame){
    static vector<Rect> faces;
    static Mat frame_gray;
    Mat rosto;
    int x = 0, y = 0;

    cvtColor(frame, frame_gray, CV_BGR2GRAY);
    equalizeHist(frame_gray, frame_gray);

    //-- Detect faces
    face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

    size_t tam = faces.size();

    for(size_t i = 0; i < tam; i++){
        Point center(faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5);



        /// Criando uma imagem com somente o rosto detectado
        rosto.create(Size(faces[i].width + 23, faces[i].height + 23), CV_8UC3);
        //Mat mask(frame_gray.size(), frame_gray.type());
//        for (int j = 0; j < mask.rows; j++){
//            for (int k = 0; k < mask.cols; k++){
//                if (j >= faces[i].x && j <= faces[i].x + faces[i].width &&
//                        k >= faces[i].y && k <= faces[i].y + faces[i].height){
//                    mask.at<uchar>(j, k) = 255;
//                } else {
//                    mask.at<uchar>(j, k) = 0;
//                }
//            }
//        }

        for (int j = 0; j < frame_gray.cols; j++){
           for (int k = 0; k < frame_gray.rows; k++){
               if (j >= faces[i].x - 10 && j <= faces[i].x + faces[i].width + 9 &&
                   k >= faces[i].y - 10 && k <= faces[i].y + faces[i].height + 9){
                   rosto.at<Vec3b>(x, y).val[0] = frame.at<Vec3b>(k, j).val[0];
                   rosto.at<Vec3b>(x, y).val[1] = frame.at<Vec3b>(k, j).val[1];
                   rosto.at<Vec3b>(x++, y).val[2] = frame.at<Vec3b>(k, j).val[2];
               }
           }

           if (x != 0){
               x = 0; y++;
           }
        }

        /// Transforma o valor da coodenada x do pixel central da face (int --> char *)
        ss.clear();
        ss << center.x;
        ss >> b;

        cout << center.x << " " << center.y << endl;

        strcpy(bytes, "b");
        strcat(bytes, b);
        strcat(bytes, "e");

        /// Envia o valor da coodenada x do pixel central da face para a porta serial do Arduino
        if (strlen(bytes) == 5){
            //cout << bytes << endl;
            RS232_cputs(COM, bytes);
        }

        /// Cria uma elipse em volta do pixel central da face para destacá-la
        ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 255, 255 ), 4, 8, 0 );
    }

      /// Exibir a frame do vídeo que acabou de ser processada
      imshow( window_name, frame );
      if (!rosto.empty())
          imshow( window2_name, rosto );
}

static void onMouse(int event, int x, int y, int, void*){
    if(event != EVENT_LBUTTONDOWN){
        if (event != EVENT_LBUTTONUP){
            //cout << "Retornei!\n";
            return;
        }
    }

    Point p = Point(x, y);
    cout << p.x << " " << p.y << endl;

    circle(frame, p, 5, Scalar(255, 0, 0), 3);
}

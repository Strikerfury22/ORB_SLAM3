#include<vector> 
#include<opencv2/core/core.hpp>
#include<iostream>

class extractedORBInfo{
    public:
    //Variables
    double tr = -1; //TimeStamp
    std::vector<cv::KeyPoint> _mvKeys; //KeyPoints 
    cv::Mat _mDescriptors; //Descriptores
    int mono = -1; //Lo que se devuelve tras extraer KeyPoints y descriptores (puede que el número de keyPoints)
    cv::Mat _grayImage; //Imagen en blanco y negro de la que se ha extraido la información
    int n_image = -1; //Identificador de la imagen

    int lado = -1; //0 Para izquierda, 1 Para derecha

    //Constructor Vacío
    extractedORBInfo();
    
    //Constructor por copia
    extractedORBInfo(const extractedORBInfo& referencia);

    //Destructor
    ~extractedORBInfo();
};
#include "extractedORBInfo.h"

//Empty Constructor
extractedORBInfo::extractedORBInfo(){
    //std::cout << "Creado extractedORBInfo vacío" << std::endl;
    //CV::Mat y std::vector no requieren de inicialización (frame no lo hace). Tampoco hacen falta los demás, porque en el .h les damos un valor por defecto.
}

//Copy Constructor
extractedORBInfo::extractedORBInfo (const extractedORBInfo& referencia){
    //std::cout << "Copiando imagen " << referencia.n_image << " lado " << referencia.lado << std::endl;
    tr = referencia.tr;
    _mvKeys = referencia._mvKeys;
    _mDescriptors = referencia._mDescriptors.clone();
    mono = referencia.mono;
    _grayImage = referencia._grayImage.clone();
    n_image = referencia.n_image;
    lado = referencia.lado;
}

extractedORBInfo::~extractedORBInfo(){
    //std::cout << "Se ha llamado al destructor de la información extraída para la imagen con identificador " << n_image << " lado " << lado <<std::endl;
}
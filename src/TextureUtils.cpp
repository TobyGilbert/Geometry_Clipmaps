#include "TextureUtils.h"

GLuint TextureUtils::createTexture(const GLchar *path){
   ilInit();
   ILuint image = ilGenImage();

   ilBindImage(image);

   ILboolean loadSuccess = ilLoadImage(path);
   if(!loadSuccess){
      std::cerr<<"Failed to load image: "<<path<<std::endl;
      ilBindImage(NULL);
      ilDeleteImage(image);
      return NULL;
   }

   ILboolean convertSuccess = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
   if(!convertSuccess){
      std::cerr<<"Failed to convert image: "<<path<<std::endl;
      ilBindImage(NULL);
      ilDeleteImage(image);
      return NULL;
   }

   GLuint texture;
   glGenTextures(1, &texture);
   glBindTexture(GL_TEXTURE_2D, texture);

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


   glTexImage2D(GL_TEXTURE_2D, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
   glBindTexture(GL_TEXTURE_2D, NULL);

   ilBindImage(NULL);
   ilDeleteImage(image);

   return texture;
}

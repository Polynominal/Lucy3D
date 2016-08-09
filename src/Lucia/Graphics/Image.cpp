#include "Lucia\Graphics\Image.h"
#include <iostream>
#include <map>
#include <string>
using namespace Lucia;
template <typename T>
bool POT(T number)
{
    int x = (int)number;
    return (x != 0) && ((x & (x - 1)) == 0);
}
using namespace Graphics;
Image::Image(std::string path)
{
    if (!ilutInitialized)
    {
        ilutRenderer(ILUT_OPENGL);
        ilInit();
        iluInit();
        ilutInit();
        ilutRenderer(ILUT_OPENGL);
        // ---
        ILenum ilError = ilGetError();
        if( ilError != IL_NO_ERROR )
        {
            LOG << "Fatal" << "Error initializing DevIL!" + std::string(iluErrorString( ilError )) << std::endl;
            sendErrorSignal("DeviLL (image loading) has failed to initialize!",true);
        };
        ilutInitialized = true;
    }
    std::map<std::string,ILuint>::const_iterator iter = Loaded_Images.find(path);
    if (iter==Loaded_Images.end())
    {

        ilGenImages(1, &imageID);
        ilBindImage(imageID);
        bool success = ilLoadImage(path.c_str());;
        ILenum error;

        if (success == IL_TRUE)
        {
            LOG << "Building" << "New Image" << std::endl;
            ILinfo ImageInfo;
            iluGetImageInfo(&ImageInfo);
            if (ImageInfo.Origin == IL_ORIGIN_UPPER_LEFT)
            {
                LOG << "Warning" << "Flipping image!" << std::endl;
                iluFlipImage();
            }

            // Convert the image into a suitable format to work with
            // NOTE: If your image contains alpha channel you can replace IL_RGB with IL_RGBA
            success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

            // Quit out if we failed the conversion
            if (!success)
            {
                error = ilGetError();
                LOG << "Fatal" << "Image conversion failed - IL reports error: " << error << " - " << iluErrorString(error) << std::endl;
                sendErrorSignal("Image conversion failed!- " + std::string(iluErrorString(error)));
            }
            else {
                success = true;
                source = path;
                Loaded_Images.insert(std::pair<std::string,int>(path,imageID));
            };
        }
        else {
            ILenum  error = ilGetError();
            LOG << "Fatal" << "failed to load: " << path << std::endl;
            LOG << "Fatal" << error << std::endl;
            sendErrorSignal("Image load failed: \n" + path + "\n Error: " + iluErrorString(error));
        };
        ilBindImage(0);
    }
    else{
        generated = true;
        success = true;
        imageID = iter->second;
        source = path;
    };
}
void Image::generateMipMap()
{
    #if defined LUCIA_USE_GLES2 || defined LUCIA_USE_GLES3
    glGenerateMipmap(GL_TEXTURE_2D);
    #else
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    #endif
}
void Image::generate()
{
    if (!generated)
    {
        LOG << "Building" << "IMAGE:" << source << std::endl;
        ilBindImage(imageID);
            width = ilGetInteger(IL_IMAGE_WIDTH);	// Image width
            height  = ilGetInteger(IL_IMAGE_HEIGHT);
            glGenTextures(1, &textureID);

            // Bind thtexture to a name
            glBindTexture(GL_TEXTURE_2D, textureID);

            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

            // Specify the texture specification
            glTexImage2D(GL_TEXTURE_2D, 				// Type of texture
                         0,				// Pyramid level (for mip-mapping) - 0 is the top level
                         ilGetInteger(IL_IMAGE_FORMAT),	// Internal pixel format to use. Can be a generic type like GL_RGB or GL_RGBA, or a sized type
                         width,	// Image width
                         height,	// Image height
                         0,				// Border width in pixels (can either be 1 or 0)
                         ilGetInteger(IL_IMAGE_FORMAT),	// Format of image pixel data
                         GL_UNSIGNED_BYTE,		// Image data type
                         ilGetData());			// The actual image data itself
                if (POT(width) and POT(height)){glGenerateMipmap(GL_TEXTURE_2D);};
        ilBindImage(0);
        generated = true;
    }
};
void Image::setName(std::string new_mode)
{
    name = new_mode;
};
void Image::bind()
{
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
}
void Image::bind(int number)
{
    GLfloat no = number;

    glActiveTexture(GL_TEXTURE0 + number);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLint programID = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM,&programID);
    if (name != "")
    {
        glUniform1i(glGetUniformLocation(programID,name.c_str()),no);
    };
}
GLuint Image::getID()
{
    return textureID;
}
void Image::remove()
{
    if (generated)
    {
        LOG << "Info" << "Removing image: " << source << std::endl;
        Loaded_Images.erase(source);
        glDeleteTextures(1,&textureID);
        ilDeleteImages(1, &imageID);
        generated = false;
    }
}
Image::~Image()
{
    remove();
    //dtor
}

/*********************************************************
 *
 * PHP-OCR Extension
 *
 * written by Glenn De Backer <glenn@simplicity.be>
 *
  *********************************************************/

// Standard
#include <iostream> 
#include <fstream> 
#include <typeinfo>

// PHP-CPP
#include <phpcpp.h> 
// Tesseract
#include <tesseract/baseapi.h>
// Leptonica
#include <leptonica/allheaders.h>

#define PHP_OCR_VERSION "1.0"

/**
 * Returns bool if file exist
 **/
bool doesFileExist(std::string fileName) {
	std::ifstream infile(fileName);
	return infile.good();
}

/**
 * Returns string containing php-ocr version
 **/
Php::Value php_ocr_version()
{
	return PHP_OCR_VERSION;
}

/**
* Apply OCR on image
**/
Php::Value php_ocr_recognize(Php::Parameters &params)
{
	Php::Value fileName;
	Php::Value language;
	Php::Value returnValue;
	Pix *image;

	// handle parameters
	if (params.size() == 2) {
		fileName = params[0];
		language = params[1];
	}
	else if (params.size() == 1) {
		fileName = params[0];
		language = "eng";
	} 
	else {
		throw Php::Exception("Wrong number of parameters");
	}

	// initialize tesseract object 
	tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI();

	// Initialize tesseract-ocr with English, without specifying tessdata path
	if (api->Init(NULL, language.stringValue().c_str())) {
		throw Php::Exception("Could not initialize tesseract. Language datasets available ?");
	}

	// check if file exist
	if (doesFileExist(fileName.stringValue())) {

		// Open input image with leptonica library
		image = pixRead(fileName.stringValue().c_str());

		api->SetImage(image);

		// Get OCR result
		returnValue = api->GetUTF8Text();
	}
	else {
		// if file doesn't exist raise an error and throw an php exception
		throw Php::Exception("Could not find image");
	}

	// Destroy used object and release memory
	api->End();
	pixDestroy(&image);

	return returnValue;
}


/**
 *  tell the compiler that the get_module is a pure C function
 */
extern "C" {
    
    /**
     *  Function that is called by PHP right after the PHP process
     *  has started, and that returns an address of an internal PHP
     *  strucure with all the details and features of your extension
     *
     *  @return void*   a pointer to an address that is understood by PHP
     */
    PHPCPP_EXPORT void *get_module() 
    {
        // static(!) Php::Extension object that should stay in memory
        // for the entire duration of the process (that's why it's static)
		static Php::Extension extension("php-ocr", PHP_OCR_VERSION);
        
		// register methods
		extension.add("php_ocr_version", php_ocr_version);
		extension.add("php_ocr_recognize", php_ocr_recognize, {
			Php::ByVal("filename", Php::Type::String, true),
			Php::ByVal("language", Php::Type::String, false)
		});
		
		// return the extension
        return extension; 
    }
}

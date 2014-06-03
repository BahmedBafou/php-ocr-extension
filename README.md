PHP-OCR
=========

PHP-OCR is PHP extension that makes it possible to apply OCR (using Googles tesseract engine) on an Image.

Version
----

1.0


Installation
--------------

To compile the extension you will also need to compile the php-cpp, tesseract and the leptonica library. They are provided in the folder third_party.



For each library you need to execute following commands

```
make
make install
```


To enable the extensions (apache2)

```
php5enmod php-ocr
```

Usage
--------------
```
$text = php_ocr_recognize("test/lorem.jpg");

echo $text // Lorem ipsum dolor sit amet, consectetur adipiscing elit. In augue mauris, tristique bibendum scelerisque sed, pulvinar et lacus. Praesent ut metus eu velit dictum aliquam eta orci
```

License
----

LGPL


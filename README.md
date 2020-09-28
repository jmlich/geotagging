
[![Copr build status](https://copr.fedorainfracloud.org/coprs/jmlich/laa/package/com.github.jmlich.geotagging/status_image/last_build.png)](https://copr.fedorainfracloud.org/coprs/jmlich/laa/package/com.github.jmlich.geotagging/)

Translate to your language at [transifex](https://www.transifex.com/jozef-mlich/geotagging/dashboard/).

![screenshot](./data/geotagging.png)
![screenshot2](./data/screenshot2.png)

[![Click to play on youtube](https://img.youtube.com/vi/BZhxQC5wgNI/0.jpg)](https://www.youtube.com/watch?v=BZhxQC5wgNI)


www.fit.vutbr.cz/study/DP/BP.php.cs?id=11578

Postup instalace:

# Instalace knihoven na linux: 
nainstalovat knihovny zlib (testováno zlib-1.2.3), expat (testováno expat-2.0.1) a exiv2 (testováno exiv2-0.21.1)
## Instalace exiv2:
- ./configure --with-zlib=DIR  --with-expat=DIR 
- make
- make install


# Instalace knihoven na windows: 
- pro překlad je nutné použít stejnou verzi MinGW jaká je použita v Qt
- přeložené statické knihovny umístit do adresáře ./libs/
- hlavičkové soubory knihovny exiv2 umístit do adresáře ./exiv2/

## Překlad zlib:
- ./configure;
- make; 
- make install

## Překlad expat:
- ./configure;
- make; 
- make install
	
## Překlad exiv2:
- ./configure --with-zlib=DIR --with-expat=DIR --disable-visibility; 
- make; 
- make install


Aplikace byla vyvíjena v Qt5
překlad:
- cmake ..
- cmake --build .

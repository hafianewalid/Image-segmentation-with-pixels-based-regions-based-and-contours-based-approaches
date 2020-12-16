# Image-segmentation-with-pixels-based-regions-based-and-contours-based-approaches

## Pixels based approuches

### C++ compile :

`$ cmake . && cmake --build . --target $target`


### C++ Run :

* Thresholding method 

`$ ./PixelBased img.jpg threshold 130 ( param1 : image , param2 : method, param3 : threshold )`

* Histogramme ranges method

`$ ./PixelBased img.jpg histo_ranges 100 ( param1 : image , param2 : method , param3 : range)`

* Otsu method

`$ ./PixelBased img.jpg otsu ( param1 : image , param2 method )`

* Kmeans method

`$ ./PixelBased img.jpg kmeans ( param1 : image , param2 : method , param3 : K )`

### Python run :

* Thresholding method 

`$ python TP2.py img.jpg threshold 150 ( param1 : image , param2 : method, param3 : threshold )`

* Histogramme ranges method

`$ python TP2.py img.jpg histo_ranges ( param1 : image , param2 method )`

* Otsu method

`$ python TP2.py img.jpg otsu ( param1 : image , param2 method )`

### Exemples : 

* Thresholding method 

<img src="https://drive.google.com/uc?export=view&id=1stP-wAIS5v5nvsq6Ek1mSEZPXeafr_I0">

* Histogramme ranges method

<img src="https://drive.google.com/uc?export=view&id=1u61A388FJkX9kx980Q03lBptL6hH1BbQ">

```
 
Results:
  Range = 50  
  Values     <0 , 50 >   <50 , 100 >   <100 , 150 >   <150 , 200 >   <200 , 250 >   <250 , 255> 
  Num            5             0              5             0              4              2   
  
  Range = 100
  Values     <0 , 100 >   <100 , 200 >   <200 , 255> 
  Num            5             5              6     

```
 
* Otsu method

<img src="https://drive.google.com/uc?export=view&id=1DWFybBLIgXyZdMhuPgSkNXguQ8zXbGCH">

* Kmeans method

<img src="https://drive.google.com/uc?export=view&id=1nUAaFNmbZZuHqSkZ7DYLq5jXz-pCtvSN">



## Regions based approuches

### C++ run :
*region growing method

`$ ./segreg`
or 
`$ ./segreg img.png`

### Python run :
*graph cut method

`$ python graphcut.py img.png`

### Region growing interface

<img src="https://drive.google.com/uc?export=view&id=1pBDAU6zaFPaT2dK_czoPkf_8SY--xBWw">

### Exemples : 

* Region growing method

<img src="https://drive.google.com/uc?export=view&id=1quoqfGmwPmK8kX33j67zIv2jhmA1_vXn">

* Graph cut method

<img src="https://drive.google.com/uc?export=view&id=1BFlElBI8ESD5wAJ3wQh3n5ihszfI2aXv">

## contours based approaches
 ### Python run :
 
 * Run contours orerations : 
 
`$ python main.py daisy.png delatation`

`$ python main.py daisy.png erosion`

`$ python main.py daisy.png ouverture`

`$ python main.py daisy.png fermuture`

`$ python main.py daisy.png Gradient_interne`

`$ python main.py daisy.png Gradient_externe`

`$ python main.py daisy.png G_morphologique`

`$ python main.py daisy.png minima`

 * Run the segmentation pipeline : 

`$ python main.py daisy.png segmentation`

### Exemples :
Contour based method :

<img src="https://drive.google.com/uc?export=view&id=1s6eDAzizfs9uG-qKb4-2iE3vmT-urPei">


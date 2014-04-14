#include "testCamera.c"


int isBlue(float *h, float *s, float *v){


   int min_saturation = 56;

   int max_saturation = 100;

   int min_hue = 228;

   int max_hue = 248;

   int min_value = 80;

   int max_value = 100;

   int boundary_min = min_saturation*min_saturation*(max_hue-min_hue)/360*min_value;

   int boundary_max = max_saturation*max_saturation*(max_hue-min_hue)/360*max_value;

   

   int comp_value = (*s)*(*s)*(*h)/360*(*v);

   if(comp_value>boundary_min && comp_value<boundary_max)

       return 1;

   return 0;

}

int isLightBlue(float *h, float *s, float *v){


   int min_saturation = 196;

   int max_saturation = 219;

   int min_hue = 196;

   int max_hue = 219;

   int min_value = 71;

   int max_value = 94;

   int boundary_min = min_saturation*min_saturation*(max_hue-min_hue)/360*min_value;

   int boundary_max = max_saturation*max_saturation*(max_hue-min_hue)/360*max_value;

   

    int comp_value = (*s)*(*s)*(*h)/360*(*v);

   if(comp_value>boundary_min && comp_value<boundary_max)

       return 1;

   return 0;

}

int isDeepBlue(float *h, float *s, float *v){

   int min_saturation =100;

   int max_saturation =100;

   int min_hue =216;

   int max_hue =276;

   int min_value =50;

   int max_value =67;

   int boundary_min = min_saturation*min_saturation*(max_hue-min_hue)/360*min_value;

   int boundary_max = max_saturation*max_saturation*(max_hue-min_hue)/360*max_value;

   

    int comp_value = (*s)*(*s)*(*h)/360*(*v);

   if(comp_value>boundary_min && comp_value<boundary_max)

       return 1;

   return 0;

}
